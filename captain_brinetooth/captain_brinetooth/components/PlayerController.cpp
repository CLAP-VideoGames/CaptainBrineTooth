#include "PlayerController.h"

#define _CRTDBG_MAP_ALLOC
#include<iostream>
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

void PlayerController::init()
{
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);

	collider_ = entity_->getComponent<BoxCollider>();
	assert(collider_ != nullptr);

	snd = entity_->getMngr()->getSoundMngr();
	assert(snd != nullptr);

	animController_ = entity_->getComponent<AnimBlendGraph>();
	assert(animController_ != nullptr);

	initEntityColliders();

	gravity = collider_->getBody()->GetGravityScale();

	lastTimeJumped = sdlutils().currRealTime();
	lasTimeDashed = sdlutils().currRealTime();

	moveLeft = moveRight = paralized = false;
	receiveInput = true;
	jumpkey_pressed = false;
}


void PlayerController::update()
{
#pragma region Input
	//Gestion del input
	assert(collider_ != nullptr);
	//death
	if (animController_->getParamValue("Dead") == 1)
		receiveInput = false;
	else
		receiveInput = true;

	if (receiveInput) {
		if (paralized) {
			timer = sdlutils().currRealTime();
			paralized = false;
			moveLeft = false;
			moveRight = false;
		}

		if (ih().keyUpEvent()) {
			//---Para la velocidad X
			if (ih().isKeyUp(SDL_SCANCODE_A)) {
				moveLeft = false;
			}
			if (ih().isKeyUp(SDL_SCANCODE_D)) {
				moveRight = false;
			}
			if (ih().isKeyUp(SDL_SCANCODE_SPACE)) {
				jumpkey_pressed = false;
			}
		}

		if (ih().keyDownEvent()) {
			if (ih().isKeyDown(SDL_SCANCODE_A)) {
				moveLeft = true;
			}

			if (ih().isKeyDown(SDL_SCANCODE_D)) {
				moveRight = true;
			}
		}

		//Parte Vertical
		if ((ih().isKeyDown(SDL_SCANCODE_SPACE) || ih().isKeyDown(SDL_CONTROLLER_BUTTON_A)) && isOnFloor && !isDashing && !jumpkey_pressed) {
			isOnFloor = false;
			//collider_->applyForce(Vector2D(0, -1), forceJump_ * 44.0f); Al ser gradual, le cuesta mucho mas
			collider_->applyLinearForce(Vector2D(0, -1), forceJump_);
			jumpkey_pressed = true;
			//Realizar da�o
			//health_->loseLife();
			snd->playSoundEffect("player_jump", 300);


		}
		//Para juego final añadir && canDash
		if (ih().isKeyDown(SDLK_LSHIFT) && canDash) {
			collider_->getBody()->SetGravityScale(0.0f);
			collider_->setSpeed(Vector2D(0, 0));
			isDashing = true;
			if (moveRight && !moveLeft) {
				animController_->flipX(true);
				collider_->applyLinearForce(Vector2D(1, 0), dashSpeed);
			}
			else if (moveLeft) {
				animController_->flipX(false);
				collider_->applyLinearForce(Vector2D(-1, 0), dashSpeed);
			}
			else {
				if (animController_->isFlipX())collider_->applyLinearForce(Vector2D(1, 0), dashSpeed);
				else collider_->applyLinearForce(Vector2D(-1, 0), dashSpeed);
			}
			collider_->changeColLayer_and_Mask(PLAYER_DASH, PLAYER_DASH_MASK);
			canDash = false;
			std::cout << "dash_pressed";
		}
	}
#pragma endregion
#pragma region States
	//isOnGround();
	//std::cout << "\n" << isOnFloor;
	//std::cout << "\n" << animController_->getParamValue("NotOnFloor");
	isOnGround();
	/*if (collider_->getBody()->GetLinearVelocity().y < 0.1 && collider_->getBody()->GetLinearVelocity().y > -0.1 && !isDashing) {
	}
	else
		isOnFloor = false;*/
#pragma endregion
#pragma region Animaciones
		//Esta tocando suelo
	if (isOnFloor) {
		animController_->setParamValue("NotOnFloor", 0);
	}
	else {
		animController_->setParamValue("NotOnFloor", 1);
	}
	//--No vel en x
	if ((moveLeft && moveRight) || (!moveLeft && !moveRight))
		animController_->setParamValue("Speed", 0);
	else
		animController_->setParamValue("Speed", 1);

	//dash aire o suelo
	if (isDashing) {
		snd->playSoundEffect("dash", 500);
		if (animController_->getCurrentAnimation()->getID() == "jump")
			animController_->setParamValue("Dash_Air", 1);
		else
			animController_->setParamValue("Dash_Ground", 1);
	}
	else {
		animController_->setParamValue("Dash_Air", 0);
		animController_->setParamValue("Dash_Ground", 0);
	}
#pragma endregion
#pragma region Movement
	//Ambas direcciones o ninguna
	if (!isDashing) {
		if ((moveLeft && moveRight) || (!moveLeft && !moveRight)) {
			collider_->setSpeed(Vector2D(0, collider_->getBody()->GetLinearVelocity().y));
		}
		else {
			if (moveLeft) {	//izqda
				b2Vec2 vel = collider_->getBody()->GetLinearVelocity();
				collider_->setSpeed(Vector2D(-speed_, vel.y));
				if (entity_->hasComponent<Inventory>()) {
					if (!entity_->getComponent<Inventory>()->playerAttacking()) {
						animController_->flipX(false);
					}
				}
				else {
					animController_->flipX(false);
				}
			}
			if (moveRight) {	//drcha
				b2Vec2 vel = collider_->getBody()->GetLinearVelocity();
				collider_->setSpeed(Vector2D(speed_, vel.y));
				if (entity_->hasComponent<Inventory>()) {
					if (!entity_->getComponent<Inventory>()->playerAttacking()) {
						animController_->flipX(true);
					}
				}
				else {
					animController_->flipX(true);
				}
			}
		}
	}
#pragma endregion
#pragma region Dash
	if (isDashing) {
		if (animController_->isComplete() && (animController_->getCurrentAnimation()->getID() == "dash_air" || animController_->getCurrentAnimation()->getID() == "dash_ground")) {
			b2Vec2 vel = collider_->getBody()->GetLinearVelocity();
			collider_->setSpeed(Vector2D(0, vel.y));
			collider_->getBody()->SetGravityScale(gravity);
			collider_->changeColLayer_and_Mask(PLAYER, PLAYER_MASK);
			isDashing = false;
		}
	}

	if (dashCoolDown + lasTimeDashed < sdlutils().currRealTime() && !canDash) {
		lasTimeDashed = sdlutils().currRealTime();
		canDash = true;
	}
	if (animController_->getParamValue("Dead") == 1)
		collider_->setSpeed(Vector2D(0, collider_->getBody()->GetLinearVelocity().y));
#pragma endregion
}

void PlayerController::initEntityColliders()
{
	if (entity_->getComponent<TriggerCollider>() != nullptr) {
		trigger_ = entity_->getComponent<TriggerCollider>();

		trigger_->getTriggerEntity()->setCollisionMethod(OnTriggerEnter);
		trigger_->getTriggerEntity()->setEndCollisionMethod(OnTriggerExit);
	}
}

void PlayerController::createJointTrigger(Entity* trigger)
{
	b2RevoluteJointDef* b2joint = new b2RevoluteJointDef();
	//Asignar a que cuerpos esta asociado el joint 
	b2joint->bodyA = entity_->getComponent<BoxCollider>()->getBody();
	b2joint->bodyB = trigger->getComponent<BoxCollider>()->getBody();
	//Si sus colisiones estan o no estan conectadas 
	b2joint->collideConnected = true;
	//No se del todo como van las anclas 
	b2joint->localAnchorA.Set(0, 0);
	//Mas o menos en lamitad de su anclaje 
	b2joint->localAnchorB.Set(0, 0);
	// Faltan los atributos -> Motor speed(Como de rapido va) , MaxmotorTorque (como de poderoso es)
	entity_->getWorld()->CreateJoint(b2joint);
}

void PlayerController::playerReceiveInput(bool state)
{
	receiveInput = state;
}

void PlayerController::OnTriggerEnter(b2Contact* contact)
{
	Entity* bodyA = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	if (bodyA != nullptr) {
		if (bodyA->hasComponent<BoxCollider>()) {
			if (bodyA->getComponent<BoxCollider>()->getColLayer() == PLAYER_JUMP) {
				Entity* bodyB = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
				if (bodyB->hasComponent<MapCollider>() || bodyB->hasComponent<BoxCollider>()) {
					uint16 bodyB_Layer = (bodyB->hasComponent<BoxCollider>()) ?
						bodyB->getComponent<BoxCollider>()->getColLayer() : bodyB->getComponent<MapCollider>()->getColLayer();

					if (bodyB_Layer == GROUND && !bodyA->getComponent<BoxCollider>()->isTriggerColliding()) {
						bodyA->getComponent<BoxCollider>()->triggerCollide(true);
					}
				}
			}
		}

		else {
			bodyA = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
			if (bodyA != nullptr) {
				if (bodyA->hasComponent<BoxCollider>()) {
					if (bodyA->getComponent<BoxCollider>()->getColLayer() == PLAYER_JUMP) {
						Entity* bodyB = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
						if (bodyB->hasComponent<MapCollider>() || bodyB->hasComponent<BoxCollider>()) {
							uint16 bodyB_Layer = (bodyB->hasComponent<BoxCollider>()) ?
								bodyB->getComponent<BoxCollider>()->getColLayer() : bodyB->getComponent<MapCollider>()->getColLayer();

							if (bodyB_Layer == GROUND && !bodyA->getComponent<BoxCollider>()->isTriggerColliding()) {
								bodyA->getComponent<BoxCollider>()->triggerCollide(true);
							}
						}
					}
				}
			}
		}
	}
}

void PlayerController::OnTriggerExit(b2Contact* contact)
{
	Entity* bodyA = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	if (bodyA != nullptr) {
		if (bodyA->hasComponent<BoxCollider>()) {
			if (bodyA->getComponent<BoxCollider>()->getColLayer() == PLAYER_JUMP) {
				Entity* bodyB = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
				if (bodyB->hasComponent<MapCollider>() || bodyB->hasComponent<BoxCollider>()) {
					uint16 bodyB_Layer = (bodyB->hasComponent<BoxCollider>()) ?
						bodyB->getComponent<BoxCollider>()->getColLayer() : bodyB->getComponent<MapCollider>()->getColLayer();

					if (bodyB_Layer == GROUND && bodyA->getComponent<BoxCollider>()->isTriggerColliding()) {
						bodyA->getComponent<BoxCollider>()->triggerCollide(false);
					}
				}
			}
		}

		else {
			bodyA = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
			if (bodyA != nullptr) {
				if (bodyA->hasComponent<BoxCollider>()) {
					if (bodyA->getComponent<BoxCollider>()->getColLayer() == PLAYER_JUMP) {
						Entity* bodyB = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
						if (bodyB->hasComponent<MapCollider>() || bodyB->hasComponent<BoxCollider>()) {
							uint16 bodyB_Layer = (bodyB->hasComponent<BoxCollider>()) ?
								bodyB->getComponent<BoxCollider>()->getColLayer() : bodyB->getComponent<MapCollider>()->getColLayer();

							if (bodyB_Layer == GROUND && bodyA->getComponent<BoxCollider>()->isTriggerColliding()) {
								bodyA->getComponent<BoxCollider>()->triggerCollide(false);
							}
						}
					}
				}
			}
		}
	}
}

void PlayerController::isOnGround()
{
	if ((trigger_->getTriggerEntity()->getComponent<BoxCollider>()->isTriggerColliding() || collider_->getBody()->GetLinearVelocity().y == 0) && !isDashing)
		isOnFloor = true;
	else
		isOnFloor = false;
}

const bool& PlayerController::isPlayerDashing()
{
	return isDashing;
}
void PlayerController::setMoveLeft(bool state) { moveLeft = state; }
void PlayerController::setMoveRight(bool state) { moveRight = state; }

