#include "lib/ecs.hpp"
#include "systems/movement.hpp"
#include "lua_binding.hpp"

#include <iostream>

#include <sol/sol.hpp>


int main() {
	Scene scene;
	lua_binding::init(scene);
	
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
	std::cout << "before update:\t" << pos.x << " " << pos.y << " " << pos.z << std::endl; 
	
	scene.updateSystems(1.f);
	scene.updateSystems(1.f);
	
	pos = scene.getComponent<TransformComponent>(entity)->position;
	std::cout << "after update:\t" << pos.x << " " << pos.y << " " << pos.z << std::endl; 
	
	lua_binding::entityUpdate(entity);
	
	pos = scene.getComponent<TransformComponent>(entity)->position;
	std::cout << "after script:\t" << pos.x << " " << pos.y << " " << pos.z << std::endl; 
	
	return 0;
}