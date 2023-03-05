#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"
#include "Transform.h"
#include "../components/BoxCollider.h"

class InkMovement : public Component {
public:
	InkMovement(bool lookingLeft);
	virtual ~InkMovement() {
	}

	void init() override;

	void update() override;

private:
	bool lookingLeft_;
	float initialVelocity;
	float currentVel;
	int dir;
	float deacceleration;

	BoxCollider* box;
};