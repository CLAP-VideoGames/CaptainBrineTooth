#pragma once
#include "../ecs/Component.h"
#include "Transform.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
#include "../sdlutils/SDLUtils.h"

class DisableOnExit : public Component
{
public:
	DisableOnExit() : tr(nullptr) {};
	virtual ~DisableOnExit() {};
	void init() override {
		tr = entity_->getComponent<Transform>();
		assert(tr != nullptr);
	}
	void update() override {
		auto pos = tr->getPos();
		if (pos.getX() >= sdlutils().width()) entity_->setActive(false);
		else if (pos.getY() >= sdlutils().height()) entity_->setActive(false);
		else if (pos.getX() <= 0) entity_->setActive(false);
		else if (pos.getY() <= 0) {
			entity_->setActive(false);
		}
	}

private:
	Transform* tr;
};

