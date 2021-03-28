#pragma once
#include "..//ecs/Component.h"
#include "Transform.h"
#include "..//ecs/Entity.h"

class Game;

class CameraFollow : public Component 
{
public: 
	CameraFollow(Transform* p, float percentage_ = 0.05f) {
		entityT = p;
		percentage = percentage_;
	}

	void init() override;

	void update() override;
	void actPos();
	void actPosAdvanced();




protected:
	Transform* entityT;
	std::vector<Entity*> entities_;

	int limit;
	int difference;
	int last;
	float percentage;
};