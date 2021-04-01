#include "Sword.h"

#include <iostream>
#include "../CollisionLayers.h"

using namespace ColLayers;

void Sword::init() {
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

void Sword::update() {
	if (ih().keyDownEvent()) {
		if (ih().isKeyDown(SDL_SCANCODE_E)) {

			//Player not attacking or in combo
			if (CURRENT_STATUS == STATUS::Iddle) {
				std::cout << "Chainsaw attack\n";

				//Set player as sawing
				CURRENT_STATUS = STATUS::Sawing;
				CURRENT_ATTACK = ATTACKS::Attack1;

				//Activate attack animation + sawing on attack


				//Activate attack trigger
				trigger = entity_->getMngr()->addEntity();
				trigger->addComponent<Transform>(tr_->getPos() + Vector2D(triggerOffSetX, triggerOffSetY),
					Vector2D(0, 0), triggerWidth, triggerHeight, 0.0f);
				anim_controller = trigger->addComponent<AnimBlendGraph>();
				anim_controller->addAnimation("iddle", &sdlutils().images().at("fondo"), 1, 1, 1, 1, 1);
				trigger->addComponent<BoxCollider>(TYPE::KINEMATIC, PLAYER_ATTACK, PLAYER_ATTACK_MASK, true);

				//Time control variables
				sawActivationTime = sdlutils().currRealTime();
			}
			else if (CURRENT_STATUS == STATUS::OnCombo) {
				switch (CURRENT_ATTACK)
				{
				case ATTACKS::Attack1:
					std::cout << "Attack 2 Initiated\n";
					CURRENT_STATUS = STATUS::Sawing;
					CURRENT_ATTACK = ATTACKS::Attack2;

					trigger = entity_->getMngr()->addEntity();
					trigger->addComponent<Transform>(tr_->getPos() + Vector2D(triggerOffSetX, triggerOffSetY),
						Vector2D(0, 0), triggerWidth, triggerHeight, 0.0f);
					anim_controller = trigger->addComponent<AnimBlendGraph>();
					anim_controller->addAnimation("iddle", &sdlutils().images().at("fondo"), 1, 1, 1, 1, 1);
					trigger->addComponent<BoxCollider>(TYPE::KINEMATIC, PLAYER_ATTACK, PLAYER_ATTACK_MASK, true);

					sawActivationTime = sdlutils().currRealTime();
					break;
				case ATTACKS::Attack2:
					std::cout << "Attack 3 Initiated\n";
					CURRENT_STATUS = STATUS::Sawing;
					CURRENT_ATTACK = ATTACKS::Attack3;

					trigger = entity_->getMngr()->addEntity();
					trigger->addComponent<Transform>(tr_->getPos() + Vector2D(triggerOffSetX, triggerOffSetY),
						Vector2D(0, 0), triggerWidth, triggerHeight, 0.0f);
					anim_controller = trigger->addComponent<AnimBlendGraph>();
					anim_controller->addAnimation("iddle", &sdlutils().images().at("fondo"), 1, 1, 1, 1, 1);
					trigger->addComponent<BoxCollider>(TYPE::KINEMATIC, PLAYER_ATTACK, PLAYER_ATTACK_MASK, true);

					sawActivationTime = sdlutils().currRealTime();
					break;
				default:
					break;
				}
			}
		}
	}

	//Button release checkers
	if (ih().keyUpEvent()) {
		if (ih().isKeyUp(SDL_SCANCODE_E)) {
			if (CURRENT_STATUS == STATUS::Sawing) {
				std::cout << "STOPPED SAWING\n";
				CURRENT_STATUS = STATUS::OnAnimationLock;

				trigger->setActive(false);
				trigger = nullptr;

				stoppedSawTime = sdlutils().currRealTime();
			}
		}
	}

	//Check out of input cases
	if (CURRENT_STATUS == STATUS::Sawing && sawActivationTime + maxHoldTime < sdlutils().currRealTime()) {
		//Deactivate chainsaw
		std::cout << "STOPPED SAWING\n";
		CURRENT_STATUS = STATUS::OnAnimationLock;

		trigger->setActive(false);
		trigger = nullptr;

		stoppedSawTime = sdlutils().currRealTime();
	}
	else if (CURRENT_STATUS == STATUS::OnAnimationLock && stoppedSawTime + animationLockTime < sdlutils().currRealTime()) {
		//Deactivate animation lock
		std::cout << "STOPPED ANIMATION\n";
		CURRENT_STATUS = STATUS::OnCombo;
		comboActivationTime = sdlutils().currRealTime();
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

	//Local debugging
	if (CURRENT_STATUS == STATUS::Sawing) {
		std::cout << "SAWING\n";
	}
}