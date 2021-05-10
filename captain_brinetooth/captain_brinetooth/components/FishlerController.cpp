#include "FishlerController.h"

void FishlerController::init() {
	currentPhase = Phase1;
	currentMovement = Walking;
	currentAttack = NotAttacking;

	movementTimer.currentTime = sdlutils().currRealTime();

	tr_ = entity_->getComponent<Transform>();
	collider_ = entity_->getComponent<BoxCollider>();
	anim_ = entity_->getComponent<AnimBlendGraph>();
	playerCloseSize = Vector2D(400, 200);
	playertr = entity_->getMngr()->getHandler<Player>()->getComponent<Transform>();
	playerIsClose = false;

	movementTimer.maxTime = sdlutils().rand().teCuoto(1500, 4000);
	walkingSpeed = 10;
	rushSpeed = 50;
	bulletVelocity = 50;

	rushAttackTelegraphTime = 500;
	rushAttackTime = 2000;

	biteAttackTelegraphTime = 500;
	biteAttackTime = 2000;

	shootAttackTelegraphTime = 500;
	shootAttackTime = 1000;

	//Trigger position variables
	triggerWidth = 100;
	triggerHeight = 100;
	triggerOffSetX = -90;
	triggerOffSetY = -30;
}

FishlerController::~FishlerController() {
	if (playerCloseDetect != nullptr) {
		playerCloseDetect->setActive(false);
		playerCloseDetect = nullptr;
	}
	if (trigger != nullptr) {
		trigger->setActive(false);
		trigger = nullptr;
	}
}

void FishlerController::update() {

	//Movement
	if (currentMovement == Walking && movementTimer.currentTime + movementTimer.maxTime < sdlutils().currRealTime()) {
		//Make a random attack
		currentMovement = AttackTelegraph;

		//Random between rush, bite and shoot
		currentAttack = (ATTACKS)sdlutils().rand().teCuoto(1, 4);

		//Stop fishler movement
		collider_->setSpeed(Vector2D());

		switch (currentAttack) {
			//Initiate rush telegraphing
		case ATTACKS::RushAttack:
			attackTelegraphTimer.maxTime = rushAttackTelegraphTime;
			attackTelegraphTimer.currentTime = sdlutils().currRealTime();

			//Initiate Rush telegraph animation


			break;
			//Initiate bite telegraphing
		case ATTACKS::Bite:
			attackTelegraphTimer.maxTime = biteAttackTelegraphTime;
			attackTelegraphTimer.currentTime = biteAttackTime;

			//Initiate Bite telegraph animation


			break;
			//Initiate shoot telegraphing
		case ATTACKS::Shoot:
			attackTelegraphTimer.maxTime = shootAttackTelegraphTime;
			attackTelegraphTimer.currentTime = sdlutils().currRealTime();

			//Initiate Shoot telegraph animation


			break;
		}
	}
	//Attack telegraph
	else if (currentMovement == AttackTelegraph && attackTelegraphTimer.currentTime + attackTelegraphTimer.maxTime < sdlutils().currRealTime()) {
		currentMovement = Attacking;

		switch (currentAttack) {
			//Rush attack
		case ATTACKS::RushAttack:
			attackTimer.maxTime = rushAttackTime;
			attackTimer.currentTime = sdlutils().currRealTime();

			//Attack
			collider_->setSpeed(Vector2D((playertr->getPos().getX() - tr_->getPos().getX()) / std::abs((playertr->getPos().getX() - tr_->getPos().getX())) * rushSpeed, 0));

			//Initiate rush attack animation


			break;
			//Bite attack
		case ATTACKS::Bite:
			attackTimer.maxTime = biteAttackTime;
			attackTimer.currentTime = sdlutils().currRealTime();

			//Attack
			//Spawn attack zone in the direction of the player
			creaTrigger();

			//Initiate bite attack animation


			break;
			//Shoot attack
		case ATTACKS::Shoot:
			attackTimer.maxTime = shootAttackTime;
			attackTimer.currentTime = sdlutils().currRealTime();

			//Spawn shot
			shoot();

			//Initiate shoot attack animation


			break;
		}
	}
	//Back to normal movement that is: walking or maybe jumping in phase 2
	else if (currentMovement == Attacking && attackTimer.currentTime + attackTimer.maxTime < sdlutils().currRealTime()) {
		movementTimer.maxTime = sdlutils().rand().teCuoto(1500, 4000); //Random movement time for a little extra spice
		movementTimer.currentTime = sdlutils().currRealTime();

		currentMovement = Walking;
	}

	//Walks in the direction of the player
	if (currentMovement == Walking) {
		collider_->setSpeed(Vector2D((playertr->getPos().getX() - tr_->getPos().getX()) / std::abs((playertr->getPos().getX() - tr_->getPos().getX())) * walkingSpeed, 0));
	}

}

/// <summary>
/// Called when the player gets into melee range
/// </summary>
/// <param name="contact"></param>
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

/// <summary>
/// Called when the player gets out of melee range
/// </summary>
/// <param name="contact"></param>
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

/// <summary>
/// Changes the variable playerIsClose
/// </summary>
/// <param name="entered"></param>
void FishlerController::playerDetection(bool entered) {
	playerIsClose = entered;
}

/// <summary>
/// Creates a trigger that detects when the player is close to Fishler
/// </summary>
void FishlerController::playerCloseDetection() {
	playerCloseDetect = entity_->getMngr()->addEntity();
	playerCloseDetect->addComponent<Transform>(tr_->getPos(), Vector2D(), playerCloseSize.getX(), playerCloseSize.getY(), 0.0f);
	playerCloseDetect->addComponent<BoxCollider>(TYPE::KINEMATIC, PLAYER_DETECTION, PLAYER_DETECTION_MASK);
	playerCloseDetect->setCollisionMethod(playerDetected);
	playerCloseDetect->setEndCollisionMethod(playerLost);
}

/// <summary>
/// Creates a trigger that deals damage to the player in the direction Fishler is looking
/// </summary>
void FishlerController::creaTrigger() {
	trigger = entity_->getMngr()->addEntity();

	if (anim_->isFlipX()) trigger->addComponent<Transform>(tr_->getPos() + Vector2D(-triggerOffSetX, triggerOffSetY),
		Vector2D(0, 0), triggerWidth, triggerHeight, 0.0f);
	else trigger->addComponent<Transform>(tr_->getPos() + Vector2D(triggerOffSetX, triggerOffSetY),
		Vector2D(0, 0), triggerWidth, triggerHeight, 0.0f);

	trigger->addComponent<BoxCollider>(TYPE::KINEMATIC, ENEMY_ATTACK, ENEMY_ATTACK_MASK, true);
	trigger->addComponent<ContactDamage>();
}

/// <summary>
/// Spawns a bullet in the direction fishler is looking at
/// </summary>
void FishlerController::shoot() {
	Entity* bullet = entity_->getMngr()->addEntity();

	Vector2D bulletpos; Vector2D bulletvel;
	if (anim_->isFlipX()) {
		bulletpos = tr_->getPos() + Vector2D(tr_->getW(), 0);
		bulletvel = Vector2D(1, 0);
	}
	else {
		bulletpos = tr_->getPos() + Vector2D(-tr_->getW(), 0);
		bulletvel = Vector2D(-1, 0);
	}

	bullet->addComponent<Transform>(bulletpos, Vector2D(0, 0), 70.0f, 20.0f, 0.0f);
	AnimBlendGraph* anim_controller = bullet->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("iddle", &sdlutils().images().at("machine_gun_bullet"), 1, 1, 1, 1, 1);
	bullet->addComponent<DisableOnExit>();
	bullet->addComponent<BoxCollider>(DYNAMIC, ENEMY_ATTACK, ENEMY_ATTACK_MASK, true);
	bullet->getComponent<BoxCollider>()->getBody()->SetGravityScale(0);
	bullet->getComponent<BoxCollider>()->applyForce(bulletvel, bulletVelocity);
	bullet->addComponent<ContactDamage>();
	bullet->addComponent<DestroyOnCollision>();
}