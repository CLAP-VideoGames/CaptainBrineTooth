#include "MachineGun.h"

#include <iostream>
#include "../game/CollisionLayers.h"
#include "AnimBlendGraph.h"

using namespace ColLayers;

void MachineGun::init() {
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);
	anim_ = entity_->getComponent<AnimBlendGraph>();

	bulletVelocity = 50;
}

void MachineGun::update() {
	if (entity_->hasComponent<Player_Health>()) {
		if (!entity_->getComponent<PlayerController>()->isPlayerDashing() && !entity_->getComponent<Player_Health>()->getPlayerIsDying()) {
			if (ih().mouseButtonEvent() || ih().keyDownEvent()) {
				if (ih().getMouseButtonState(InputHandler::MOUSEBUTTON::LEFT) || ih().isKeyDown(SDL_CONTROLLER_BUTTON_X)) {

					//Player not attacking or in combo
					if (CURRENT_STATUS == STATUS::Iddle && overheat <= maxOverheat && startedReloading + reloadingTime < sdlutils().currRealTime()) {
						std::cout << "Started shooting\n";

						//Set player as sawing
						CURRENT_STATUS = STATUS::Shooting;

						//Activate attack animation + sawing on attack
						if (anim_->getParamIndex("machineGun_att") != -1)
							anim_->setParamValue("machineGun_att", 1);

						//Shoot
						isAttacking = true;
						shoot();

						//Time control variables
						shotActivationTime = sdlutils().currRealTime();
					}
				}
			}
		}
		else {
			if (CURRENT_STATUS != STATUS::Iddle) {
				CURRENT_STATUS = STATUS::Iddle;

				isAttacking = false;
				if (anim_->getParamIndex("machineGun_att") != -1)
					anim_->setParamValue("machineGun_att", 0);
			}
		}


		if (ih().mouseButtonEvent()) {
			if (!ih().getLeftMouseButtonPressed() && CURRENT_STATUS == STATUS::Shooting) {
				std::cout << "Stopped shooting\n";

				CURRENT_STATUS = STATUS::Iddle;

				isAttacking = false;
				//Desactivar animacion
				if (anim_->getParamIndex("machineGun_att") != -1)
					anim_->setParamValue("machineGun_att", 0);
			}
		}

		//Check out of input cases
		if (CURRENT_STATUS == STATUS::Shooting && overheat >= maxOverheat) {
			//Deactivate chainsaw
			std::cout << "Started reload\n";
			CURRENT_STATUS = STATUS::Reloading;

			startedReloading = sdlutils().currRealTime();
			entity_->getMngr()->getSoundMngr()->playSoundEffect("recarga_anchoa", 0);

			isAttacking = false;
			if (anim_->getParamIndex("machineGun_att") != -1)
				anim_->setParamValue("machineGun_att", 2);
		}
		else if (CURRENT_STATUS == STATUS::Shooting && overheatSpikeTime + timeBetweenOverheatSpikes < sdlutils().currRealTime()) {

			overheat++;

			std::cout << overheat << "\n";

			overheatSpikeTime = sdlutils().currRealTime();
		}
		else if (CURRENT_STATUS == STATUS::Reloading && startedReloading + reloadingTime < sdlutils().currRealTime()) {
			//Deactivate animation lock
			std::cout << "Stopped reloading\n";
			CURRENT_STATUS = STATUS::Iddle;

			if (!ih().isKeyDown(SDL_SCANCODE_E)) {
				if (anim_->getParamIndex("machineGun_att") != -1)
					anim_->setParamValue("machineGun_att", 0);
			}

			overheat = 0;
		}

		//Decrementamos el overheat
		if (CURRENT_STATUS == STATUS::Iddle && overheat > 0 && decreaseOverheatSpikeTime + timeBetweenOverheatSpikes < sdlutils().currRealTime()) {
			overheat--;

			std::cout << overheat << "\n";
			decreaseOverheatSpikeTime = sdlutils().currRealTime();
		}

		//Comprobamos si hay que disparar una bala
		if (CURRENT_STATUS == STATUS::Shooting && shotActivationTime + timeBetweenShots < sdlutils().currRealTime()) {

			//Shoot
			shoot();

			shotActivationTime = sdlutils().currRealTime();
		}
	}
}

void MachineGun::shoot() {
	Entity* bullet = entity_->getMngr()->addEntity();

	Vector2D bulletpos; Vector2D bulletvel;
	if (anim_->isFlipX()) {
		bulletpos = tr_->getPos() + Vector2D(+60, 0);
		bulletvel = Vector2D(1, 0);
	}
	else {
		bulletpos = tr_->getPos() + Vector2D(-60, 0);
		bulletvel = Vector2D(-1, 0);
	}

	entity_->getMngr()->getSoundMngr()->playSoundEffect("disparo_anchoa", 700);

	bullet->addComponent<Transform>(bulletpos, Vector2D(0, 0), 70.0f, 20.0f, 0.0f);
	AnimBlendGraph* anim_controller = bullet->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("iddle", &sdlutils().images().at("machine_gun_bullet"), 1, 1, 1, 1, 1);
	if (bulletvel.getX() < 0) {
		anim_controller->flipX(true);
	}else
		anim_controller->flipX(false);
	bullet->addComponent<DisableOnExit>();
	bullet->addComponent<BoxCollider>(DYNAMIC, PLAYER_ATTACK, PLAYER_ATTACK_MASK, true);
	bullet->getComponent<BoxCollider>()->getBody()->SetGravityScale(0);
	bullet->getComponent<BoxCollider>()->applyForce(bulletvel, bulletVelocity);
	bullet->addComponent<WeaponDamageDetection>(7, 0, true);
	bullet->addComponent<DestroyOnCollision>(); // esto da problemas si no se hacen bien las capas de colision?
}