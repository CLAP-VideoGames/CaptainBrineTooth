#include "DisableOnExit.h"

DisableOnExit::DisableOnExit() : timer_(sdlutils().currRealTime()) {};

void DisableOnExit::update() {
	if (timer_ + 2000 < sdlutils().currRealTime()) {
		entity_->setActive(false);
		timer_ = sdlutils().currRealTime();
	}
}