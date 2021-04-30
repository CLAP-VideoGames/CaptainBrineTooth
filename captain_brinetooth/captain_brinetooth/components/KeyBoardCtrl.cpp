#include "KeyBoardCtrl.h"

KeyBoardCtrl::KeyBoardCtrl(MapProcedural* m) :
	tr_(nullptr), speed_(3.0), mapa(m) {
}

KeyBoardCtrl::~KeyBoardCtrl()
{
}

void KeyBoardCtrl::init() {
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);
	collider_ = entity_->getComponent<BoxCollider>();
	assert(collider_ != nullptr);
	animController_ = entity_->getComponent<AnimBlendGraph>();
	//assert(animController_ != nullptr);
}

void KeyBoardCtrl::update() {
	if (ih().keyDownEvent()) {
		assert(collider_ != nullptr);
		if (ih().isKeyDown(SDL_SCANCODE_UP)) {
			collider_->setSpeed(Vector2D(0.0f, -speed_));
		}
		else if (ih().isKeyDown(SDL_SCANCODE_DOWN)) {
			collider_->setSpeed(Vector2D(0.0f, speed_));
		}
		else if (ih().isKeyDown(SDL_SCANCODE_LEFT)) {
			collider_->setSpeed(Vector2D(-speed_, 0.0f));
		}
		else if (ih().isKeyDown(SDL_SCANCODE_RIGHT)) {
			collider_->setSpeed(Vector2D(speed_, 0.0f));
		}
		else if (ih().isKeyDown(SDL_SCANCODE_SPACE)) {
			collider_->setSpeed(Vector2D(0.0f, 0.0f));
			//entity_->getComponent<Player_Health>()->loseLife();
		}
		//Test animacion, Ejemplo de uso
		else if (ih().isKeyDown(SDL_SCANCODE_A)) {
			animController_->setParamValue("NotOnFloor", 1);
		}
		else if (ih().isKeyDown(SDL_SCANCODE_X)) {
			animController_->setParamValue("NotOnFloor", 0);
		}

		if (ih().isKeyDown(SDL_SCANCODE_I)) {
			mapa->TravelNextRoom(0);
		}
		else if (ih().isKeyDown(SDL_SCANCODE_L)) {
			mapa->TravelNextRoom(1);
		}
		if (ih().isKeyDown(SDL_SCANCODE_K)) {
			mapa->TravelNextRoom(2);
		}
		if (ih().isKeyDown(SDL_SCANCODE_J)) {
			mapa->TravelNextRoom(3);
		}
	}


}
