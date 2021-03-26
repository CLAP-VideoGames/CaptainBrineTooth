#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"
#include "Transform.h"
#include "../components/BoxCollider.h"

class EnemyMovement : public Component {
public:
	EnemyMovement(Vector2D v) :
		tr_(nullptr),
		vel(v)
	{
	}
	virtual ~EnemyMovement() {
	}

	void init() override;

	void update() override;

	void setVel(int velIni);

private:
	Transform* tr_;

	Vector2D vel;
	Vector2D dir;

	BoxCollider* box;

};

