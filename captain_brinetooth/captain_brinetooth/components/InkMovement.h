#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"
#include "Transform.h"
#include "../components/BoxCollider.h"

class InkMovement : public Component {
public:
	InkMovement(bool lookingLeft) :
		lookingLeft_(lookingLeft)
	{
		initialVelocity = 10;
	}
	virtual ~InkMovement() {
	}

	void init() override;

private:
	bool lookingLeft_;
	float initialVelocity;
	Vector2D vel;
	float dir;

	BoxCollider* box;
};