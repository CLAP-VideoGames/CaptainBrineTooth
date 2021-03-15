// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once


#include <SDL.h>
#include <cassert>
#include "Transform.h"
#include "../ecs/Entity.h"
#include "../ecs/Component.h"
#include "../sdlutils/InputHandler.h"

class KeyBoardCtrl: public Component {
public:
	KeyBoardCtrl() :
			tr_(nullptr), speed_(1.0) {
	}
	virtual ~KeyBoardCtrl() {
	}


	inline void setSpeed(float speed) {
		speed_ = speed;
	}

	void init() override {
		tr_ = entity_->getComponent<Transform>();
		assert(tr_ != nullptr);
		collider_ = entity_->getComponent<BoxCollider>();
		assert(collider_ != nullptr);
	}

	void update() override {
		if (ih().keyDownEvent()) {
			assert(collider_ != nullptr);
			if (ih().isKeyDown(SDL_SCANCODE_UP)) {
				collider_->setSpeed(Vector2D(0.0f, -speed_));
			} else if (ih().isKeyDown(SDL_SCANCODE_DOWN)) {
				collider_->setSpeed(Vector2D(0.0f, speed_));
			} else if (ih().isKeyDown(SDL_SCANCODE_LEFT)) {
				collider_->setSpeed(Vector2D(-speed_, 0.0f));
			} else if (ih().isKeyDown(SDL_SCANCODE_RIGHT)) {
				collider_->setSpeed(Vector2D(speed_, 0.0f));
			} else if (ih().isKeyDown(SDL_SCANCODE_SPACE)) {
				collider_->setSpeed(Vector2D(0.0f, 0.0f));
			}
		}
	}

private:
	Transform *tr_;
	BoxCollider* collider_;
	float speed_;
}
;

