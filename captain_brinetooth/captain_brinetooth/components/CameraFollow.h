#pragma once
#include "..//ecs/Component.h"
#include "Transform.h"
#include "..//ecs/Entity.h"

class Game;

class CameraFollow : public Component 
{
public: 
	CameraFollow(Transform* p) {
		player = p;
	}

	void init() override;

	void update() override;


protected:
	Transform* player;
	std::vector<Entity*> entities_;

};