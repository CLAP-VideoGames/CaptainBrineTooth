#include "FishlerController.h"

void FishlerController::init() {
	currentPhase = Phase1;
	currentMovement = Walking;
	currentAttack = NotAttacking;

	movementTimer.currentTime = sdlutils().currRealTime();

	tr_ = entity_->getComponent<Transform>();
	collider_ = entity_->getComponent<BoxCollider>();
	anim_ = entity_->getComponent<AnimBlendGraph>();
	playertr = entity_->getMngr()->getHandler<Player>()->getComponent<Transform>();
	health = entity_->getComponent<Enemy_Health>();

	movementTimer.maxTime = sdlutils().rand().teCuoto(1500, 4000);
	walkingSpeed = 0.7f;
	rushSpeed = 3;
	bulletVelocity = 20;
	spikeVelocity = 20;

	rushAttackTelegraphTime = 500;
	rushAttackTime = 2000;

	shootAttackTelegraphTime = 500;
	shootAttackTime = 1000;

	spikesAttackTelegraphTime = 1700;
	spikeAttackTime = 1000;

	changePhaseRequest = false;
	phaseChangeTimer.maxTime = 2000;

	//Trigger position variables
	triggerWidth = 150;
	triggerHeight = 200;
	triggerOffSetX = -100;
	triggerOffSetY = 0;
}

FishlerController::~FishlerController() {
	if (trigger != nullptr) {
		trigger->setActive(false);
		trigger = nullptr;
	}
}

void FishlerController::update() {
	if (health->getHealth() > 0) {
		//Movement
		if (currentMovement == Walking && movementTimer.currentTime + movementTimer.maxTime < sdlutils().currRealTime()) {
			//Make a random attack
			currentMovement = AttackTelegraph;

			if (currentPhase == Phase1)
				//Random between rush, bite and shoot
				currentAttack = (ATTACKS)sdlutils().rand().teCuoto(1, 3);
			else
				currentAttack = (ATTACKS)sdlutils().rand().teCuoto(1, 4);

			//Stop fishler movement
			collider_->setSpeed(Vector2D(0, 0));

			switch (currentAttack) {
				//Initiate rush telegraphing
			case ATTACKS::RushAttack:
				attackTelegraphTimer.maxTime = rushAttackTelegraphTime;
				attackTelegraphTimer.currentTime = sdlutils().currRealTime();
				entity_->getMngr()->getSoundMngr()->playSoundEffect("fishler_run", 0);

				if ((playertr->getPos().getX() < tr_->getPos().getX())) {
					anim_->flipX(true);
				}
				else {
					anim_->flipX(false);
				}

				//Initiate Rush telegraph animation
				anim_->setParamValue("fishler_action", 2);

				break;
			case ATTACKS::Shoot:
				attackTelegraphTimer.maxTime = shootAttackTelegraphTime;
				attackTelegraphTimer.currentTime = sdlutils().currRealTime();
				entity_->getMngr()->getSoundMngr()->playSoundEffect("fishler_shot", 0);
				if ((playertr->getPos().getX() < tr_->getPos().getX())) {
					anim_->flipX(true);
				}
				else {
					anim_->flipX(false);
				}

				//Initiate Shoot telegraph animation
				anim_->setParamValue("fishler_action", 1);

				break;
			case ATTACKS::Spikes:
				attackTelegraphTimer.maxTime = spikesAttackTelegraphTime;
				attackTelegraphTimer.currentTime = sdlutils().currRealTime();
				entity_->getMngr()->getSoundMngr()->playSoundEffect("explosion_pez", 0);

				if ((playertr->getPos().getX() < tr_->getPos().getX())) {
					anim_->flipX(false);
				}
				else {
					anim_->flipX(true);
				}

				//Initiate spike shooting telegraph animation
				anim_->setParamValue("fishler_action", 4);

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
				float direction;
				if ((playertr->getPos().getX() < tr_->getPos().getX())) {
					direction = -1;
				}
				else {
					direction = 1;
				}

				collider_->Resize(Vector2D(300, 200));
				collider_->setSpeed(Vector2D(direction * rushSpeed, 0));

				//Initiate rush attack animation
				anim_->setParamValue("fishler_action", 3);

				break;
				//Shoot attack
			case ATTACKS::Shoot:
				attackTimer.maxTime = shootAttackTime;
				attackTimer.currentTime = sdlutils().currRealTime();

				//Spawn shot
				shoot();

				//Initiate shoot attack animation


				break;
			case ATTACKS::Spikes:
				attackTimer.maxTime = spikeAttackTime;
				attackTimer.currentTime = sdlutils().currRealTime();

				//Spawn a certain number of spikes
				for (int i = 0; i < 3; i++) {
					shootSpike();
				}

				//Initiate spike shooting attack animation


				break;
			}
		}
		//Back to normal movement that is walking
		else if (currentMovement == Attacking && attackTimer.currentTime + attackTimer.maxTime < sdlutils().currRealTime()) {
			if (changePhaseRequest) {
				currentPhase = PHASE::Phase2;
				currentMovement = MOVEMENT_STATE::PhaseChange;

				phaseChangeTimer.currentTime = sdlutils().currRealTime();
				entity_->getMngr()->getSoundMngr()->playSoundEffect("fishler_risa", 0);

				anim_->setParamValue("fishler_action", 5);

				if (currentAttack == ATTACKS::RushAttack)
					collider_->Resize(Vector2D(300*0.2, 200));
				collider_->setSpeed(Vector2D());

				changePhaseRequest = false;
			}
			else {
				movementTimer.maxTime = sdlutils().rand().teCuoto(1500, 4000); //Random movement time for a little extra spice
				movementTimer.currentTime = sdlutils().currRealTime();

				if (currentAttack == ATTACKS::RushAttack)
					collider_->Resize(Vector2D(300 * 0.2, 200));
				currentMovement = Walking;

				anim_->setParamValue("fishler_action", 0);
			}
		}

		//Walks in the direction of the player
		if (currentMovement == Walking) {
			float direction;
			if ((playertr->getPos().getX() < tr_->getPos().getX())) {
				direction = -1;
				anim_->flipX(true);
			}
			else {
				direction = 1;
				anim_->flipX(false);
			}
			collider_->setSpeed(Vector2D(direction * walkingSpeed, collider_->getBody()->GetLinearVelocity().y));
		}
		else if (currentMovement == MOVEMENT_STATE::PhaseChange && phaseChangeTimer.currentTime + phaseChangeTimer.maxTime < sdlutils().currRealTime()) {
			//Disparar pinchos
			currentMovement = MOVEMENT_STATE::AttackTelegraph;
			currentAttack = ATTACKS::Spikes;
			attackTelegraphTimer.maxTime = spikesAttackTelegraphTime;
			attackTelegraphTimer.currentTime = sdlutils().currRealTime();
			entity_->getMngr()->getSoundMngr()->playSoundEffect("explosion_pez", 0);

			if ((playertr->getPos().getX() < tr_->getPos().getX())) {
				anim_->flipX(false);
			}
			else {
				anim_->flipX(true);
			}

			//Initiate spike shooting telegraph animation
			anim_->setParamValue("fishler_action", 4);
		}
	}
	else {

		collider_->setSpeed(Vector2D());
	}
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
	trigger->addComponent<ContactDamage>(entity_);
	trigger->addComponent<DisableOnExit>();
}

/// <summary>
/// Spawns a bullet in the direction fishler is looking at
/// </summary>
void FishlerController::shoot() {
	Entity* bullet = entity_->getMngr()->addEntity();

	Vector2D bulletpos; Vector2D bulletvel;
	if (!anim_->isFlipX()) {
		bulletpos = tr_->getPos();
		bulletvel = Vector2D(1, 0);
	}
	else {
		bulletpos = tr_->getPos();
		bulletvel = Vector2D(-1, 0);
	}

	bullet->addComponent<Transform>(bulletpos, Vector2D(0, 0), 70.0f, 20.0f, 0.0f);
	AnimBlendGraph* anim_controller = bullet->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("iddle", &sdlutils().images().at("fishler_bullet"), 1, 1, 1, 1, 1);
	if (anim_->isFlipX()) {
		anim_controller->flipX(true);
	}
	else {
		anim_controller->flipX(false);
	}
	bullet->addComponent<DisableOnExit>();
	bullet->addComponent<BoxCollider>(DYNAMIC, ENEMY_ATTACK, ENEMY_ATTACK_MASK, true);
	bullet->getComponent<BoxCollider>()->getBody()->SetGravityScale(0);
	bullet->getComponent<BoxCollider>()->applyForce(bulletvel, bulletVelocity);
	bullet->addComponent<ContactDamage>(entity_);
	bullet->addComponent<DestroyOnCollision>();
}

void FishlerController::shootSpike() {
	Entity* spike = entity_->getMngr()->addEntity();

	Vector2D bulletpos; Vector2D bulletvel;
	if (anim_->isFlipX()) {
		bulletpos = tr_->getPos();
		bulletvel = Vector2D(1, (float)sdlutils().rand().teCuoto(-100, 20) / 100.0f);
	}
	else {
		bulletpos = tr_->getPos();
		bulletvel = Vector2D(-1, (float)sdlutils().rand().teCuoto(-100, 20) / 100.0f);
	}

	spike->addComponent<Transform>(bulletpos, Vector2D(0, 0), 50.0f, 50.0f, 0.0f);
	AnimBlendGraph* anim_controller = spike->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("iddle", &sdlutils().images().at("fishler_spike"), 1, 1, 1, 1, 1);
	if (anim_->isFlipX()) {
		anim_controller->flipX(true);
	}
	else {
		anim_controller->flipX(false);
	}
	spike->addComponent<DisableOnExit>(12000);
	spike->addComponent<BoxCollider>(DYNAMIC, ENEMY_ATTACK, ENEMY_ATTACK_MASK, true);
	spike->getComponent<BoxCollider>()->applyForce(bulletvel, bulletVelocity);
	spike->addComponent<GetStuckOnWall>();
	spike->addComponent<ContactDamage>(entity_);
}

void FishlerController::changePhase() {
	changePhaseRequest = true;
}