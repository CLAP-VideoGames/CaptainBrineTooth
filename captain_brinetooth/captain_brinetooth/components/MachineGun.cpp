#include "MachineGun.h"

#include <iostream>
#include "../game/CollisionLayers.h"
#include "AnimBlendGraph.h"

using namespace ColLayers;

void MachineGun::init() {
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);
	anim_ = entity_->getComponent<AnimBlendGraph>();

	bulletVelocity = 2;
}

void MachineGun::update() {
	if (ih().keyDownEvent()) {
		if (ih().isKeyDown(SDL_SCANCODE_E)) {

			//Player not attacking or in combo
			if (CURRENT_STATUS == STATUS::Iddle && overheat <= maxOverheat && startedReloading + reloadingTime < sdlutils().currRealTime()) {
				std::cout << "Started shooting\n";

				//Set player as sawing
				CURRENT_STATUS = STATUS::Shooting;

				//Activate attack animation + sawing on attack
				if (anim_->searchParamValue("chainsaw_att") != -1)
					anim_->setParamValue("chainsaw_att", 1);

				//Shoot
				shoot();

				//Time control variables
				shotActivationTime = sdlutils().currRealTime();
			}
		}
	}

	if (ih().keyUpEvent()) {
		if (ih().isKeyUp(SDL_SCANCODE_E) && CURRENT_STATUS == STATUS::Shooting) {
			std::cout << "Stopped shooting\n";

			CURRENT_STATUS = STATUS::Iddle;

			//Desactivar animacion

		}
	}

	//Check out of input cases
	if (CURRENT_STATUS == STATUS::Shooting && overheat >= maxOverheat) {
		//Deactivate chainsaw
		std::cout << "Started reload\n";
		CURRENT_STATUS = STATUS::Reloading;

		startedReloading = sdlutils().currRealTime();

		if (anim_->searchParamValue("chainsaw_att") != -1)
			anim_->setParamValue("chainsaw_att", 0);
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

		if (anim_->searchParamValue("chainsaw_att") != -1)
			anim_->setParamValue("chainsaw_att", 0);

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

void MachineGun::shoot() {
	Entity* bullet = entity_->getMngr()->addEntity();

	Vector2D bulletpos; Vector2D bulletvel;
	if (entity_->getComponent<PlayerController>()->getFlip()) {
		bulletpos = tr_->getPos() + Vector2D(tr_->getW() + 60, entity_->getComponent<Transform>()->getH() / 2);
		bulletvel = Vector2D(1, 0);
	}
	else {
		bulletpos = tr_->getPos() + Vector2D(-60, entity_->getComponent<Transform>()->getH() / 2);
		bulletvel = Vector2D(-1, 0);
	}


	bullet->addComponent<Transform>(bulletpos, Vector2D(0, 0), 10.0f, 10.0f, 0.0f);
	AnimBlendGraph* anim_controller = bullet->addComponent<AnimBlendGraph>();
	bullet->addComponent<DisableOnExit>();
	bullet->addComponent<BoxCollider>(DYNAMIC, ENEMY_ATTACK, ENEMY_ATTACK_MASK);
	bullet->getComponent<BoxCollider>()->applyForce(bulletvel, bulletVelocity);
	bullet->addComponent<WeaponDamageDetection>(20);
	// bullet->addComponent<DestroyOnCollision>(); // esto da problemas si no se hacen bien las capas de colision?
}