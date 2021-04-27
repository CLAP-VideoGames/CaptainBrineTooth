#pragma once


#include <SDL.h>
#include <cassert>
#include "Transform.h"
#include "../ecs/Entity.h"
#include "../ecs/Component.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Manager.h"
#include "AnimBlendGraph.h"
#include "WeaponDamageDetection.h"
#include "PlayerController.h"


class Chainsaw : public Component {
public:
	Chainsaw() :
		tr_(nullptr) {
	}
	virtual ~Chainsaw() {
		if (anim_->getParamIndex("chainsaw_att") != -1)
			anim_->setParamValue("chainsaw_att", 0);
		if (trigger != nullptr) {
			trigger->setActive(false);
			trigger = nullptr;
		}
	}

	void init() override;

	void update() override;
	void creaTrigger(int damage);

private:
	enum STATUS { Iddle, Sawing, OnAnimationLock, OnCombo };
	enum ATTACKS { NotAttacking, Attack1, Attack2, Attack3 };

	Transform* tr_;
	AnimBlendGraph* anim_controller;

	//Local control variables (don't touch or change them except in code)
	int CURRENT_STATUS = STATUS::Iddle;
	int CURRENT_ATTACK = ATTACKS::NotAttacking;
	float sawActivationTime;
	float stoppedSawTime;
	float comboActivationTime;
	float stabActivationTime;
	bool currentlyStabbing = false;
	float stoppedAttackingTime = 0;
	float currentLoopAnimationTime;
	bool toLoop;
	AnimBlendGraph* anim_; 

	//Cooldown variables (should be able to be changed as you see fit)
	float maxHoldTime = 3000;	//Time player is sawing
	float animationLockTime = 1000;		//Counted after one attack is stopped to avoid animation clipping
	float maxComboPanningTime = 500;	//Counted after animation lock is released
	float timeBetweenStabs = 1000;
	float stabTriggerTime = 500;
	float timeBeforeNextAttackStarts = 1000;
	float timeLoopAnimationTransition = 500;

	//Variables related to ChainsawTriggerPosition
	int triggerOffSetX = -60;
	int triggerOffSetY = -30;
	int triggerWidth = 120;
	int triggerHeight = 130;
	
	int thirdTriggerOffSetX = -90;
	int thirdTriggerOffSetY = 20;
	int thirdTriggerWidth = 120;
	int thirdTriggerHeight = 50;

	Entity* trigger;
};