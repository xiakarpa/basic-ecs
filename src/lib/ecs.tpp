#ifndef ECS_TPP
#define ECS_TPP

#include <cassert>


#pragma region ComponentManager

template <typename T> 
void ComponentManager::connectComponent(Entity entity, T component) {
	static_assert(std::is_base_of<IComponent, T>::value);
	assert((hasEntity(entity)) && "ComponentManager::connectComponent: Can't connect component. No such entity.");
	assert((!hasComponent<T>(entity)) && "ComponentManager::connectComponent: Can't connect component. Component already connected to this entity.");
	std::shared_ptr<T> ptr = std::make_shared<T>(component);
	components[entity].push_back(std::static_pointer_cast<IComponent>(ptr));
}

template <typename T> 
bool ComponentManager::hasComponent(Entity entity) {		
	return hasComponent(entity, typeid(T).name());
}

template <typename T> 
std::shared_ptr<T> ComponentManager::getComponent(Entity entity) {
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

#pragma endregion

#pragma region System

template <typename... Tail>
bool System<Tail...>::hasComponents(Entity entity) {
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

template <typename... Tail>
System<Tail...>::System() {
	([&] {
		static_assert(std::is_base_of<IComponent, Tail>::value);
		types.push_back(typeid(Tail).name());
	} (), ...);
}

template <typename... Tail>
const char* System<Tail...>::type() {
	return typeid(*this).name();
}

template <typename... Tail>
void System<Tail...>::connectManager(std::shared_ptr<ComponentManager>& componentManager) {
	this->componentManager = componentManager;
}

#pragma endregion

#pragma region Scene

template <typename T> 
void Scene::connectComponent(Entity entity, T component) {
	componentManager->connectComponent<T>(entity, component);
}

template <typename T> 
bool Scene::hasComponent(Entity entity) {
	return componentManager->hasComponent<T>(entity);
}

template <typename T> 
std::shared_ptr<T> Scene::getComponent(Entity entity) {
	return componentManager->getComponent<T>(entity);
}

template <typename T>
void Scene::connectSystem() {
	static_assert(std::is_base_of<ISystem, T>::value);
	assert((!hasSystem<T>()) && "ComponentManager::connectSystem: Can't connect system. System already connected to this manager.");
	std::shared_ptr<T> system = std::make_shared<T>();
	system->connectManager(componentManager);
	systems.push_back(std::dynamic_pointer_cast<ISystem>(system));
}

template <typename T> 
bool Scene::hasSystem() {
	return getSystem<T>() != nullptr;
}

template <typename T> 
std::shared_ptr<T> Scene::getSystem() {
	for (const auto& system : systems) {
		if (system->type() == typeid(T).name()) {
			return std::dynamic_pointer_cast<T>(system);
		}
	}
	return nullptr;
}

#pragma endregion


#endif