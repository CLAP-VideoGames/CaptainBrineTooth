#pragma once
#include "../ecs/Entity.h"
#include "../ecs/Component.h"
#include "../sdlutils/InputHandler.h"
#include "Transform.h"
#include "AnimBlendGraph.h"
#include "BoxCollider.h"
#include "MapCollider.h"
#include "TriggerCollider.h"

#include "../ecs/SoundManager.h"
using namespace ColLayers;

class PlayerController : public Component {
public:																
PlayerController(const float & speed = 3.0f, const float& forceJ = 0.9f , const float& dashS = 1.0f):
															//falso				//falso
		tr_(nullptr), speed_(speed), forceJump_(forceJ), isOnFloor(false), isOnAir(false), dashSpeed(dashS), isDashing(false), canDash(true){}

	void init() override;
	//Necesitamos un getter de la velocidad del collider
	void update() override;
	void initEntityColliders();
	void createJointTrigger(Entity* trigger);
	void playerReceiveInput(bool state);

	void Paralize() { paralized = true; };

	static void OnTriggerEnter(b2Contact* contact);
	static void OnTriggerExit(b2Contact* contact);

	void isOnGround();
	const bool& isPlayerDashing();

private:
	Transform* tr_ = nullptr;
	BoxCollider* collider_ = nullptr;
	AnimBlendGraph* animController_ = nullptr;
	SoundManager* snd = nullptr;
	TriggerCollider* trigger_ = nullptr;
	bool receiveInput;

	int time = 1500, dashCoolDown = 1000, lasTimeDashed, lastTimeJumped,timer;
	float speed_, forceJump_, maxSpeed, dashSpeed, gravity;
	//Flags
	bool isOnFloor, isOnAir, isDashing, canDash, moveLeft, moveRight, paralized, jumpkey_pressed;
	
};