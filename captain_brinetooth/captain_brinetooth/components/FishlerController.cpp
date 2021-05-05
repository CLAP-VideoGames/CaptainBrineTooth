#include "FishlerController.h"

void FishlerController::init() {
	currentPhase = Phase1;
	currentMovement = Walking;
	currentAttack = NotAttacking;

	movementTimer.currentTime = sdlutils().currRealTime();

	tr_ = entity_->getComponent<Transform>();
	playerCloseSize = Vector2D(400, 200);
	playerIsClose = false;
}

FishlerController::~FishlerController() {
	if (playerCloseDetect != nullptr) {
		playerCloseDetect->setActive(false);
		playerCloseDetect = nullptr;
	}
}

void FishlerController::update() {
	//PHASE 1
	if (currentPhase == Phase1) {
		//Movement
		if (currentMovement == Walking && movementTimer.currentTime + movementTimer.maxTime < sdlutils().currRealTime()) {
			//Make a random attack
			currentMovement = Attacking;
		}
	}
	//PHASE 2
	else {

	}


}

void FishlerController::render()
{
}

void FishlerController::playerDetected(b2Contact* contact) {
	Entity* fishler = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	if (fishler != nullptr && fishler->hasComponent<FishlerController>()) {
		fishler->getComponent<FishlerController>()->playerDetection(true);
	}
	else {
		fishler = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
		if (fishler != nullptr && fishler->hasComponent<FishlerController>()) {
			fishler->getComponent<FishlerController>()->playerDetection(true);
		}
	}
}

void FishlerController::playerLost(b2Contact* contact) {
	Entity* fishler = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	if (fishler != nullptr && fishler->hasComponent<FishlerController>()) {
		fishler->getComponent<FishlerController>()->playerDetection(false);
	}
	else {
		fishler = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
		if (fishler != nullptr && fishler->hasComponent<FishlerController>()) {
			fishler->getComponent<FishlerController>()->playerDetection(false);
		}
	}
}

void FishlerController::playerDetection(bool entered) {
	playerIsClose = entered;
}

void FishlerController::playerCloseDetection() {
	playerCloseDetect = entity_->getMngr()->addEntity();
	playerCloseDetect->addComponent<Transform>(tr_->getPos(), Vector2D(), playerCloseSize.getX(), playerCloseSize.getY(), 0.0f);
	playerCloseDetect->addComponent<BoxCollider>(TYPE::KINEMATIC, PLAYER_DETECTION, PLAYER_DETECTION_MASK);
	playerCloseDetect->setCollisionMethod(playerDetected);
	playerCloseDetect->setEndCollisionMethod(playerLost);
}