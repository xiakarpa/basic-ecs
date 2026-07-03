#ifndef COMPONENTS_TRANSFORM_HPP
#define COMPONENTS_TRANSFORM_HPP

#include "../lib/ecs.hpp"


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


#endif