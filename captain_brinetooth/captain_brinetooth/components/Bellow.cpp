#include "Bellow.h"

#include <iostream>
#include "../game/CollisionLayers.h"
#include "AnimBlendGraph.h"

using namespace ColLayers;

void Bellow::init() {
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);
	anim_ = entity_->getComponent<AnimBlendGraph>();
}

void Bellow::update() {
	if (ih().keyDownEvent()) {
		if (ih().isKeyDown(SDL_SCANCODE_E)) {

			//Player not attacking or in combo
			if (CURRENT_STATUS == STATUS::Iddle && shotActivationTime + timeBetweenShots < sdlutils().currRealTime()) {
				std::cout << "Started shooting\n";

				//Set player as sawing
				CURRENT_STATUS = STATUS::Shooting;

				//Activate attack animation + sawing on attack
				if (anim_->getParamIndex("machineGun_att") != -1)
					anim_->setParamValue("machineGun_att", 1);

				//Shoot
				shoot();

				//Time control variables
				shotActivationTime = sdlutils().currRealTime();
			}
		}
	}

	//Check out of input cases
	if (CURRENT_STATUS == STATUS::Shooting && shotActivationTime + timeBetweenShots < sdlutils().currRealTime()) {
		//Deactivate chainsaw
		std::cout << "Stopped shooting\n";
		CURRENT_STATUS = STATUS::Iddle;

		if (anim_->getParamIndex("machineGun_att") != -1)
			anim_->setParamValue("machineGun_att", 2);
	}
}

void Bellow::shoot() {
	auto* level0 = entity_->getMngr()->getHandler<Map>()->getComponent<Level0>();
	Entity* bullet = entity_->getMngr()->addEntity();

	Vector2D bulletpos; Vector2D bulletvel;
	if (anim_->isFlipX()) {
		bulletpos = tr_->getPos() + Vector2D(+150, 0);
	}
	else {
		bulletpos = tr_->getPos() + Vector2D(-150, 0);
	}

	//Sonido

	bullet->addComponent<Transform>(bulletpos, Vector2D(0, 0), 150.0f, 150.0f, 0.0f);
	AnimBlendGraph* anim_controller = bullet->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("iddle", &sdlutils().images().at("machine_gun_bullet"), 1, 1, 1, 1, 1);
	bullet->addComponent<DisableOnExit>();
	bullet->addComponent<BoxCollider>(DYNAMIC, PLAYER_ATTACK, PLAYER_ATTACK_MASK, true);
	bullet->addComponent<WeaponDamageDetection>(150, 2);
	bullet->addComponent<InkMovement>(!anim_->isFlipX());

	level0->addProjectile(bullet);
}