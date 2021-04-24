#include "PlayerController.h"

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

	lastTimeJumped = sdlutils().currRealTime();
	lasTimeDashed = sdlutils().currRealTime();
}


void PlayerController::update()
{
#pragma region States
	isOnGround();
	std::cout << "\n" << isOnFloor;
	//std::cout << "\n" << animController_->getParamValue("NotOnFloor");
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
	if (collider_->getBody()->GetLinearVelocity().x == 0) {
		animController_->setParamValue("Speed", 0);
	}
	
	/*//--No vel en Y
	if (collider_->getBody()->GetLinearVelocity().y == 0) {
		isOnFloor = true;
		animController_->setParamValue("NotOnFloor", 0);
	}*/
#pragma endregion
#pragma region Input
	//Gestion del input
	if (ih().keyDownEvent()) {
		assert(collider_ != nullptr);
		//Parte Horizontal
		if (ih().isKeyDown(SDL_SCANCODE_LEFT) && !isDashing) {
			b2Vec2 vel = collider_->getBody()->GetLinearVelocity();
			collider_->setSpeed(Vector2D(-speed_, vel.y));
			animController_->setParamValue("Speed", 1);
			animController_->flipX(false);
		}
		else if (ih().isKeyDown(SDL_SCANCODE_RIGHT) && !isDashing) {
			b2Vec2 vel = collider_->getBody()->GetLinearVelocity();
			collider_->setSpeed(Vector2D(speed_, vel.y));
			animController_->setParamValue("Speed", 1);
			animController_->flipX(true);
		}
		//Parte Vertical
		if (ih().isKeyDown(SDL_SCANCODE_SPACE) && isOnFloor && !isDashing) {
			isOnFloor = false;
			b2Vec2 vel = collider_->getBody()->GetLinearVelocity();
			//collider_->applyForce(Vector2D(0, -1), forceJump_ * 44.0f); Al ser gradual, le cuesta mucho m�s
			collider_->applyLinearForce(Vector2D(0, -1), forceJump_);

			//Realizar da�o
			//health_->loseLife();
			snd->playSoundEffect("player_jump", 300);
		}

		//Para juego final añadir && canDash
		if (ih().isKeyDown(SDLK_LSHIFT) && !isDashing) {
			gravity = collider_->getBody()->GetGravityScale();
			collider_->getBody()->SetGravityScale(0.0f);
			collider_->setSpeed(Vector2D(0, 0));
			isDashing = true;
			if (animController_->isFlipX())collider_->applyLinearForce(Vector2D(1, 0), dashSpeed);
			else collider_->applyLinearForce(Vector2D(-1, 0), dashSpeed);
			canDash = false;
		}

	}
	//---Para la velocidad X
	if ((ih().isKeyUp(SDL_SCANCODE_RIGHT) && ih().isKeyUp(SDL_SCANCODE_LEFT) && !isDashing)) {
		collider_->setSpeed(Vector2D(0, collider_->getBody()->GetLinearVelocity().y));
	}
#pragma endregion

	if (isDashing) {
		b2Vec2 vel = collider_->getBody()->GetLinearVelocity();
		if (vel.x > 0) {
			collider_->setSpeed(Vector2D(vel.x - 0.3, 0));
		}
		else {
			collider_->getBody()->SetGravityScale(gravity);
			isDashing = false;
		}
	}

	if (sdlutils().currRealTime() - lasTimeDashed >= dashCoolDown) {
		lasTimeDashed = sdlutils().currRealTime();
		canDash = true;
	}
}

void PlayerController::initEntityColliders()
{
	if (entity_->getComponent<TriggerCollider>() != nullptr) {
		trigger_ = entity_->getComponent<TriggerCollider>();

		trigger_->getTriggerEntity()->setCollisionMethod(OnTriggerEnter);
		trigger_->getTriggerEntity()->setEndCollisionMethod(OnTriggerExit);

		//createJointTrigger(trigger_->getTriggerEntity());
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

void PlayerController::OnTriggerEnter(b2Contact* contact)
{
	Entity* bodyA = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	if (bodyA != nullptr) {
		uint16 bodyA_layer = (bodyA->getComponent<BoxCollider>() != nullptr)? 
			bodyA->getComponent<BoxCollider>()->getColLayer() : bodyA->getComponent<MapCollider>()->getColLayer();
		if (bodyA_layer == PLAYER) {
			Entity* bodyB = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
			//METODO 1 (Mapa)
			if (bodyB->getComponent<BoxCollider>() != nullptr || bodyB->getComponent<MapCollider>() != nullptr) {
				uint16 bodyB_Layer = (bodyB->getComponent<BoxCollider>() != nullptr) ?
					bodyB->getComponent<BoxCollider>()->getColLayer() : bodyB->getComponent<MapCollider>()->getColLayer();
				if(bodyB_Layer == GROUND)
					bodyA->getComponent<BoxCollider>()->triggerCollide(true);
			}
			/*//METODO 2
			if (bodyB->getComponent<BoxCollider>() != nullptr) {
				if(bodyB->getComponent<BoxCollider>()->getColLayer() == GROUND)
					bodyA->getComponent<BoxCollider>()->triggerCollide(true);
			}*/
		}
		else {
			bodyA = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
			if (bodyA != nullptr) {
				uint16 bodyA_layer = (bodyA->getComponent<BoxCollider>() != nullptr) ?
					bodyA->getComponent<BoxCollider>()->getColLayer() : bodyA->getComponent<MapCollider>()->getColLayer();
				if (bodyA_layer == PLAYER) {
					Entity* bodyB = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
					//METODO 1 (Mapa)
					if (bodyB->getComponent<BoxCollider>() != nullptr || bodyB->getComponent<MapCollider>() != nullptr) {
						uint16 bodyB_Layer = (bodyB->getComponent<BoxCollider>() != nullptr) ?
							bodyB->getComponent<BoxCollider>()->getColLayer() : bodyB->getComponent<MapCollider>()->getColLayer();
						if(bodyB_Layer == GROUND)
							bodyA->getComponent<BoxCollider>()->triggerCollide(true);
					}
					//METODO 2
					/*if (bodyB->getComponent<BoxCollider>() != nullptr) {
						if (bodyB->getComponent<BoxCollider>()->getColLayer() == GROUND)
							bodyA->getComponent<BoxCollider>()->triggerCollide(true);
					}*/
				}
			}
		}
	}
}

void PlayerController::OnTriggerExit(b2Contact* contact)

{
	Entity* bodyA = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	if (bodyA != nullptr) {
		uint16 bodyA_layer = (bodyA->getComponent<BoxCollider>() != nullptr) ?
			bodyA->getComponent<BoxCollider>()->getColLayer() : bodyA->getComponent<MapCollider>()->getColLayer();
		if (bodyA_layer == PLAYER) {
			Entity* bodyB = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
			//METODO 1 (Mapa)
			if (bodyB->getComponent<BoxCollider>() != nullptr || bodyB->getComponent<MapCollider>() != nullptr) {
				uint16 bodyB_Layer = (bodyB->getComponent<BoxCollider>() != nullptr) ?
					bodyB->getComponent<BoxCollider>()->getColLayer() : bodyB->getComponent<MapCollider>()->getColLayer();
				if (bodyB_Layer == GROUND)
					bodyA->getComponent<BoxCollider>()->triggerCollide(false);
			}
			/*//METODO 2
			if (bodyB->getComponent<BoxCollider>() != nullptr) {
				if(bodyB->getComponent<BoxCollider>()->getColLayer() == GROUND)
					bodyA->getComponent<BoxCollider>()->triggerCollide(false);
			}*/
		}
		else {
			bodyA = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
			if (bodyA != nullptr) {
				uint16 bodyA_layer = (bodyA->getComponent<BoxCollider>() != nullptr) ?
					bodyA->getComponent<BoxCollider>()->getColLayer() : bodyA->getComponent<MapCollider>()->getColLayer();
				if (bodyA_layer == PLAYER) {
					Entity* bodyB = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
					//METODO 1 (Mapa)
					if (bodyB->getComponent<BoxCollider>() != nullptr || bodyB->getComponent<MapCollider>() != nullptr) {
						uint16 bodyB_Layer = (bodyB->getComponent<BoxCollider>() != nullptr) ?
							bodyB->getComponent<BoxCollider>()->getColLayer() : bodyB->getComponent<MapCollider>()->getColLayer();
						if (bodyB_Layer == GROUND)
							bodyA->getComponent<BoxCollider>()->triggerCollide(false);
					}
					//METODO 2
					/*if (bodyB->getComponent<BoxCollider>() != nullptr) {
						if (bodyB->getComponent<BoxCollider>()->getColLayer() == GROUND)
							bodyA->getComponent<BoxCollider>()->triggerCollide(false);
					}*/
				}
			}
		}
	}
}

void PlayerController::isOnGround()
{
	if (trigger_->getTriggerEntity()->getComponent<BoxCollider>()->isTriggerColliding())
		isOnFloor = true;
	else
		isOnFloor = false;
}
