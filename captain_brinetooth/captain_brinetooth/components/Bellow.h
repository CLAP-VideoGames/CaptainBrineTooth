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
#include "DisableOnExit.h"
#include "DestroyOnCollision.h"
#include "InkMovement.h"
#include "../levels/Level0.h"


class Bellow : public WeaponComponent {
public:
	Bellow() :
		tr_(nullptr) {
	}
	virtual ~Bellow() {
		if (anim_->getParamIndex("chainsaw_att") != -1)
			anim_->setParamValue("chainsaw_att", 0);
	}

	void init() override;

	void update() override;

	void shoot();
private:
	enum STATUS { Iddle, Shooting};

	Transform* tr_;
	AnimBlendGraph* anim_controller;

	//Local control variables (don't touch or change them except in code)
	int CURRENT_STATUS = STATUS::Iddle;
	float shotActivationTime;
	AnimBlendGraph* anim_;

	//Cooldown variables (should be able to be changed as you see fit)
	float timeBetweenShots = 700;
};