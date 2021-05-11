#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
#include "Transform.h"
#include "EnemyTrigger.h"

class ElfSharkAttack: public Component
{
public:
	ElfSharkAttack(Entity* entity_Parent) : entity_Parent_(entity_Parent){};
	virtual ~ElfSharkAttack();
	void init() override;
	void update() override;
	bool canAttack();
	void createAttackTrigger();
	static void hasEnter(b2Contact* contact);
	static void hasExit(b2Contact* contact);
	void entityInRange();
	void entityOutRange();
	void attack();
	void move();

private:
	Entity* entity_Parent_;
	Transform* entitytr_;
	Vector2D triggerSize_;
	Vector2D colliderSize_Parent_;
	//Atributos de ataque 
	bool entity_in_range_;
	float speed_ = 1.0f;
	bool attack_player;
	Vector2D attackTriggerSize_ = Vector2D(0.5, 0.75);
	Entity* attackTrigger_;
	//Cooldown
	float cd_attack_time = 2000;
	float elapsed_time_lastAttack;
};

