#pragma once


#include <SDL.h>
#include <cassert>
#include "Transform.h"
#include "../ecs/Entity.h"
#include "../ecs/Component.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"

enum STATUS {Iddle, Sawing, OnAnimationLock, OnCombo};
enum ATTACKS {NotAttacking, Attack1, Attack2, Attack3};

class Chainsaw : public Component {
public:
	Chainsaw() :
		tr_(nullptr) {
	}
	virtual ~Chainsaw() {
	}

	void init() override;

	void update() override;

private:
	Transform* tr_;

	//Local control variables (don't touch or change them except in code)
	int CURRENT_STATUS = STATUS::Iddle;
	int CURRENT_ATTACK = ATTACKS::NotAttacking;
	float sawActivationTime;
	float stoppedSawTime;
	float comboActivationTime;

	//Cooldown variables (should be able to be changed as you see fit)
	float maxHoldTime = 3000;	//Be careful with units
	float animationLockTime = 1000;		//Counted after one attack is stopped to avoid animation clipping
	float maxComboPanningTime = 2000;	//Counted after animation lock is released

}
;