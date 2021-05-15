#include "Fists.h"

#include <iostream>
#include "../game/CollisionLayers.h"
#include "AnimBlendGraph.h"

using namespace ColLayers;

void Fists::init() {
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);
	anim_ = entity_->getComponent<AnimBlendGraph>();
}

void Fists::update() {
	if (!entity_->getComponent<PlayerController>()->isPlayerDashing()) {
		if (ih().mouseButtonEvent() || ih().keyUpEvent()) {
			if (ih().getMouseButtonState(InputHandler::MOUSEBUTTON::LEFT) || ih().isKeyDown(SDL_CONTROLLER_BUTTON_X)) {

				//Player not attacking or in combo
				if (CURRENT_STATUS == STATUS::Iddle && stoppedAttackingTime + timeBeforeNextAttackStarts < sdlutils().currRealTime()) {
					std::cout << "Fist attack\n";

					//Set player as sawing
					CURRENT_STATUS = STATUS::OnAnimationLock;
					CURRENT_ATTACK = ATTACKS::Attack1;
					entity_->getMngr()->getSoundMngr()->playSoundEffect("normal_punch1", 0);
					//Activate attack animation + sawing on attack
					if (anim_->getParamIndex("fist_att") != -1)
						anim_->setParamValue("fist_att", 1);

					isAttacking = true;
					creaTrigger(20);

					//Time control variables
					stoppedSawTime = sdlutils().currRealTime();
				}
				else if (CURRENT_STATUS == STATUS::OnCombo) {
					switch (CURRENT_ATTACK)
					{
					case ATTACKS::Attack1:
						std::cout << "Attack 2 Initiated\n";
						CURRENT_STATUS = STATUS::Sawing;
						CURRENT_ATTACK = ATTACKS::Attack2;
						entity_->getMngr()->getSoundMngr()->playSoundEffect("normal_punch2", 0);

						isAttacking = true;
						if (anim_->getParamIndex("fist_att") != -1)
							anim_->setParamValue("fist_att", 2);

						sawActivationTime = sdlutils().currRealTime();
						break;

					case ATTACKS::Attack2:
						std::cout << "Attack 3 Initiated\n";
						CURRENT_STATUS = STATUS::OnAnimationLock;
						CURRENT_ATTACK = ATTACKS::Attack3;
						entity_->getMngr()->getSoundMngr()->playSoundEffect("normal_punch3", 0);

						if (anim_->getParamIndex("fist_att") != -1)
							anim_->setParamValue("fist_att", 3);

						isAttacking = true;
						creaTrigger(40);

						stoppedSawTime = sdlutils().currRealTime();
						break;
					case ATTACKS::Attack3:
						std::cout << "Fist attack\n";

						//Set player as sawing
						CURRENT_STATUS = STATUS::OnAnimationLock;
						CURRENT_ATTACK = ATTACKS::Attack1;

						entity_->getMngr()->getSoundMngr()->playSoundEffect("normal_punch1", 0);


						//Activate attack animation + sawing on attack
						if (anim_->getParamIndex("fist_att") != -1)
							anim_->setParamValue("fist_att", 1);

						isAttacking = true;
						creaTrigger(20);

						//Time control variables
						stoppedSawTime = sdlutils().currRealTime();
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
			std::cout << "STOPPED STABBING\n";
			CURRENT_STATUS = STATUS::OnAnimationLock;

			isAttacking = false;
			if (anim_->getParamIndex("fist_att") != -1)
				anim_->setParamValue("fist_att", 0);

			stoppedSawTime = sdlutils().currRealTime();
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

			isAttacking = false;
			if (anim_->getParamIndex("fist_att") != -1)
				anim_->setParamValue("fist_att", 0);
		}

		//Updating the trigger's position
		if (trigger != nullptr) {
			if (anim_->isFlipX()) trigger->getComponent<BoxCollider>()->getBody()->SetTransform(b2Vec2((tr_->getPos().getX() + (-triggerOffSetX)) / sdlutils().getPPM(),
				(tr_->getPos().getY() + triggerOffSetY) / sdlutils().getPPM()), 0.0f);
			else trigger->getComponent<BoxCollider>()->getBody()->SetTransform(b2Vec2((tr_->getPos().getX() + triggerOffSetX) / sdlutils().getPPM(),
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
			std::cout << "Fist Punch\n";

			
			creaTrigger(20);

			stabActivationTime = sdlutils().currRealTime();
			currentlyStabbing = true;
		}
	}
	else {
		if (CURRENT_STATUS != STATUS::Iddle) {
			isAttacking = false;
			if (trigger != nullptr) {
				trigger->setActive(false);
				trigger = nullptr;
			}
			CURRENT_STATUS = STATUS::Iddle;
			CURRENT_ATTACK = ATTACKS::NotAttacking;
		}
	}



}

void Fists::creaTrigger(int damage) {
	//Activate attack trigger
	trigger = entity_->getMngr()->addEntity();
	if (anim_->isFlipX()) trigger->addComponent<Transform>(tr_->getPos() + Vector2D(-triggerOffSetX, triggerOffSetY),
		Vector2D(0, 0), triggerWidth * entity_->getComponent<Transform>()->getW() / 100.0f, triggerHeight, 0.0f);
	else trigger->addComponent<Transform>(tr_->getPos() + Vector2D(triggerOffSetX, triggerOffSetY),
		Vector2D(0, 0), triggerWidth * entity_->getComponent<Transform>()->getW() / 100.0f, triggerHeight, 0.0f);

	trigger->addComponent<BoxCollider>(TYPE::KINEMATIC, PLAYER_ATTACK, PLAYER_ATTACK_MASK, true);
	trigger->addComponent<WeaponDamageDetection>(damage);
}