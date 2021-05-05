#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Manager.h"
#include "../game/CollisionLayers.h"
#include "Transform.h"
#include "EnemyTrigger.h"
#include "BoxCollider.h"
#include "MapCollider.h"
#include "AnimBlendGraph.h"
#include "ContactDamage.h"
class PompeyWormAttack :public Component
{
public:
	PompeyWormAttack(Entity* entity_Parent) : entity_Parent_(entity_Parent) {};
	virtual ~PompeyWormAttack() {};
	void init() override;
	void update() override; 
	static void hasEnter(b2Contact* contact);
	static void hasExit(b2Contact* contact);
	void entityInRange();
	void entityOutRange();
	void attack();
	void patrol();
	void createTriggerAttack();
	static void hit(b2Contact* contact);

private:
	Entity* entity_Parent_;
	Transform* entitytr_;
	Vector2D triggerSize_;
	//Atributos de ataque 
	bool entity_in_range_;
	float speed_ = 1.0f;
	float or_;
	float attack_anticipation_ = 0;	//offset del ataque que permite atacar al player un poco antes de que este a rango del ataque
	Vector2D attackSpriteSize_ = Vector2D(20.0, 20.0);
	Vector2D attackTriggerSize_ = Vector2D(20.0, 20.0);
	Vector2D puddleSize = Vector2D(300.0, 20);
	Entity* attackTrigger_;
	float attack_force = 2.5f;
	bool shot;
	//Cooldowns
	float cd_move_time = 100;
	float cd_attack_time = 5000;
	float cd_puddle_lifetime = 150;
	float move_time, elapsed_time_lastAttack, lifetime_puddle;
};