#include "PompeyWormAttack.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Manager.h"
#include "../game/CollisionLayers.h"
#include "BoxCollider.h"
#include "AnimBlendGraph.h"
#include "AnimBlendGraph.h"
#include "ContactDamage.h"
using namespace ColLayers;


void PompeyWormAttack::init()
{
	entitytr_ = entity_Parent_->getComponent<Transform>();
	assert(entitytr_ != nullptr);
	triggerSize_ = Vector2D(entity_->getComponent<Transform>()->getW(), entity_->getComponent<Transform>()->getH());
	//Atributos ataque
	entity_in_range_ = false;
	//Crear componente triggerEnemy
	entity_->setCollisionMethod(hasEnter);
	entity_->setEndCollisionMethod(hasExit);
	attackTrigger_ = nullptr;
	shot = false;
	//Set de los timers
	move_time = cd_move_time;
	elapsed_time_lastAttack = 0;
}

void PompeyWormAttack::update() {
	//Actualiza la posicion del trigger
	//El player ha entrado
	if (entity_in_range_ && elapsed_time_lastAttack + cd_attack_time < sdlutils().currRealTime()) {
		attack();
		//Reset cooldown
		elapsed_time_lastAttack = sdlutils().currRealTime();
	}
	if (entity_Parent_->getComponent<AnimBlendGraph>()->isComplete() && shot) { 
		createTriggerAttack(); 
		shot = false; 
		entity_Parent_->getComponent<AnimBlendGraph>()->setParamValue("Attack", 0);
	}
	if(!shot) { patrol(); }
}

void PompeyWormAttack::hasEnter(b2Contact* contact) {
	Entity* body = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	if (body != nullptr) {
		if (body->getComponent<PompeyWormAttack>() != nullptr)
			body->getComponent<PompeyWormAttack>()->entityInRange();
		else {
			body = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
			if (body != nullptr)body->getComponent<PompeyWormAttack>()->entityInRange();
		}
	}
}

void PompeyWormAttack::hasExit(b2Contact* contact) {
	Entity* body = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	if (body != nullptr) {
		if (body->getComponent<PompeyWormAttack>() != nullptr)
			body->getComponent<PompeyWormAttack>()->entityOutRange();
		else {
			body = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
			if (body != nullptr)body->getComponent<PompeyWormAttack>()->entityOutRange();
		}
	}
}

void PompeyWormAttack::entityInRange() { entity_in_range_ = true; }
void PompeyWormAttack::entityOutRange() { entity_in_range_ = false; }

void PompeyWormAttack::attack() {
	//Player Transform
	Transform* playertr_ = entity_->getMngr()->getHandler<Player>()->getComponent<Transform>();
	b2Vec2 vel = entity_Parent_->getComponent<BoxCollider>()->getBody()->GetLinearVelocity();
	//no se mueve
	entity_Parent_->getComponent<BoxCollider>()->setSpeed((Vector2D(0, vel.y)));
	//Actualizar posicion
	entity_->getComponent<BoxCollider>()->setSpeed((Vector2D(0, vel.y)));
	//Orientacion del enemigo
	if (playertr_->getPos().getX() < entitytr_->getPos().getX()) {
		//Se mueve hacia la izquierda
		entity_Parent_->getComponent<AnimBlendGraph>()->flipX(true);
	}
	else {
		//Se mueve hacia la derecha
		entity_Parent_->getComponent<AnimBlendGraph>()->flipX(false);
	}
	//Llama al cambio de estado de animacion
	entity_Parent_->getComponent<AnimBlendGraph>()->setParamValue("Attack", 1);
	shot = true;
}

void PompeyWormAttack::patrol() {
	//Player Transform
	b2Vec2 vel = entity_Parent_->getComponent<BoxCollider>()->getBody()->GetLinearVelocity();
	if (move_time >= cd_move_time) {
		if (!entity_Parent_->getComponent<AnimBlendGraph>()->isFlipX()) {
			or_ = -1;
			entity_Parent_->getComponent<AnimBlendGraph>()->flipX(true);
		}
		else {
			or_ = 1;
			entity_Parent_->getComponent<AnimBlendGraph>()->flipX(false);
		}
		//Se mueve hacia la izquierda
		entity_Parent_->getComponent<BoxCollider>()->setSpeed((Vector2D(or_* speed_, vel.y)));
		//Actualizar posicion
		entity_->getComponent<BoxCollider>()->setSpeed((Vector2D(or_*speed_, vel.y)));
		move_time = 0;
	}
	else {
		bool flip_sprite = (or_ > 0)? false:true;
		entity_Parent_->getComponent<AnimBlendGraph>()->flipX(flip_sprite);
		//Se mueve hacia la izquierda
		entity_Parent_->getComponent<BoxCollider>()->setSpeed((Vector2D(or_ * speed_, vel.y)));
		//Actualizar posicion
		entity_->getComponent<BoxCollider>()->setSpeed((Vector2D(or_ * speed_, vel.y)));
	}
	move_time++;
}

void PompeyWormAttack::createTriggerAttack()
{
	Transform* playertr_ = entity_->getMngr()->getHandler<Player>()->getComponent<Transform>();
	//Crea trigger de ataque
	attackTrigger_ = entity_->getMngr()->addEntity();
	attackTrigger_->addComponent<Transform>(Vector2D(entitytr_->getPos().getX(), entitytr_->getPos().getY()),
		Vector2D(0, 0), attackTriggerSize_.getX(), attackTriggerSize_.getY(), 0.0f);
	attackTrigger_->addComponent<Animation>("pompey", &sdlutils().images().at("pompey_worm_spit"), 1, 1, 1, 1, 0);
	attackTrigger_->addComponent<Animation>("debug", &sdlutils().images().at("debug_square"), 1, 1, 1, 1, 0);
	attackTrigger_->addComponent<BoxCollider>(DYNAMIC, ENEMY_ATTACK, ENEMY_ATTACK_MASK, true);
	//Posicion del player
	Vector2D attackdir = (playertr_->getPos() - entitytr_->getPos());
	attackdir = attackdir.normalize();
	attackTrigger_->getComponent<BoxCollider>()->applyForce(attackdir, attack_force);
	//attackTrigger_->addComponent<ContactDamage>();
}
