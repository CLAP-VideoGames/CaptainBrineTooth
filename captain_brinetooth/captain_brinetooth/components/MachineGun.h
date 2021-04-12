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
#include "DisableOnExit.h"
#include "DestroyOnCollision.h"


class MachineGun : public Component {
public:
	MachineGun() :
		tr_(nullptr) {
	}
	virtual ~MachineGun() {
	}

	void init() override;

	void update() override;

	void shoot();
private:
	enum STATUS {Iddle, Shooting, Reloading};

	Transform* tr_;
	AnimBlendGraph* anim_controller;

	//Local control variables (don't touch or change them except in code)
	int CURRENT_STATUS = STATUS::Iddle;
	float startedReloading;
	float shotActivationTime;
	bool currentlyShooting = false;
	float overheat;
	float overheatSpikeTime;
	float decreaseOverheatSpikeTime;
	float bulletVelocity;
	AnimBlendGraph* anim_;

	//Cooldown variables (should be able to be changed as you see fit)
	float reloadingTime = 2000;		//Counted after one attack is stopped to avoid animation clipping
	float timeBetweenShots = 333;
	float timeBetweenOverheatSpikes = 100;
	float maxOverheat = 30;
};