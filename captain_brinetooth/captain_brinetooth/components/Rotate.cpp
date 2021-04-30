#include "Rotate.h"

Rotate::Rotate() :
	tr_(nullptr), speed_(10.0) {
}

Rotate::~Rotate()
{
}

void Rotate::init() {
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);

}

void Rotate::update() {
	if (ih().keyDownEvent()) {
		auto& vel = tr_->getVel();
		if (ih().isKeyDown(SDLK_LEFT)) {
			tr_->setRot(tr_->getRot() + 5.0f);
		}
		else if (ih().isKeyDown(SDLK_RIGHT)) {
			tr_->setRot(tr_->getRot() - 5.0f);
		}
	}
}
