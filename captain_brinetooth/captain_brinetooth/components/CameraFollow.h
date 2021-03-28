#pragma once
#include "..//ecs/Component.h"
#include "Transform.h"
#include "..//ecs/Entity.h"

class Game;

class CameraFollow : public Component 
{
public: 
	CameraFollow(Transform* p, const Vector2D & offS = Vector2D(0.0f, 0.f), const float& percentage_ = 0.0f) : lastDiff(), differenceX(), differenceY(){
		entityT = p;
		percentage = percentage_;
		offset = offS;
	}

	void init() override;

	void update() override;
	void actPos();
	void actPosAdvanced();

protected:
	Transform* entityT;
	std::vector<Entity*> entities_;

	int differenceX, differenceY;
	float percentage;
	Vector2D lastDiff, offset;
};