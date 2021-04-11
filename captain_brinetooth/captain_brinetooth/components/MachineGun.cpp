#include "MachineGun.h"

#include <iostream>
#include "../game/CollisionLayers.h"
#include "AnimBlendGraph.h"

using namespace ColLayers;

void MachineGun::init() {
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);
	anim_ = entity_->getComponent<AnimBlendGraph>();
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
		std::cout << "Building overheat\n";
		overheat++;
	}
	else if (CURRENT_STATUS == STATUS::Reloading && startedReloading + reloadingTime < sdlutils().currRealTime()) {
		//Deactivate animation lock
		std::cout << "Stopped reloading\n";
		CURRENT_STATUS = STATUS::Iddle;

		if (anim_->searchParamValue("chainsaw_att") != -1)
			anim_->setParamValue("chainsaw_att", 0);

		overheat = 0;
	}

	//Comprobamos si hay que disparar una bala
	if (CURRENT_STATUS == STATUS::Shooting && shotActivationTime + timeBetweenShots < sdlutils().currRealTime()) {
		
		//Shoot


		shotActivationTime = sdlutils().currRealTime();
	}
}