#ifndef SYSTEMS_MOVEMENT_HPP
#define SYSTEMS_MOVEMENT_HPP

#include "../lib/ecs.hpp"
#include "../components/transform.hpp"


class MovementSystem : public System<TransformComponent, VelocityComponent> {
public:
	using System::System;
	
	void init() override;
	void update(float dt) override;
};


#endif