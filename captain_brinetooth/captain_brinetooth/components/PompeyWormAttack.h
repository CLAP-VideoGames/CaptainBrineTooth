#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
#include "Transform.h"
#include "EnemyTrigger.h"
class PompeyWormAttack :public Component
{
public:
	PompeyWormAttack(Entity* entity_Parent) : entity_Parent_(entity_Parent) {};
	virtual ~PompeyWormAttack() {};
	void init() override;
	void update() override; 
	bool canAttack();
	static void hasEnter(b2Contact* contact);
	static void hasExit(b2Contact* contact);
	void entityInRange();
	void entityOutRange();
	void attack();
	void patrol();
	void createTriggerAttack();

private:
	Entity* entity_Parent_;
	Transform* entitytr_;
	Vector2D triggerSize_;
	//Atributos de ataque 
	bool entity_in_range_;
	float speed_ = 1.0f;
	float or_;
	float attack_anticipation_ = 0;	//offset del ataque que permite atacar al player un poco antes de que este a rango del ataque
	Vector2D attackTriggerSize_ = Vector2D(50.0f, 50.0f);
	Entity* attackTrigger_;
	float attack_force = 25.0f;
	bool shot;
	//Cooldown
	float cd_move_time = 100;
	float cd_attack_time = 2000;
	float move_time, elapsed_time_lastAttack;
};