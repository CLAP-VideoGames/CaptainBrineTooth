#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "Transform.h"
#include "../sdlutils/SDLUtils.h"
#include "BoxCollider.h"
class Cuerda : public Component {
public:
	Cuerda(Entity* h);
	virtual ~Cuerda() {};
	void init() override;
	void update() override;

	void cuerdaDownMovement();
	void cuerdaUpMovement();
private:
	Transform* tr_ = nullptr;
	BoxCollider* collider_ = nullptr;
	Entity* hook;
	AnimBlendGraph* anims_;
};