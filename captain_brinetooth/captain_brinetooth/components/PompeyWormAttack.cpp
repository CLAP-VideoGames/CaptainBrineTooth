#include "PompeyWormAttack.h"
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
	lifetime_puddle = 0;
}

void PompeyWormAttack::update() {
	//Charco de veneno

	if (attackTrigger_ != nullptr) {
		if (attackTrigger_->isActive() && attackTrigger_->hasComponent<BoxCollider>()) {
			if (attackTrigger_->getComponent<BoxCollider>()->isTriggerColliding()) {
				if (lifetime_puddle >= cd_puddle_lifetime) {
					attackTrigger_->getComponent<BoxCollider>()->triggerCollide(false);
					attackTrigger_->setActive(false);
					attackTrigger_ = nullptr;
					lifetime_puddle = 0;
				}
				else {
					attackTrigger_->getComponent<BoxCollider>()->Resize(Vector2D(300.0, 20.0));	//Obligatorio llamarlo en clases no estaticas
					lifetime_puddle++;
				}
			}
		}
		else if (!attackTrigger_->isActive()) {
			attackTrigger_ = nullptr;
		}
	}
	//Actualiza la posicion del trigger
	//El player ha entrado
	if (entity_in_range_ && elapsed_time_lastAttack + cd_attack_time < sdlutils().currRealTime() && attackTrigger_ == nullptr) {
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
	entity_Parent_->getMngr()->getSoundMngr()->playSoundEffect("disparo_fringehead", 0);
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
		Vector2D(0, 0), attackSpriteSize_.getX(), attackSpriteSize_.getY(), 0.0f);
	Animation* sprite = attackTrigger_->addComponent<Animation>("pompey", &sdlutils().images().at("pompey_worm_spit"), 1, 1, 1, 1, 0);
	sprite->flipX(true);
	attackTrigger_->addComponent<BoxCollider>(DYNAMIC, ENEMY_ATTACK, ENEMY_ATTACK_MASK, true, 0.0f, true,0.0f, attackTriggerSize_);
	//Posicion del player
	Vector2D attackdir = (playertr_->getPos() - entitytr_->getPos());
	attackdir = attackdir.normalize();
	attackTrigger_->getComponent<BoxCollider>()->applyForce(attackdir, attack_force);
	attackTrigger_->addComponent<ContactDamage>();
	attackTrigger_->setCollisionMethod(hit);
}

void PompeyWormAttack::hit(b2Contact* contact)
{
	Entity* bodyA = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	if (bodyA != nullptr) {
		if (bodyA->hasComponent<ContactDamage>()) {
			Entity* bodyB = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
			//Colision con el player
			if (bodyB->getComponent<BoxCollider>()->getColLayer() == PLAYER) {
				bodyA->getComponent<ContactDamage>()->makeDamage();
				if (!bodyA->getComponent<BoxCollider>()->isTriggerColliding()) {
					//Desactiva el trigger
					bodyA->setActive(false);
				}
			}
			//Colision con el Mapa
			else if (bodyB->hasComponent<BoxCollider>() || bodyB->hasComponent<MapCollider>()) {
				uint16 bodyB_Layer = (bodyB->hasComponent<BoxCollider>()) ?
					bodyB->getComponent<BoxCollider>()->getColLayer() : bodyB->getComponent<MapCollider>()->getColLayer();
				if (bodyB_Layer == GROUND && !bodyA->getComponent<BoxCollider>()->isTriggerColliding()) {
					auto* collider = bodyA->getComponent<BoxCollider>();
					collider->setSpeed(Vector2D(0.0, 0.0));
					collider->getBody()->SetType(b2BodyType::b2_kinematicBody);
					collider->triggerCollide(true);
				}
			}
		}
		else {
			bodyA = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
			if (bodyA != nullptr) {
				if (bodyA->hasComponent<ContactDamage>()) {
					Entity* bodyB = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
					//Colision con el player
					if (bodyB->getComponent<BoxCollider>()->getColLayer() == PLAYER) {
						bodyA->getComponent<ContactDamage>()->makeDamage();
						if (!bodyA->getComponent<BoxCollider>()->isTriggerColliding()) {
							//Desactiva el trigger
							bodyA->setActive(false);
						}
					}
					//Colision con el Mapa
					else if (bodyB->hasComponent<BoxCollider>()|| bodyB->hasComponent<MapCollider>()) {
						uint16 bodyB_Layer = (bodyB->hasComponent<BoxCollider>()) ?
							bodyB->getComponent<BoxCollider>()->getColLayer() : bodyB->getComponent<MapCollider>()->getColLayer();
						if (bodyB_Layer == GROUND && !bodyA->getComponent<BoxCollider>()->isTriggerColliding()) {
							auto* collider = bodyA->getComponent<BoxCollider>();
							collider->setSpeed(Vector2D(0.0, 0.0));
							collider->getBody()->SetType(b2BodyType::b2_kinematicBody); 
							collider->triggerCollide(true);
						}
					}
				}
			}
		}
	}
}

