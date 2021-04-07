#include "ElfSharkAttack.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Manager.h"
#include "../game/CollisionLayers.h"
#include "BoxCollider.h"
#include "AnimBlendGraph.h"
#include "AnimBlendGraph.h"
#include "ContactDamage.h"
using namespace ColLayers;


void ElfSharkAttack::init()
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
	//Set de los timers
	elapsed_time_lastAttack = 0;
}

void ElfSharkAttack::update() {
	//Actualiza la posicion del trigger
	//Joseda por favor dime que hacer aqui que no se pone donde le digo
	//El player ha entrado
	if (entity_in_range_) {
		//A rango de ataque
		if (canAttack()) {
			if (attackTrigger_ == nullptr) {
				if (elapsed_time_lastAttack + cd_attack_time < sdlutils().currRealTime()) {
					//Ataque
					attack();
					//Reset cooldown
					elapsed_time_lastAttack = sdlutils().currRealTime();
				}
			}
			else {
				if (entity_Parent_->getComponent<AnimBlendGraph>()->isComplete()) {
					//Desactiva el trigger
					attackTrigger_->setActive(false);
					attackTrigger_ = nullptr;
					//Llama al cambio de estado de animacion
					entity_Parent_->getComponent<AnimBlendGraph>()->setParamValue("Attack", 0);
				}
			}
		}
		else { move(); }
	}
	else{ 
		//Desactiva el trigger del ataque al acabar la animacion
		if (attackTrigger_ != nullptr) {
			if (entity_Parent_->getComponent<AnimBlendGraph>()->isComplete()) {
				//Desactiva el trigger
				attackTrigger_->setActive(false);
				attackTrigger_ = nullptr;
				//Llama al cambio de estado de animacion
				entity_Parent_->getComponent<AnimBlendGraph>()->setParamValue("Attack", 0);
			}
		}
		entity_Parent_->getComponent<BoxCollider>()->setSpeed((Vector2D(0,0)));
		//Actualizar posicion
		entity_->getComponent<BoxCollider>()->setSpeed((Vector2D(0, 0)));
	}
}

bool ElfSharkAttack::canAttack() {
	//Player Transform
	Transform* playertr_ = entity_->getMngr()->getHandler<Player>()->getComponent<Transform>();
	float xRange =  attackTriggerSize_.getX() + attack_anticipation_; //rango X del ataque
	if (playertr_->getPos().getX() + playertr_->getW() > entitytr_->getPos().getX() - xRange &&
		playertr_->getPos().getX() < entitytr_->getPos().getX() + xRange + entitytr_->getW())	//Si esta a rango de ataque
		return true;
	else
		return false;
}

void ElfSharkAttack::hasEnter(b2Contact* contact) {
	Entity* body = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;

	ElfSharkAttack* elfAtt_;
	if (body != nullptr){
		elfAtt_ = body->getComponent<ElfSharkAttack>();
		if (elfAtt_ != nullptr) elfAtt_->entityInRange();
	}
}

void ElfSharkAttack::hasExit(b2Contact* contact) {
	Entity* body = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;

	ElfSharkAttack* elfAtt_;
	if (body != nullptr) {
		elfAtt_ = body->getComponent<ElfSharkAttack>();
		if (elfAtt_ != nullptr) elfAtt_->entityOutRange();
	}
}

void ElfSharkAttack::entityInRange() { entity_in_range_ = true; }
void ElfSharkAttack::entityOutRange() { entity_in_range_ = false; }

void ElfSharkAttack::attack() {
	//Player Transform
	Transform* playertr_ = entity_->getMngr()->getHandler<Player>()->getComponent<Transform>();
	//no se mueve
	entity_Parent_->getComponent<BoxCollider>()->setSpeed((Vector2D(0,0)));
	//Actualizar posicion
	entity_->getComponent<BoxCollider>()->setSpeed((Vector2D(0,0)));
	//Offset
	float offsetY = (entitytr_->getH() - attackTriggerSize_.getY()) * 0.5f;
	float offsetX;
	if (playertr_->getPos().getX() + playertr_->getW()*0.5f < entitytr_->getPos().getX() + entitytr_->getW()*0.5f) {	//izqda
		offsetX = -attackTriggerSize_.getX()*0.5f;
		entity_Parent_->getComponent<AnimBlendGraph>()->flipX(true);
	}
	else{	//drcha
		offsetX = attackTriggerSize_.getX();
		entity_Parent_->getComponent<AnimBlendGraph>()->flipX(false);
	}
	//Crea trigger de ataque
	attackTrigger_ = entity_->getMngr()->addEntity();
	attackTrigger_->addComponent<Transform>(Vector2D(entitytr_->getPos().getX() + offsetX, entitytr_->getPos().getY() + offsetY),
		Vector2D(0, 0), attackTriggerSize_.getX(), attackTriggerSize_.getY(), 0.0f);
	attackTrigger_->addComponent<BoxCollider>(KINEMATIC, ENEMY_ATTACK, ENEMY_ATTACK_MASK,true);
	attackTrigger_->addComponent<Animation>("1", &sdlutils().images().at("Square"), 1, 1, 1, 1, 0);
	attackTrigger_->addComponent<ContactDamage>();
	//Llama al cambio de estado de animacion
	entity_Parent_->getComponent<AnimBlendGraph>()->setParamValue("Attack", 1);
}

void ElfSharkAttack::move() {
	//Player Transform
	Transform* playertr_ = entity_->getMngr()->getHandler<Player>()->getComponent<Transform>();
	b2Vec2 vel = entity_Parent_->getComponent<BoxCollider>()->getBody()->GetLinearVelocity();
	if (playertr_->getPos().getX() < entitytr_->getPos().getX() - attackTriggerSize_.getX()) {
		//Se mueve hacia la izquierda
		entity_Parent_->getComponent<BoxCollider>()->setSpeed((Vector2D(-speed_, vel.y)));
		entity_Parent_->getComponent<AnimBlendGraph>()->flipX(true);
		//Actualizar posicion
		entity_->getComponent<BoxCollider>()->setSpeed((Vector2D(-speed_, vel.y)));
	}
	else {
		//Se mueve hacia la derecha
		entity_Parent_->getComponent<BoxCollider>()->setSpeed((Vector2D(speed_, vel.y)));
		entity_Parent_->getComponent<AnimBlendGraph>()->flipX(false);
		//Actualizar posicion
		entity_->getComponent<BoxCollider>()->setSpeed((Vector2D(speed_, vel.y)));
	}
}
