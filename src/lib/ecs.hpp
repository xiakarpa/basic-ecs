#ifndef ECS_HPP
#define ECS_HPP

#include <cstdint>
#include <memory>
#include <vector>
#include <unordered_map>


using Entity = std::uint32_t;

#pragma region Component

class IComponent {
public:
	// виртуальная функция для всех компонентов, чтобы получать их уникальное "имя" - то есть их тип, для их поиска и отслеживания
	virtual const char* type();
};

// менеджер компонент-сущностей, который нужен системам, потому что мы не можем дать доступ к сцене системам из-за обоюдной зависимости
class ComponentManager {
private:
	std::unordered_map<Entity, std::vector<std::shared_ptr<IComponent>>> components;
	Entity nextId = 0;
	
public:
	std::vector<Entity> entities;
	
	Entity createEntity();
	void removeEntity(Entity entity);
	bool hasEntity(Entity entity);
	
	// шаблоны использующие имя типа для поиска компонентов
	template <typename T> void connectComponent(Entity entity, T component);
	template <typename T> bool hasComponent(Entity entity);
	bool hasComponent(Entity entity, const char* type);
	template <typename T> std::shared_ptr<T> getComponent(Entity entity);
};

#pragma endregion

#pragma region System

// необходимо для каста, так как мы не можем делать каст System<Comp1...> к System<>
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

	bool hasComponents(Entity entity) final;
	
public:
	System();
	const char* type() final;
	void connectManager(std::shared_ptr<ComponentManager>& componentManager) final;
};

#pragma endregion

class Scene {
private:
	std::vector<std::shared_ptr<ISystem>> systems;
	std::shared_ptr<ComponentManager> componentManager;
	
public:
	Scene();

	Entity createEntity();
	void removeEntity(Entity entity);
	bool hasEntity(Entity entity);
	
	template <typename T> void connectComponent(Entity entity, T component);
	template <typename T> bool hasComponent(Entity entity);
	bool hasComponent(Entity entity, const char* type);
	template <typename T> std::shared_ptr<T> getComponent(Entity entity);
	
	template <typename T> void connectSystem();
	template <typename T> bool hasSystem();
	template <typename T> std::shared_ptr<T> getSystem();
	void initSystems();
	void updateSystems(float dt);
};


#include "ecs.tpp"

#endif