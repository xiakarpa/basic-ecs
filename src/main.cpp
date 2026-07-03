#include "lib/ecs.hpp"

int main() {
	Scene scene;
	
	scene.connectSystem<MovementSystem>();
	
	Entity entity = scene.createEntity();
	scene.connectComponent<TransformComponent>(entity, TransformComponent((Vec3){0,0,0}, (Vec3){0,0,0}, (Vec3){0,0,0}));
	scene.connectComponent<VelocityComponent>(entity, VelocityComponent((Vec3){1,2,3}, (Vec3){0,0,0}, (Vec3){0,0,0}));
	
	scene.initSystems();
	
	std::cout << scene.getComponent<TransformComponent>(entity)->position.x << " " 
		<< scene.getComponent<TransformComponent>(entity)->position.y << " " 
		<< scene.getComponent<TransformComponent>(entity)->position.z << std::endl; 
	
	scene.updateSystems(1.f);
	scene.updateSystems(1.f);
	
	std::cout << scene.getComponent<TransformComponent>(entity)->position.x << " " 
		<< scene.getComponent<TransformComponent>(entity)->position.y << " " 
		<< scene.getComponent<TransformComponent>(entity)->position.z << std::endl; 
	
	return 0;
}