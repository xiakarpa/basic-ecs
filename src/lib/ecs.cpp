#include "ecs.hpp"

#include <memory>
#include <algorithm>
#include <cassert>


const char* IComponent::type() {
	return typeid(*this).name();
}

#pragma region ComponentManager

bool ComponentManager::hasEntity(Entity entity) {
	return std::find(entities.begin(), entities.end(), entity) != entities.end();
}

Entity ComponentManager::createEntity() {
	assert((!hasEntity(nextId)) && "ComponentManager::createEntity: Can't create Entity. This id is already in use.");
	Entity entity = nextId;
	entities.push_back(entity);
	nextId++;
	return entity;
}
	
void ComponentManager::removeEntity(Entity entity) {
	auto it = std::find(entities.begin(), entities.end(), entity);
	if (it != entities.end()) {
		entities.erase(it);
		components.erase(components.find(entity));
	}
}

bool ComponentManager::hasComponent(Entity entity, const char* type) {
	if (!hasEntity(entity)) {
		return false;
	}
	
	for (const auto& component : components[entity]) {
		if (component->type() == type) {
			return true;
		}
	}
	return false;
}

#pragma endregion

#pragma region Scene

Scene::Scene() {
	componentManager = std::make_shared<ComponentManager>();
}

Entity Scene::createEntity() {
	return componentManager->createEntity();
}

void Scene::removeEntity(Entity entity) {
	componentManager->removeEntity(entity);
}

bool Scene::hasEntity(Entity entity) {
	return componentManager->hasEntity(entity);
}

bool Scene::hasComponent(Entity entity, const char* type) {
	return componentManager->hasComponent(entity, type);
}

void Scene::initSystems() {
	for (const auto& system : systems) {
		system->init();
	}
}

void Scene::updateSystems(float dt) {
	for (const auto& system : systems) {
		system->update(dt);
	}
}

#pragma endregion
