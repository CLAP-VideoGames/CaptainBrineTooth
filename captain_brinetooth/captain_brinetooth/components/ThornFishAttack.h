#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
#include "Transform.h"
#include "EnemyTrigger.h"
#include "Enemy_Health.h"

class ThornFishAttack : public Component {
public:
	ThornFishAttack(){};
	virtual ~ThornFishAttack();
	void init() override;
	void update() override;
	void canAttack();
	void createAttackTrigger();
	void attack();

private:
	Entity* entity_Parent_;
	Transform* entitytr_;
	Vector2D triggerSize_;
	//Atributos de ataque 
	bool attack_player;
	Vector2D attackTriggerSize_;
	Entity* attackTrigger_;
	//Cooldown
	float cd_attack_time;
	float elapsed_time_lastAttack;
};