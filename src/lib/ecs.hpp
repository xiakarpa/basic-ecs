#ifndef ECS_H
#define ECS_H

#include <cstdint>
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <cassert>




using Entity = std::uint32_t;

class IComponent {
public:
	virtual const char* type();
};

template <typename... Tail> class System;


/*
 зачем тут класс требуется и указатели
если бы можно было делать без класса компонента, то всё реализовывалось бы на массиве каких-то анонимных T которые даже наследоваться не должны
*/

/*
почему shared_ptr
казалось бы в нашем случае логично использовать unique_ptr, так как только менеджер должен владеть компонентами
но во первых может возникнуть ситуация, когда два разных энтити используют один компонент (условно позиция игрока используется в игроке и во враге для ИИ)
а во вторых мы отдаем указатели в функции getComponent - возвращать в таких функциях weak_ptr можно и в целом правильно, но если вдруг возникнет еще один глобальный менеджер, знающих о компонентах - их туда не получится добавить
*/
class ComponentManager {
private:
	std::unordered_map<Entity, std::vector<std::shared_ptr<IComponent>>> components;
	Entity nextId = 0;
	
public:
	std::vector<Entity> entities;
	
	Entity createEntity();
	void removeEntity(Entity entity);
	bool hasEntity(Entity entity);
	
	template <typename T> void connectComponent(Entity entity, T component) {
		static_assert(std::is_base_of<IComponent, T>::value);
		assert((hasEntity(entity)) && "ComponentManager::connectComponent: Can't connect component. No such entity.");
		assert((!hasComponent<T>(entity)) && "ComponentManager::connectComponent: Can't connect component. Component already connected to this entity.");
		std::shared_ptr<T> ptr = std::make_shared<T>(component);
		components[entity].push_back(std::static_pointer_cast<IComponent>(ptr));
	}
	template <typename T> bool hasComponent(Entity entity) {		
		return hasComponent(entity, typeid(T).name());
	}
	bool hasComponent(Entity entity, const char* type);
	template <typename T> std::shared_ptr<T> getComponent(Entity entity) {
		if (!hasEntity(entity)) {
			return nullptr;
		}
		
		const char* type = typeid(T).name();
		for (auto& component : components[entity]) {
			if (component->type() == type) {
				return std::dynamic_pointer_cast<T>(component);
			}
		}
		
		return nullptr;
	}
};


/*
гораздо проще и адекватнее было бы хранить сцену как raw pointer
но при наличии умных указателей это бы смешало стили и выглядело бы убого
*/
class ISystem {
protected: 
	virtual bool hasComponents(Entity entity) = 0;
public:
	virtual ~ISystem() = default;
	virtual const char* type() = 0;
	virtual void connectManager(std::shared_ptr<ComponentManager>& componentManager) = 0;
	virtual void init() {}
	virtual void update(float dt) {}
};

template <typename... Tail>
class System : public ISystem {
protected:
	std::vector<const char*> types;
	std::shared_ptr<ComponentManager> componentManager;

	bool hasComponents(Entity entity) final {
		if (!componentManager) {
			return false;
		}
		
		for (auto& type : types) {
			if (!componentManager->hasComponent(entity, type)) {
				return false;
			}
		}
		
		return true;
	}
	
public:
	System() {
		([&] {
			static_assert(std::is_base_of<IComponent, Tail>::value);
			types.push_back(typeid(Tail).name());
		} (), ...);
	}
	
	const char* type() final {
		return typeid(*this).name();
	}
	
	void connectManager(std::shared_ptr<ComponentManager>& componentManager) final {
		this->componentManager = componentManager;
	}
};


class Scene {
private:
	std::vector<std::shared_ptr<ISystem>> systems;
	std::shared_ptr<ComponentManager> componentManager;
	
public:
	Scene() {
		componentManager = std::make_shared<ComponentManager>();
	}

	Entity createEntity() {
		return componentManager->createEntity();
	}
	void removeEntity(Entity entity) {
		componentManager->removeEntity(entity);
	}
	bool hasEntity(Entity entity) {
		return componentManager->hasEntity(entity);
	}
	
	template <typename T> 
	void connectComponent(Entity entity, T component) {
		componentManager->connectComponent<T>(entity, component);
	}
	template <typename T> 
	bool hasComponent(Entity entity) {
		return componentManager->hasComponent<T>(entity);
	}
	bool hasComponent(Entity entity, const char* type) {
		return componentManager->hasComponent(entity, type);
	}
	template <typename T> 
	std::shared_ptr<T> getComponent(Entity entity) {
		return componentManager->getComponent<T>(entity);
	}
	
	template <typename T>
	void connectSystem() {
		static_assert(std::is_base_of<ISystem, T>::value);
		assert((!hasSystem<T>()) && "ComponentManager::connectSystem: Can't connect system. System already connected to this manager.");
		std::shared_ptr<T> system = std::make_shared<T>();
		system->connectManager(componentManager);
		systems.push_back(std::dynamic_pointer_cast<ISystem>(system));
	}
	template <typename T> 
	bool hasSystem() {
		return getSystem<T>() != nullptr;
	}
	template <typename T> 
	std::shared_ptr<T> getSystem() {
		for (const auto& system : systems) {
			if (system->type() == typeid(T).name()) {
				return std::dynamic_pointer_cast<T>(system);
			}
		}
		return nullptr;
	}
	void initSystems() {
		for (const auto& system : systems) {
			system->init();
		}
	}
	void updateSystems(float dt) {
		for (const auto& system : systems) {
			system->update(dt);
		}
	}
};








struct Vec3 {
	float x;
	float y;
	float z;
};

class TransformComponent : public IComponent {
public:
	Vec3 position;
	Vec3 rotation;
	Vec3 scale;
	
	TransformComponent(Vec3 position, Vec3 rotation, Vec3 scale) : position(position), rotation(rotation), scale(scale) {}
};

class VelocityComponent : public TransformComponent {
public:
	using TransformComponent::TransformComponent;
};

class HealthComponent : public IComponent {
public:
	std::uint32_t hp;
	std::uint32_t maxHp;
	
	HealthComponent(std::uint32_t hp, std::uint32_t maxHp) : hp(hp), maxHp(maxHp) {}
};


class MovementSystem : public System<TransformComponent, VelocityComponent> {
public:
	using System::System;
	
	void init() override;
	void update(float dt) override;
};


#endif