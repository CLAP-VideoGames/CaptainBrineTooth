#pragma once
#include "ecs/Component.h"
class Enemy_Health : public Component
{
public:
	Enemy_Health(int health) : lifes(health) {};
	virtual ~Enemy_Health() {};
	void loseLife(int damage);


protected:
	int lifes;
};

