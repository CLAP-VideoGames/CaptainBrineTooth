#pragma once


#include <SDL.h>
#include <cassert>
#include "Transform.h"
#include "BoxCollider.h"
#include "../ecs/Entity.h"
#include "../ecs/Component.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Manager.h"
#include "AnimBlendGraph.h"
#include "WeaponDamageDetection.h"
#include "PlayerController.h"


class Sword : public Component {
public:
	Sword() :
		tr_(nullptr), trigger(nullptr) {
	}
	virtual ~Sword() {
		if (anim_->getParamIndex("sword_att") != -1)
			anim_->setParamValue("sword_att", 0);
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
	float animationLockTime = 666;		//Counted after one attack is stopped to avoid animation clipping
	float maxComboPanningTime = 500;	//Counted after animation lock is released
	float timeBetweenStabs = 333;
	float stabTriggerTime = 200;
	float timeBeforeNextAttackStarts = 666;
	float timeLoopAnimationTransition = 500;

	//Variables related to ChainsawTriggerPosition
	int triggerOffSetX = -90;
	int triggerOffSetY = -30;
	int triggerWidth = 150;
	int triggerHeight = 130;

	int thirdTriggerOffSetX = -100;
	int thirdTriggerOffSetY = 5;
	int thirdTriggerWidth = 130;
	int thirdTriggerHeight = 50;

	Entity* trigger;
};