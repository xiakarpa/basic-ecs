#ifndef COMPONENTS_HEALTH_HPP
#define COMPONENTS_HEALTH_HPP

#include "../lib/ecs.hpp"


class HealthComponent : public IComponent {
public:
	std::uint32_t hp;
	std::uint32_t maxHp;
	
	HealthComponent(std::uint32_t hp, std::uint32_t maxHp) : hp(hp), maxHp(maxHp) {}
};


#endif