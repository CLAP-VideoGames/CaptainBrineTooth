#include "PescaController.h"
#include "Gancho.h"
#include "Cuerda.h"
PescaController::PescaController(){}
void PescaController::init() {
	collider_ = entity_->getComponent<BoxCollider>();
	assert(collider_ != nullptr);
	collider_->getBody()->SetGravityScale(0.0f);
}
void PescaController::update() {
	if(!move){
		if (ih().keyDownEvent())
			if (ih().isKeyDown(SDLK_k)) {
				move = true;
				if (entity_->getComponent<Gancho>() != nullptr) {
					entity_->getComponent<Gancho>()->setBajar(true);
				}
				if (entity_->getComponent<Cuerda>() != nullptr) {
					entity_->getComponent<Cuerda>()->setBajar(true);
				}
			}
				
	}
	else {
		if (entity_->getMngr()->getHandler<Rod>()->getComponent<BoxCollider>()->getPhysicalPos().getX() <= 600) {
			collider_->setSpeed(Vector2D(0, collider_->getBody()->GetLinearVelocity().y));
			if (ih().keyDownEvent()) {
				if (ih().isKeyDown(SDL_SCANCODE_RIGHT)) {
					collider_->setSpeed(Vector2D(2, collider_->getBody()->GetLinearVelocity().y));
				}

			}
		}
		else if(entity_->getMngr()->getHandler<Rod>()->getComponent<BoxCollider>()->getPhysicalPos().getX() >= 1400){
			collider_->setSpeed(Vector2D(0, collider_->getBody()->GetLinearVelocity().y));
			if (ih().keyDownEvent()) {
				if (ih().isKeyDown(SDL_SCANCODE_LEFT)) {
					collider_->setSpeed(Vector2D(-2, collider_->getBody()->GetLinearVelocity().y));
				}

			}
		}
		else {
			if (ih().keyDownEvent()) {
				if (ih().isKeyDown(SDL_SCANCODE_LEFT)) {
					collider_->setSpeed(Vector2D(-2, collider_->getBody()->GetLinearVelocity().y));
				}
				if (ih().isKeyDown(SDL_SCANCODE_RIGHT)) {
					collider_->setSpeed(Vector2D(2, collider_->getBody()->GetLinearVelocity().y));
				}
			}
		}
		if (ih().keyUpEvent()) {
			if (ih().isKeyUp(SDL_SCANCODE_RIGHT) && ih().isKeyUp(SDL_SCANCODE_LEFT)) collider_->setSpeed(Vector2D(0, collider_->getBody()->GetLinearVelocity().y));
		}
	}
}