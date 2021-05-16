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
	if (!entity_->getComponent<PlayerController>()->isPlayerDashing()) {
		if (ih().mouseButtonEvent() || ih().keyUpEvent()) {
			if (ih().getMouseButtonState(InputHandler::MOUSEBUTTON::LEFT) || ih().isKeyDown(SDL_CONTROLLER_BUTTON_X)) {

				//Player not attacking or in combo
				if (CURRENT_STATUS == STATUS::Iddle && shotActivationTime + timeBetweenShots < sdlutils().currRealTime()) {
					std::cout << "Started shooting\n";

					//Set player as sawing
					CURRENT_STATUS = STATUS::Shooting;
					//Activate attack animation + sawing on attack
					if (anim_->getParamIndex("bellow_att") != -1)
						anim_->setParamValue("bellow_att", 1);

					//Shoot
					isAttacking = true;
					shoot();

					//Time control variables
					shotActivationTime = sdlutils().currRealTime();
				}
			}
		}
	}
	

	//Check out of input cases
	if (CURRENT_STATUS == STATUS::Shooting && shotActivationTime + timeBetweenShots < sdlutils().currRealTime()) {
		//Deactivate chainsaw
		std::cout << "Stopped shooting\n";
		CURRENT_STATUS = STATUS::Iddle;

		isAttacking = false;

		if (anim_->getParamIndex("bellow_att") != -1)
			anim_->setParamValue("bellow_att", 0);
	}
}

void Bellow::shoot() {
	auto* level0 = entity_->getMngr()->getHandler<Map>()->getComponent<Level0>();
	Entity* bullet = entity_->getMngr()->addEntity();

	Vector2D bulletpos; Vector2D bulletvel;
	if (anim_->isFlipX()) {
		bulletpos = tr_->getPos() + Vector2D(+150, -30);
	}
	else {
		bulletpos = tr_->getPos() + Vector2D(-150, -30);
	}

	//Sonido

	bullet->addComponent<Transform>(bulletpos, Vector2D(0, 0), 150.0f, 150.0f, 0.0f);
	AnimBlendGraph* anim_controller = bullet->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("tint", &sdlutils().images().at("squid_tint"), 2, 2, 4, 8, -1);
	bullet->addComponent<DisableOnExit>();
	bullet->addComponent<BoxCollider>(DYNAMIC, PLAYER_ATTACK, PLAYER_ATTACK_MASK, true);
	bullet->addComponent<WeaponDamageDetection>(20, 2);
	bullet->addComponent<InkMovement>(!anim_->isFlipX());

	level0->addProjectile(bullet);
	entity_->getMngr()->getSoundMngr()->playSoundEffect("inkShot", 0);

}