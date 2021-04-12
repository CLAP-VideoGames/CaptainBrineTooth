#pragma once
#include "../ecs/Component.h"
class Enemy_Health : public Component
{
public:
	Enemy_Health(int health) : lifes(health) {};
	virtual ~Enemy_Health() {};
	void loseLife(int damage);
	void setHealth(int health) { lifes = health; };
	int getHealth() { return lifes; };


protected:
	int lifes;
};

