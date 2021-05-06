#pragma once

#include "Transform.h"
#include "../ecs/Component.h"
#include "../sdlutils/InputHandler.h"
#include "./BoxCollider.h"
#include "../ecs/Entity.h"
class PescaController : public Component {
public:

	PescaController(int w);
	void init() override;
	void update() override;

private:
	BoxCollider* collider_ = nullptr;
	bool move = false;
	int s_width;
};