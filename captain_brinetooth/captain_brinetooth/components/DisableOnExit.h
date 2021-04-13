#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"

class DisableOnExit : public Component
{
public:
	DisableOnExit() : timer_(sdlutils().currRealTime()) {}
	virtual ~DisableOnExit() {}

	void update() override {
		if (timer_ + 2000 < sdlutils().currRealTime()) {
			entity_->setActive(false);
			timer_ = sdlutils().currRealTime();
		}
	}

private:
	float timer_;
};

