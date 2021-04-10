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


class Crab : public Component {
public:
	Crab() :
		tr_(nullptr) {
	}
	virtual ~Crab() {
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
	AnimBlendGraph* anim_;

	//Cooldown variables (should be able to be changed as you see fit)
	float maxHoldTime = 666;	//Time player is sawing
	float animationLockTime = 666;		//Counted after one attack is stopped to avoid animation clipping
	float maxComboPanningTime = 2000;	//Counted after animation lock is released
	float timeBetweenStabs = 333;
	float stabTriggerTime = 200;
	float timeBeforeNextAttackStarts = 666;

	//Variables related to ChainsawTriggerPosition
	int triggerOffSetX = -60;
	int triggerOffSetY = 100;
	int triggerWidth = 150;
	int triggerHeight = 150;

	Entity* trigger;
};