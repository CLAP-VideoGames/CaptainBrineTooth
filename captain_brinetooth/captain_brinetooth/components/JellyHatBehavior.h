#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
#include "Transform.h"
#include "EnemyTrigger.h"
#include "Enemy_Health.h"
#include <cmath>

class JellyHatBehavior : public Component
{
public:
	JellyHatBehavior(Entity* entity_Parent) : entity_Parent_(entity_Parent) {};
	virtual ~JellyHatBehavior() {};
	void init() override;
	void update() override;
	/*bool canAttack();
	static void hasEnter(b2Contact* contact);
	static void hasExit(b2Contact* contact);
	void entityInRange();
	void entityOutRange();
	void attack();
	void move();*/

private:
	Entity* entity_Parent_;
	Transform* entitytr_;
	Enemy_Health* entityhealth_;

	Transform* playertr_;
	float iniH;
	float iniW;
	int iniHealth;
};


