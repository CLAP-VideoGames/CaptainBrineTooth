#include "DisableOnExit.h"

DisableOnExit::DisableOnExit(float time) : timer_(sdlutils().currRealTime()), timeBeforeDestroy(time) {};

void DisableOnExit::update() {
	if (timer_ + timeBeforeDestroy < sdlutils().currRealTime()) {
		entity_->setActive(false);
		timer_ = sdlutils().currRealTime();
	}
}