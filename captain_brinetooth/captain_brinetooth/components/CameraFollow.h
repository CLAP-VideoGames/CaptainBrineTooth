#pragma once
#include "..//ecs/Component.h"
#include "Transform.h"
#include "..//ecs/Entity.h"

class Game;

class CameraFollow : public Component 
{
public: 
	CameraFollow(Transform* p) {
		entityT = p;
	}

	void init() override;

	void update() override;


protected:
	Transform* entityT;
	std::vector<Entity*> entities_;

};