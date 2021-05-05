#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Manager.h"
#include "../game/CollisionLayers.h"

using namespace ColLayers;

class FishlerController : public Component
{
	struct Timer {
		float maxTime;
		float currentTime;
	};

public:
	FishlerController();
	virtual ~FishlerController();
	void init() override;
	void update() override;
	void render() override;

	static void playerDetected(b2Contact* contact);
	static void playerLost(b2Contact* contact);
	void playerDetection(bool entered);
	void playerCloseDetection();

protected:
	enum PHASE {Phase1, Phase2};
	enum MOVEMENT_STATE {Iddle, Stunned, Attacking, Walking, Jumping};
	enum ATTACKS {NotAttacking, RushAttack, Bite, Shoot, Spikes};

	PHASE currentPhase;
	MOVEMENT_STATE currentMovement;
	ATTACKS currentAttack;

	Transform* tr_;

	Transform* playertr;
	Entity* playerCloseDetect;
	Vector2D playerCloseSize;
	bool playerIsClose;
#pragma region Timers
	//Initial movement timers
	Timer movementTimer;

#pragma endregion
};