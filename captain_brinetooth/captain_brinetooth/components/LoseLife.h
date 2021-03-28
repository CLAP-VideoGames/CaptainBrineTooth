#pragma once

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include <iostream>


//This component functions as an example of a component that can be used to trigger an effect when said entity collides with something

class LoseLife : public Component {
public:
	LoseLife() {
	}
	virtual ~LoseLife() {
	}

	void init() override {
		entity_->setCollisionMethod(LoseLifeMethod);
	}

	static void LoseLifeMethod() {
		std::cout << "OHHH NOOO Perdió vida\n";
	}
};