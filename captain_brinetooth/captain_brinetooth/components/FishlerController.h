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

	static void playerDetected(b2Contact* contact);
	static void playerLost(b2Contact* contact);
	void playerDetection(bool entered);
	void playerCloseDetection();

	void creaTrigger();
	void shoot();

	void createFloorDetection();
	void changeToOnFloor();
	static void floorTouched(b2Contact* contact);

	void shootSpike();

protected:
	enum PHASE {Phase1, Phase2};
	enum MOVEMENT_STATE {Iddle, Stunned, AttackTelegraph, Attacking, Walking};
	enum ATTACKS {NotAttacking, RushAttack, Bite, Shoot, Spikes, Tentacles};

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
	Entity* playerCloseDetect;
	Vector2D playerCloseSize;
	bool playerIsClose;

	Entity* floorDetect;
	bool onFloor;	//Determines when Fishler is on the floor an therefore he is able to jump

	float walkingSpeed;
	float rushSpeed;
	float bulletVelocity;
	float spikeVelocity;

#pragma region Timers
	//Main loop timers
	Timer movementTimer;
	Timer attackTelegraphTimer;
	Timer attackTimer;

	//Attack times
	float rushAttackTelegraphTime;
	float rushAttackTime;

	float biteAttackTelegraphTime;
	float biteAttackTime;

	float shootAttackTelegraphTime;
	float shootAttackTime;

	float spikesAttackTelegraphTime;
	float spikeAttackTime;

#pragma endregion
};