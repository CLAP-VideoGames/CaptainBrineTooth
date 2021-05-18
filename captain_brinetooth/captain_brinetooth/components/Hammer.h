#pragma once


#include <SDL.h>
#include <cassert>
#include "Transform.h"
#include "BoxCollider.h"
#include "../ecs/Entity.h"
#include "WeaponComponent.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Manager.h"
#include "AnimBlendGraph.h"
#include "WeaponDamageDetection.h"
#include "PlayerController.h"
#include "Player_Health.h"


class Hammer : public WeaponComponent {
public:
	Hammer() :
		tr_(nullptr) {
	}
	virtual ~Hammer() {
		if (anim_->getParamIndex("hammer_att") != -1)
			anim_->setParamValue("hammer_att", 0);
		if (trigger != nullptr) {
			trigger->setActive(false);
			trigger = nullptr;
		}
	}

	void init() override;

	void update() override;
	void creaTrigger(int damage);

private:
	enum STATUS { Iddle, OnAnimationLock, OnCombo };
	enum ATTACKS { NotAttacking, Attack1, Attack2 };

	Transform* tr_;
	AnimBlendGraph* anim_controller;

	//Local control variables (don't touch or change them except in code)
	int CURRENT_STATUS = STATUS::Iddle;
	int CURRENT_ATTACK = ATTACKS::NotAttacking;
	float sawActivationTime;
	float stoppedSawTime;
	float comboActivationTime;
	AnimBlendGraph* anim_;

	//Cooldown variables (should be able to be changed as you see fit) (not changed)
	float animationLockTime = 700;		//Counted after one attack is stopped to avoid animation clipping
	float maxComboPanningTime = 200;	//Counted after animation lock is released

	//Variables related to Hammer (not changed)
	int triggerOffSetX = -60;
	int triggerOffSetY = -20;
	int triggerWidth = 150;
	int triggerHeight = 130;

	Entity* trigger;
};