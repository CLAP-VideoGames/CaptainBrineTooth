#pragma once
#include "ecs/Component.h"
class Enemy_Health : public Component
{
public:
	Enemy_Health() {};
	virtual ~Enemy_Health() ;
	void init() override {lifes = 10;};
	void update() override {};
	void render() override {};
	void loseLife(int damage);


protected:
	int lifes;


};

