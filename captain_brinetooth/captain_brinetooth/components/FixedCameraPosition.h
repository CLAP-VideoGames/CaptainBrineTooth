#pragma once
#include "../ecs/Component.h"
#include "Transform.h"
#include "BoxCollider.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"




class Camera;

class FixedCameraPosition : public Component
{
public:
	FixedCameraPosition() {
		entityT_ = nullptr;
		bC_ = nullptr;
	};

	virtual ~FixedCameraPosition() {};


	void init() override;
	void update() override;

private:

	Transform* entityT_;
	BoxCollider* bC_;

};