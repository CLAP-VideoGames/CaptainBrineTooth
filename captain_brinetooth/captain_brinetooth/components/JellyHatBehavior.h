#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
#include "Transform.h"
#include "EnemyTrigger.h"
#include "Enemy_Health.h"
#include "AnimBlendGraph.h"
#include <cmath>

class JellyHatBehavior : public Component
{
public:
	JellyHatBehavior(Entity* entity_Parent) : entity_Parent_(entity_Parent) {};
	virtual ~JellyHatBehavior() {};
	void init() override;
	void update() override;
	static void collisionPlayer(b2Contact* contact);

private:
	Entity* entity_Parent_;
	Transform* entitytr_;
	Enemy_Health* entityhealth_;
	BoxCollider* entitycollider_;
	AnimBlendGraph* anims_;

	Transform* playertr_;
	float iniH;
	float iniW;
	float lastJump = 0;
	int iniHealth;
};


