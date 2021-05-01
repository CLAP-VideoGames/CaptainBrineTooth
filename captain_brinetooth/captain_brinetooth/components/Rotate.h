// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <SDL.h>
#include <cassert>

#include "../ecs/Component.h"
#include "../sdlutils/InputHandler.h"
#include "../ecs/Entity.h"
#include "Transform.h"

class Rotate : public Component {
public:
	Rotate();
	virtual ~Rotate();

	inline void setSpeed(float speed) {
		speed_ = speed;
	}

	void init() override;

	void update() override;

private:
	Transform* tr_;
	float speed_;
};

