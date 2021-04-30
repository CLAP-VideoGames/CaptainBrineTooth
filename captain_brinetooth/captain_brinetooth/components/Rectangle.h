// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <cassert>

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"
#include "Transform.h"

class Rectangle: public Component {
public:
	Rectangle();

	Rectangle(SDL_Color color);

	virtual ~Rectangle();

	void init() override;
	void render() override;

private:
	SDL_Color color_ = SDL_Color();
	Transform *tr_;
};
