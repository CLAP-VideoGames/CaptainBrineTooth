#include "PescaController.h"
PescaController::PescaController(){}
void PescaController::init() {
	collider_ = entity_->getComponent<BoxCollider>();
	assert(collider_ != nullptr);
	collider_->getBody()->SetGravityScale(0.0f);
}
void PescaController::update() {
	if (ih().keyDownEvent()) {
		if (ih().isKeyDown(SDL_SCANCODE_LEFT)) {
			collider_->setSpeed(Vector2D(-1, collider_->getBody()->GetLinearVelocity().y));
		}
		if (ih().isKeyDown(SDL_SCANCODE_RIGHT)) {
			collider_->setSpeed(Vector2D(1, collider_->getBody()->GetLinearVelocity().y));
		}
	}
}