#pragma once

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
#include "../sdlutils/SDLUtils.h"
#include "Transform.h"

// por seguridad poner el ultimo de todos los componentes de la entidad a la que se lo pongais
class DestroyOnCollision : public Component {
public:
	DestroyOnCollision() {}
	~DestroyOnCollision() {}

	virtual void init() override;
	static void destroyOnCollision(b2Contact* contact);
};