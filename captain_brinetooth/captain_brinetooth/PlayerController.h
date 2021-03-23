#pragma once
#include "../ecs/Component.h"
class PlayerController : public Component
{
public:
	PlayerController()
	{

	}

	virtual ~PlayerController() {
	}

	void init() override {

	}

	void update() override {
	}
	void controllerAux();
};

