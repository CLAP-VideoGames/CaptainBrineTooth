#include "Sword.h"

#include <iostream>
#include "../game/CollisionLayers.h"
#include "AnimBlendGraph.h"

using namespace ColLayers;

void Sword::init() {
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);
	anim_ = entity_->getComponent<AnimBlendGraph>();
}

void Sword::update() {
	if (!entity_->getComponent<PlayerController>()->isPlayerDashing()) {
		if (ih().mouseButtonEvent() || ih().keyUpEvent()) {
			if (ih().getMouseButtonState(InputHandler::MOUSEBUTTON::LEFT) || ih().isKeyDown(SDL_CONTROLLER_BUTTON_X)) {

				//Player not attacking or in combo
				if (CURRENT_STATUS == STATUS::Iddle && stoppedAttackingTime + timeBeforeNextAttackStarts < sdlutils().currRealTime()) {
					std::cout << "Sword attack\n";

					//Set player as sawing
					CURRENT_STATUS = STATUS::OnAnimationLock;
					CURRENT_ATTACK = ATTACKS::Attack1;

					if (!currentlyStabbing)
					{
						entity_->getMngr()->getSoundMngr()->playSoundEffect("sword_whoosh0", 0);
					}

					//Activate attack animation + sawing on attack
					if (anim_->getParamIndex("sword_att") != -1)
						anim_->setParamValue("sword_att", 1);

					//
					creaTrigger(30);

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
						if (!currentlyStabbing)
						{
							entity_->getMngr()->getSoundMngr()->playSoundEffect("sword_whoosh1", 0);
						}
						if (anim_->getParamIndex("sword_att") != -1)
							anim_->setParamValue("sword_att", 2);

						//
						creaTrigger(30);

						stoppedSawTime = sdlutils().currRealTime();
						break;
					case ATTACKS::Attack2:
						std::cout << "Attack 3 Initiated\n";
						CURRENT_STATUS = STATUS::Sawing;
						CURRENT_ATTACK = ATTACKS::Attack3;

						if (anim_->getParamIndex("sword_att") != -1)
							anim_->setParamValue("sword_att", 3);

						sawActivationTime = sdlutils().currRealTime();

						toLoop = true;
						currentLoopAnimationTime = sdlutils().currRealTime();

						break;
					case ATTACKS::Attack3:
						std::cout << "Sword attack\n";

						//Set player as sawing
						CURRENT_STATUS = STATUS::OnAnimationLock;
						CURRENT_ATTACK = ATTACKS::Attack1;

						if (!currentlyStabbing)
						{
							entity_->getMngr()->getSoundMngr()->playSoundEffect("sword_whoosh0", 0);
						}

						//Activate attack animation + sawing on attack
						if (anim_->getParamIndex("sword_att") != -1)
							anim_->setParamValue("sword_att", 1);

						
						creaTrigger(4);

						//Time control variables
						stoppedSawTime = sdlutils().currRealTime();
						break;
					default:
						break;
					}
				}
			}
		}
		//Transicion de la animacion de preparar tercer ataque a la realización del bucle
		if (toLoop && CURRENT_STATUS == STATUS::Sawing && currentLoopAnimationTime + timeLoopAnimationTransition < sdlutils().currRealTime()) {
			anim_->setParamValue("sword_att", 4);
			toLoop = false;
		}

		//Check out of input cases
		if (CURRENT_STATUS == STATUS::Sawing && sawActivationTime + maxHoldTime < sdlutils().currRealTime()) {
			//Deactivate chainsaw
			std::cout << "STOPPED STABBING\n";
			CURRENT_STATUS = STATUS::OnAnimationLock;

			if (anim_->getParamIndex("sword_att") != -1)
				anim_->setParamValue("sword_att", 0);

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

			if (anim_->getParamIndex("sword_att") != -1)
				anim_->setParamValue("sword_att", 0);
		}

		//Updating the trigger's position
		if (trigger != nullptr) {
			if (CURRENT_ATTACK == ATTACKS::Attack3) {
				if (anim_->isFlipX()) trigger->getComponent<BoxCollider>()->getBody()->SetTransform(b2Vec2((tr_->getPos().getX() + (-thirdTriggerOffSetX)) / sdlutils().getPPM(),
					(tr_->getPos().getY() + thirdTriggerOffSetY) / sdlutils().getPPM()), 0.0f);
				else trigger->getComponent<BoxCollider>()->getBody()->SetTransform(b2Vec2((tr_->getPos().getX() + thirdTriggerOffSetX) / sdlutils().getPPM(),
					(tr_->getPos().getY() + thirdTriggerOffSetY) / sdlutils().getPPM()), 0.0f);
			}
			else {
				if (anim_->isFlipX()) trigger->getComponent<BoxCollider>()->getBody()->SetTransform(b2Vec2((tr_->getPos().getX() + (-triggerOffSetX)) / sdlutils().getPPM(),
					(tr_->getPos().getY() + triggerOffSetY) / sdlutils().getPPM()), 0.0f);
				else trigger->getComponent<BoxCollider>()->getBody()->SetTransform(b2Vec2((tr_->getPos().getX() + triggerOffSetX) / sdlutils().getPPM(),
					(tr_->getPos().getY() + triggerOffSetY) / sdlutils().getPPM()), 0.0f);
			}
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
			std::cout << "Stab\n";

			//
			creaTrigger(20);

			entity_->getMngr()->getSoundMngr()->playSoundEffect("estocada_espada", 0);

			stabActivationTime = sdlutils().currRealTime();
			currentlyStabbing = true;
		}
	}
	else {
		if (CURRENT_STATUS != STATUS::Iddle) {
			if (trigger != nullptr) {
				trigger->setActive(false);
				trigger = nullptr;
			}
		CURRENT_STATUS = STATUS::Iddle;
		CURRENT_ATTACK = ATTACKS::NotAttacking;
		}
	}
}

void Sword::creaTrigger(int damage) {
	trigger = entity_->getMngr()->addEntity();
	if (CURRENT_ATTACK == ATTACKS::Attack3) {
		if (anim_->isFlipX()) trigger->addComponent<Transform>(tr_->getPos() + Vector2D(-thirdTriggerOffSetX, thirdTriggerOffSetY),
			Vector2D(0, 0), thirdTriggerWidth * entity_->getComponent<Transform>()->getW()/100.0f, thirdTriggerHeight, 0.0f);
		else trigger->addComponent<Transform>(tr_->getPos() + Vector2D(thirdTriggerOffSetX, thirdTriggerOffSetY),
			Vector2D(0, 0), thirdTriggerWidth * entity_->getComponent<Transform>()->getW() / 100.0f, thirdTriggerHeight, 0.0f);
	}
	else {
		if (anim_->isFlipX()) trigger->addComponent<Transform>(tr_->getPos() + Vector2D(-triggerOffSetX, triggerOffSetY),
			Vector2D(0, 0), triggerWidth * entity_->getComponent<Transform>()->getW() / 100.0f, triggerHeight, 0.0f);
		else trigger->addComponent<Transform>(tr_->getPos() + Vector2D(triggerOffSetX, triggerOffSetY),
			Vector2D(0, 0), triggerWidth * entity_->getComponent<Transform>()->getW() / 100.0f, triggerHeight, 0.0f);
	}

	trigger->addComponent<BoxCollider>(TYPE::KINEMATIC, PLAYER_ATTACK, PLAYER_ATTACK_MASK, true);
	trigger->addComponent<WeaponDamageDetection>(damage);
}