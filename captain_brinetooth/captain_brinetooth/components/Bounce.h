// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "Transform.h"
#include "../sdlutils/SDLUtils.h"

class Bounce: public Component {
public:
	Bounce();
	virtual ~Bounce();
	void init() override;

	void update() override;

private:
	Transform *tr_;
};

