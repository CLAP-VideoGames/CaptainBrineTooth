#include "Chainsaw.h"

#include <iostream>
#include "../CollisionLayers.h"
#include "AnimBlendGraph.h"

using namespace ColLayers;

void Chainsaw::init() {
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);
	anim_ = entity_->getComponent<AnimBlendGraph>();
}

void Chainsaw::update() {
	if (ih().keyDownEvent()) {
		if (ih().isKeyDown(SDL_SCANCODE_E)) {

			//Player not attacking or in combo
			if (CURRENT_STATUS == STATUS::Iddle && stoppedAttackingTime + timeBeforeNextAttackStarts < sdlutils().currRealTime()) {
				std::cout << "Attack 1 Initiated\n";

				//Set player as sawing
				CURRENT_STATUS = STATUS::OnAnimationLock;
				CURRENT_ATTACK = ATTACKS::Attack1;

				//Activate attack animation + sawing on attack
				if(anim_->searchParamValue("chainsaw_att") != -1)
					anim_->setParamValue("chainsaw_att", 1);

				//Activate attack trigger
				trigger = entity_->getMngr()->addEntity();
				trigger->addComponent<Transform>(tr_->getPos() + Vector2D(triggerOffSetX, triggerOffSetY),
					Vector2D(0, 0), triggerWidth, triggerHeight, 0.0f);
				anim_controller = trigger->addComponent<AnimBlendGraph>();
				anim_controller->addAnimation("iddle", &sdlutils().images().at("fondo"), 1, 1, 1, 1, 1);
				trigger->addComponent<BoxCollider>(TYPE::KINEMATIC, PLAYER_ATTACK, PLAYER_ATTACK_MASK, true);
				
				//Time control variables
				stoppedSawTime = sdlutils().currRealTime();
			}
			else if (CURRENT_STATUS == STATUS::OnCombo) {
				switch (CURRENT_ATTACK)
				{
				case ATTACKS::Attack1:
					std::cout << "Attack 2 Initiated\n";
					CURRENT_STATUS = STATUS::OnAnimationLock;
					CURRENT_ATTACK = ATTACKS::Attack2;

					//
					if (anim_->searchParamValue("chainsaw_att") != -1)
						anim_->setParamValue("chainsaw_att", 2);

					trigger = entity_->getMngr()->addEntity();
					trigger->addComponent<Transform>(tr_->getPos() + Vector2D(triggerOffSetX, triggerOffSetY),
						Vector2D(0, 0), triggerWidth, triggerHeight, 0.0f);
					anim_controller = trigger->addComponent<AnimBlendGraph>();
					anim_controller->addAnimation("iddle", &sdlutils().images().at("fondo"), 1, 1, 1, 1, 1);
					trigger->addComponent<BoxCollider>(TYPE::KINEMATIC, PLAYER_ATTACK, PLAYER_ATTACK_MASK, true);

					stoppedSawTime = sdlutils().currRealTime();
					break;
				case ATTACKS::Attack2:
					std::cout << "Attack 3 Initiated\n";
					CURRENT_STATUS = STATUS::Sawing;
					CURRENT_ATTACK = ATTACKS::Attack3;

					if (anim_->searchParamValue("chainsaw_att") != -1)
						anim_->setParamValue("chainsaw_att", 3);

					sawActivationTime = sdlutils().currRealTime();
					break;
				default:
					break;
				}
			}
		}
	}

	//Check out of input cases
	if (CURRENT_STATUS == STATUS::Sawing && sawActivationTime + maxHoldTime < sdlutils().currRealTime()) {
		//Deactivate chainsaw
		std::cout << "STOPPED SAWING\n";
		CURRENT_STATUS = STATUS::OnAnimationLock;

		stoppedSawTime = sdlutils().currRealTime();

		if (anim_->searchParamValue("chainsaw_att") != -1)
			anim_->setParamValue("chainsaw_att", 0);
	}
	else if (CURRENT_STATUS == STATUS::OnAnimationLock && stoppedSawTime + animationLockTime < sdlutils().currRealTime()) {
		//Deactivate animation lock
		std::cout << "STOPPED ANIMATION\n";
		CURRENT_STATUS = STATUS::OnCombo;

		if (trigger != nullptr) {
			trigger->setActive(false);
			trigger = nullptr;
		}

		comboActivationTime = sdlutils().currRealTime();
	}
	else if (CURRENT_STATUS == STATUS::OnCombo && comboActivationTime + maxComboPanningTime < sdlutils().currRealTime()) {
		//Deactivate combo availability
		std::cout << "STOPPED COMBO\n";
		CURRENT_STATUS = STATUS::Iddle;
		CURRENT_ATTACK = ATTACKS::NotAttacking;

		stoppedAttackingTime = sdlutils().currRealTime();

		if (anim_->searchParamValue("chainsaw_att") != -1)
			anim_->setParamValue("chainsaw_att", 0);
	}

	//Updating the trigger's position
	if (trigger != nullptr) {
		trigger->getComponent<BoxCollider>()->getBody()->SetTransform(b2Vec2((tr_->getPos().getX() + triggerOffSetX) / sdlutils().getPPM(),
			(tr_->getPos().getY() + triggerOffSetY) / sdlutils().getPPM()), 0.0f);
	}

	if (currentlyStabbing && stabActivationTime + stabTriggerTime < sdlutils().currRealTime()) {
		if (trigger != nullptr) {
			trigger->setActive(false);
			trigger = nullptr;
		}
		currentlyStabbing = false;
	}

	//Comprobamos si hay que spawnear una estocada
	if (CURRENT_STATUS == Sawing && stabActivationTime + timeBetweenStabs < sdlutils().currRealTime()) {
		std::cout << "Saw\n";

		trigger = entity_->getMngr()->addEntity();
		trigger->addComponent<Transform>(tr_->getPos() + Vector2D(triggerOffSetX, triggerOffSetY),
			Vector2D(0, 0), triggerWidth, triggerHeight, 0.0f);
		anim_controller = trigger->addComponent<AnimBlendGraph>();
		anim_controller->addAnimation("iddle", &sdlutils().images().at("fondo"), 1, 1, 1, 1, 1);
		trigger->addComponent<BoxCollider>(TYPE::KINEMATIC, PLAYER_ATTACK, PLAYER_ATTACK_MASK, true);

		stabActivationTime = sdlutils().currRealTime();
		currentlyStabbing = true;
	}
}