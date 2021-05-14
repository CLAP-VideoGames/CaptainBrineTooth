#include "ElfSharkAttack.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Manager.h"
#include "../game/CollisionLayers.h"
#include "BoxCollider.h"
#include "AnimBlendGraph.h"
#include "AnimBlendGraph.h"
#include "ContactDamage.h"
using namespace ColLayers;

ElfSharkAttack::~ElfSharkAttack() {
	if (attackTrigger_ != nullptr)
		attackTrigger_->setActive(false);
	if (entity_Parent_ != nullptr)
		entity_Parent_->setActive(false);
}

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
	attack_player = false;
	//Set de los timers
	elapsed_time_lastAttack = 0;
	colliderSize_Parent_ = entity_Parent_->getComponent<BoxCollider>()->getSize();
}

void ElfSharkAttack::update() {
	//Actualiza la posicion del trigger
	//Joseda por favor dime que hacer aqui que no se pone donde le digo
	//El player ha entrado
	if (entity_Parent_->getComponent<Enemy_Health>()->getHealth() > 0) {
		if (entity_in_range_) {
			//A rango de ataque
			if (canAttack()) {
				if (attackTrigger_ == nullptr) {
					if (elapsed_time_lastAttack + cd_attack_time < sdlutils().currRealTime()) {
						//Ataque
						attack();
					}
				}
				else {
					std::string tag = entity_Parent_->getComponent<AnimBlendGraph>()->getCurrentAnimation()->getID();
					if (!attack_player && tag == "attack_end" && entity_Parent_->getComponent<AnimBlendGraph>()->getParamValue("Attack") == 2) {
						//Desactiva el trigger
						attackTrigger_->setActive(false);
						attackTrigger_ = nullptr;
						//Llama al cambio de estado de animacion
						entity_Parent_->getComponent<AnimBlendGraph>()->setParamValue("Attack", 0);
					}
				}
			}
			else {
				entity_Parent_->getComponent<BoxCollider>()->Resize(Vector2D(colliderSize_Parent_.getX(), colliderSize_Parent_.getY()));
				move();
			}
		}
		else {
			//Para el enemigo
			entity_Parent_->getComponent<AnimBlendGraph>()->setParamValue("Speed", 0);
			entity_Parent_->getComponent<BoxCollider>()->setSpeed((Vector2D(0, entity_Parent_->getComponent<BoxCollider>()->getBody()->GetLinearVelocity().y)));
			//Actualizar posicion
			entity_->getComponent<BoxCollider>()->setSpeed((Vector2D(0, entity_Parent_->getComponent<BoxCollider>()->getBody()->GetLinearVelocity().y)));
		}
		//Ataque
		if (entity_Parent_->getComponent<AnimBlendGraph>()->getCurrentAnimation()->getID() == "attack_ini" && entity_Parent_->getComponent<AnimBlendGraph>()->isComplete() && attack_player) {
			//Llama al cambio de estado de animacion
			entity_Parent_->getComponent<AnimBlendGraph>()->setParamValue("Attack", 2);
			createAttackTrigger();
			attack_player = false;
			//Reset cooldown
			elapsed_time_lastAttack = sdlutils().currRealTime();
		}
		if (attackTrigger_ != nullptr) {
			if (entity_Parent_->getComponent<AnimBlendGraph>()->getCurrentAnimation()->getID() == "attack_end") {
				//Desactiva el trigger
				attackTrigger_->setActive(false);
				attackTrigger_ = nullptr;
				//Llama al cambio de estado de animacion
				entity_Parent_->getComponent<AnimBlendGraph>()->setParamValue("Attack", 0);
			}
			else {
				attackTrigger_->getComponent<BoxCollider>()
					->setSpeed(Vector2D(entity_->getComponent<BoxCollider>()->getBody()->GetLinearVelocity().x,
						entity_->getComponent<BoxCollider>()->getBody()->GetLinearVelocity().y));
			}
		}
	}
}

bool ElfSharkAttack::canAttack() {
	//Player Transform
	Transform* playertr_ = entity_->getMngr()->getHandler<Player>()->getComponent<Transform>();
	Vector2D tiggerSize_ = Vector2D(entitytr_->getW() * attackTriggerSize_.getX(), entitytr_->getH() * attackTriggerSize_.getY());
	float attack_anticipation_ = 0;
	float xRange = tiggerSize_.getX()*0.8 + attack_anticipation_; 
	//rango X del ataque
	if (playertr_->getPos().getX() + playertr_->getW() > entitytr_->getPos().getX() - xRange &&
		playertr_->getPos().getX() < entitytr_->getPos().getX() - xRange*0.5 + entitytr_->getW())	//Si esta a rango de ataque
		return true;
	else
		return false;
}

void ElfSharkAttack::createAttackTrigger()
{

	Vector2D tiggerSize_ = Vector2D(entitytr_->getW() * attackTriggerSize_.getX(), entitytr_->getH() * attackTriggerSize_.getY());
	Transform* playertr_ = entity_->getMngr()->getHandler<Player>()->getComponent<Transform>();
	//Offset
	float offsetY = (entitytr_->getH() - tiggerSize_.getY()) * 0.5f;
	float offsetX = tiggerSize_.getX();
	if (playertr_->getPos().getX() + playertr_->getW() * 0.5f < entitytr_->getPos().getX() + entitytr_->getW() * 0.5f)	//izqda
		offsetX = -offsetX;

	//Crea trigger de ataque
	attackTrigger_ = entity_->getMngr()->addEntity();
	attackTrigger_->addComponent<Transform>(Vector2D(entitytr_->getPos().getX() + offsetX, entitytr_->getPos().getY() + offsetY),
		Vector2D(0, 0), tiggerSize_.getX(), tiggerSize_.getY(), 0.0f);
	attackTrigger_->addComponent<BoxCollider>(KINEMATIC, ENEMY_ATTACK, ENEMY_ATTACK_MASK, true);
	attackTrigger_->addComponent<ContactDamage>();
	// Reproducimos el sonido de mordisco
	entity_->getMngr()->getSoundMngr()->playSoundEffect("mordisco_elfshark", 300);
}

void ElfSharkAttack::hasEnter(b2Contact* contact) {
	Entity* body = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	if (body != nullptr) {
		if (body->hasComponent<ElfSharkAttack>())
			body->getComponent<ElfSharkAttack>()->entityInRange();
		else {
			body = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
			if (body != nullptr)body->getComponent<ElfSharkAttack>()->entityInRange();
		}
	}
}

void ElfSharkAttack::hasExit(b2Contact* contact) {
	Entity* body = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	if (body != nullptr) {
		if (body->hasComponent<ElfSharkAttack>())
			body->getComponent<ElfSharkAttack>()->entityOutRange();
		else {
			body = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
			if (body != nullptr)body->getComponent<ElfSharkAttack>()->entityOutRange();
		}
	}
}

void ElfSharkAttack::entityInRange() { entity_in_range_ = true; }
void ElfSharkAttack::entityOutRange() { entity_in_range_ = false; }

void ElfSharkAttack::attack() {
	entity_Parent_->getComponent<BoxCollider>()->Resize(Vector2D(colliderSize_Parent_.getX() * 0.5, colliderSize_Parent_.getY()));
	//Player Transform
	Transform* playertr_ = entity_->getMngr()->getHandler<Player>()->getComponent<Transform>();
	//No se mueve
	entity_Parent_->getComponent<BoxCollider>()->setSpeed((Vector2D(0, 0)));
	//Actualizar posicion
	entity_->getComponent<BoxCollider>()->setSpeed((Vector2D(0, 0)));

	if (playertr_->getPos().getX() + playertr_->getW() * 0.5f < entitytr_->getPos().getX() + entitytr_->getW() * 0.5f)	//izqda
		entity_Parent_->getComponent<AnimBlendGraph>()->flipX(false);
	else
		entity_Parent_->getComponent<AnimBlendGraph>()->flipX(true);

	//Llama al cambio de estado de animacion
	entity_Parent_->getComponent<AnimBlendGraph>()->setParamValue("Attack", 1);
	attack_player = true;
}

void ElfSharkAttack::move() {
	//Player Transform
	Transform* playertr_ = entity_->getMngr()->getHandler<Player>()->getComponent<Transform>();
	b2Vec2 vel = entity_Parent_->getComponent<BoxCollider>()->getBody()->GetLinearVelocity();
	if (playertr_->getPos().getX() < entitytr_->getPos().getX() - attackTriggerSize_.getX()) {
		//Se mueve hacia la izquierda
		entity_Parent_->getComponent<BoxCollider>()->setSpeed((Vector2D(-speed_, vel.y)));
		entity_Parent_->getComponent<AnimBlendGraph>()->flipX(false);
		//Actualizar posicion
		entity_->getComponent<BoxCollider>()->setSpeed((Vector2D(-speed_, vel.y)));
	}
	else {
		//Se mueve hacia la derecha
		entity_Parent_->getComponent<BoxCollider>()->setSpeed((Vector2D(speed_, vel.y)));
		entity_Parent_->getComponent<AnimBlendGraph>()->flipX(true);
		//Actualizar posicion
		entity_->getComponent<BoxCollider>()->setSpeed((Vector2D(speed_, vel.y)));
	}
	//Cancela animacion de ataque
	entity_Parent_->getComponent<AnimBlendGraph>()->setParamValue("Attack", 0);
	entity_Parent_->getComponent<AnimBlendGraph>()->setParamValue("Speed", 1);
}
