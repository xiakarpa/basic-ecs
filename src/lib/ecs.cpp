#include "ecs.hpp"


#include <cstdint>
#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <cassert>





const char* IComponent::type() {
	return typeid(*this).name();
}


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





void MovementSystem::init() {
	std::cout << "MovementSystem::init [" << types[0] << ", " << types[1] << "]" << std::endl;
}

void MovementSystem::update(float dt) {
	for (auto& entity : componentManager->entities) {
		if (hasComponents(entity)) {
			std::shared_ptr<TransformComponent> transform = componentManager->getComponent<TransformComponent>(entity);
			std::shared_ptr<VelocityComponent> velocity = componentManager->getComponent<VelocityComponent>(entity);
			
			transform->position.x += velocity->position.x * dt;
			transform->position.y += velocity->position.y * dt;
			transform->position.z += velocity->position.z * dt;
		}
	}
}