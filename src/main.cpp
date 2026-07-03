#include "lib/ecs.hpp"
#include "systems/movement.hpp"

#include <iostream>


int main() {
	Scene scene;
	
	scene.connectSystem<MovementSystem>();
	
	Entity entity = scene.createEntity();
	scene.connectComponent<TransformComponent>(
		entity, 
		TransformComponent(
			(Vec3){0,0,0}, 
			(Vec3){0,0,0}, 
			(Vec3){0,0,0}
		)
	);
	scene.connectComponent<VelocityComponent>(
		entity, 
		VelocityComponent(
			(Vec3){1,2,3}, 
			(Vec3){0,0,0}, 
			(Vec3){0,0,0}
		)
	);
	
	scene.initSystems();
	
	Vec3 pos = scene.getComponent<TransformComponent>(entity)->position;
	std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl; 
	
	scene.updateSystems(1.f);
	scene.updateSystems(1.f);
	
	pos = scene.getComponent<TransformComponent>(entity)->position;
	std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl; 
	
	return 0;
}