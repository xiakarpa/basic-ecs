#include "movement.hpp"

#include <iostream>


void MovementSystem::init() {
	std::cout << "MovementSystem::init [" << types[0] << ", " << types[1] << "]" << std::endl;
}

void MovementSystem::update(float dt) {
	for (auto& entity : componentManager->entities) {
		if (hasComponents(entity)) {
			Vec3& pos = componentManager->getComponent<TransformComponent>(entity)->position;
			Vec3 velPos = componentManager->getComponent<VelocityComponent>(entity)->position;
			
			pos.x += velPos.x * dt;
			pos.y += velPos.y * dt;
			pos.z += velPos.z * dt;
		}
	}
}