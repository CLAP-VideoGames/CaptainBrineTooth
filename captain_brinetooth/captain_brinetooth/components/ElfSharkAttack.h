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
	//Atributos de ataque 
	bool entity_in_range_;
	float speed_ = 1.0f;
	bool attack_player;
	float attack_anticipation_ = 0;	//offset del ataque que permite atacar al player un poco antes de que este a rango del ataque
	Vector2D attackTriggerSize_ = Vector2D(180.0f, 90.0f);
	Entity* attackTrigger_;
	//Cooldown
	float cd_attack_time = 2000;
	float elapsed_time_lastAttack;
};

