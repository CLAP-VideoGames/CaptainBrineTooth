#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "ContactDamage.h"
#include "DisableOnExit.h"
#include "DestroyOnCollision.h"
#include "GetStuckOnWall.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Manager.h"
#include "../game/CollisionLayers.h"
#include "AnimBlendGraph.h"
#include <cmath>

using namespace ColLayers;

class FishlerController : public Component
{
	struct Timer {
		float maxTime;
		float currentTime;
	};

public:
	FishlerController() {};
	virtual ~FishlerController();
	void init() override;
	void update() override;

	void creaTrigger();
	void shoot();

	void shootSpike();

	void changePhase();

protected:
	enum PHASE {Phase1, Phase2};
	enum MOVEMENT_STATE {Iddle, PhaseChange, AttackTelegraph, Attacking, Walking};
	enum ATTACKS {NotAttacking, RushAttack, Shoot, Spikes, Tentacles};

	PHASE currentPhase;
	MOVEMENT_STATE currentMovement;
	ATTACKS currentAttack;

	Transform* tr_;
	BoxCollider* collider_;
	AnimBlendGraph* anim_;
	Entity* trigger;

	float triggerWidth;
	float triggerHeight;
	float triggerOffSetX;
	float triggerOffSetY;

	Transform* playertr;

	float walkingSpeed;
	float rushSpeed;
	float bulletVelocity;
	float spikeVelocity;

	bool changePhaseRequest;

#pragma region Timers
	//Main loop timers
	Timer movementTimer;
	Timer attackTelegraphTimer;
	Timer attackTimer;
	Timer phaseChangeTimer;

	//Attack times
	float rushAttackTelegraphTime;
	float rushAttackTime;

	float shootAttackTelegraphTime;
	float shootAttackTime;

	float spikesAttackTelegraphTime;
	float spikeAttackTime;

#pragma endregion
};