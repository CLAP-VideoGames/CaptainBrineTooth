#include "Hammer.h"

#include <iostream>
#include "../CollisionLayers.h"

using namespace ColLayers;

void Hammer::init() {
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);
	anim_ = entity_->getComponent<AnimBlendGraph>();
}

void Hammer::update() {
	if (ih().keyDownEvent()) {
		if (ih().isKeyDown(SDL_SCANCODE_E)) {

			//Player not attacking or in combo
			if (CURRENT_STATUS == STATUS::Iddle) {
				std::cout << "Martillazo! el primero\n";

				//Set player as sawing
				CURRENT_STATUS = STATUS::OnAnimationLock;
				CURRENT_ATTACK = ATTACKS::Attack1;

				//Activate attack animation + sawing on attack
				if (anim_->searchParamValue("hammer_att") != -1)
					anim_->setParamValue("hammer_att", 1);

				//Activate attack trigger
				trigger = entity_->getMngr()->addEntity();
				trigger->addComponent<Transform>(tr_->getPos() + Vector2D(triggerOffSetX, triggerOffSetY),
					Vector2D(0, 0), triggerWidth, triggerHeight, 0.0f);
				/*anim_controller = trigger->addComponent<AnimBlendGraph>();
				anim_controller->addAnimation("iddle", &sdlutils().images().at("fondo"), 1, 1, 1, 1, 1);*/
				trigger->addComponent<BoxCollider>(TYPE::KINEMATIC, PLAYER_ATTACK, PLAYER_ATTACK_MASK, true);

				//Time control variables
				stoppedSawTime = sdlutils().currRealTime();
			}
			//Player already attacked once
			else if (CURRENT_STATUS == STATUS::OnCombo && CURRENT_ATTACK == ATTACKS::Attack1) {
				std::cout << "Martillazo! el segundo\n";

				CURRENT_STATUS = STATUS::OnAnimationLock;
				CURRENT_ATTACK = ATTACKS::Attack2;

				//
				if (anim_->searchParamValue("hammer_att") != -1)
					anim_->setParamValue("hammer_att", 2);

				trigger = entity_->getMngr()->addEntity();
				trigger->addComponent<Transform>(tr_->getPos() + Vector2D(triggerOffSetX, triggerOffSetY),
					Vector2D(0, 0), triggerWidth, triggerHeight, 0.0f);
				/*anim_controller = trigger->addComponent<AnimBlendGraph>();
				anim_controller->addAnimation("iddle", &sdlutils().images().at("fondo"), 1, 1, 1, 1, 1);*/
				trigger->addComponent<BoxCollider>(TYPE::KINEMATIC, PLAYER_ATTACK, PLAYER_ATTACK_MASK, true);

				stoppedSawTime = sdlutils().currRealTime();
			}
			else if (CURRENT_STATUS == STATUS::OnCombo && CURRENT_ATTACK == ATTACKS::Attack2) {
				std::cout << "Martillazo! el primero\n";

				//Set player as sawing
				CURRENT_STATUS = STATUS::OnAnimationLock;
				CURRENT_ATTACK = ATTACKS::Attack1;

				//Activate attack animation + sawing on attack
				if (anim_->searchParamValue("hammer_att") != -1)
					anim_->setParamValue("hammer_att", 1);

				//Activate attack trigger
				trigger = entity_->getMngr()->addEntity();
				trigger->addComponent<Transform>(tr_->getPos() + Vector2D(triggerOffSetX, triggerOffSetY),
					Vector2D(0, 0), triggerWidth, triggerHeight, 0.0f);
				/*anim_controller = trigger->addComponent<AnimBlendGraph>();
				anim_controller->addAnimation("iddle", &sdlutils().images().at("fondo"), 1, 1, 1, 1, 1);*/
				trigger->addComponent<BoxCollider>(TYPE::KINEMATIC, PLAYER_ATTACK, PLAYER_ATTACK_MASK, true);

				//Time control variables
				stoppedSawTime = sdlutils().currRealTime();
			}
		}
	}

	//Check out of input cases
	if (CURRENT_STATUS == STATUS::OnAnimationLock && stoppedSawTime + animationLockTime < sdlutils().currRealTime()) {
		//Deactivate animation lock
		std::cout << "STOPPED ANIMATION\n";
		CURRENT_STATUS = STATUS::OnCombo;
		comboActivationTime = sdlutils().currRealTime();
		if (trigger != nullptr) {
			trigger->setActive(false);
			trigger = nullptr;
		}

		if (anim_->searchParamValue("hammer_att") != -1)
			anim_->setParamValue("hammer_att", 0);
	}
	else if (CURRENT_STATUS == STATUS::OnCombo && comboActivationTime + maxComboPanningTime < sdlutils().currRealTime()) {
		//Deactivate combo availability
		std::cout << "STOPPED COMBO\n";
		CURRENT_STATUS = STATUS::Iddle;
		CURRENT_ATTACK = ATTACKS::NotAttacking;
	}

	//Updating the trigger's position
	if (trigger != nullptr) {
		trigger->getComponent<BoxCollider>()->getBody()->SetTransform(b2Vec2((tr_->getPos().getX() + triggerOffSetX) / sdlutils().getPPM(),
			(tr_->getPos().getY() + triggerOffSetY) / sdlutils().getPPM()), 0.0f);
	}
}