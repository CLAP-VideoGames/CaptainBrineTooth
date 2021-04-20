#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "Transform.h"
#include "../sdlutils/SDLUtils.h"
#include "BoxCollider.h"
class Cuerda : public Component {
public:
	Cuerda();
	virtual ~Cuerda() {};
	void init() override;
	void update() override;

	void cuerdaMovement();

private:
	Transform* tr_ = nullptr;
	BoxCollider* collider_ = nullptr;
};