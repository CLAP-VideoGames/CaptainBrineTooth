#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"

class DisableOnExit : public Component
{
public:
	DisableOnExit();
	virtual ~DisableOnExit() {};

	void update() override;

private:
	float timer_;
};

