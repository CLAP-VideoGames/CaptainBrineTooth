#include "PescaController.h"
#include "Gancho.h"
#include "Cuerda.h"
PescaController::PescaController(int w){ 
	s_width = w;
}
void PescaController::init() {
	collider_ = entity_->getComponent<BoxCollider>();
	assert(collider_ != nullptr);
	collider_->getBody()->SetGravityScale(0.0f);
	
}
void PescaController::update() {
	if(!move){
		if (ih().keyDownEvent())
			if (ih().isKeyDown(SDLK_SPACE)) {
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
		if (entity_->getMngr()->getHandler<Rod>()->getComponent<BoxCollider>()->getPhysicalPos().getX() <= s_width * 0.15) {
			collider_->setSpeed(Vector2D(0, collider_->getBody()->GetLinearVelocity().y));
			if (ih().keyDownEvent()) {
				if (ih().isKeyDown(SDL_SCANCODE_D)) {
					collider_->setSpeed(Vector2D(2, collider_->getBody()->GetLinearVelocity().y));
				}

			}
		}
		else if(entity_->getMngr()->getHandler<Rod>()->getComponent<BoxCollider>()->getPhysicalPos().getX() >= s_width * 0.75){
			collider_->setSpeed(Vector2D(0, collider_->getBody()->GetLinearVelocity().y));
			if (ih().keyDownEvent()) {
				if (ih().isKeyDown(SDL_SCANCODE_A)) {
					collider_->setSpeed(Vector2D(-2, collider_->getBody()->GetLinearVelocity().y));
				}

			}
		}
		else {
			if (ih().keyDownEvent()) {
				if (ih().isKeyDown(SDL_SCANCODE_A)) {
					collider_->setSpeed(Vector2D(-2, collider_->getBody()->GetLinearVelocity().y));
				}
				if (ih().isKeyDown(SDL_SCANCODE_D)) {
					collider_->setSpeed(Vector2D(2, collider_->getBody()->GetLinearVelocity().y));
				}
			}
		}
		if (ih().keyUpEvent()) {
			if (ih().isKeyUp(SDL_SCANCODE_D) && ih().isKeyUp(SDL_SCANCODE_A)) collider_->setSpeed(Vector2D(0, collider_->getBody()->GetLinearVelocity().y));
		}
	}
}