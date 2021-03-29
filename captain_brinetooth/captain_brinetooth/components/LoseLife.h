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
		//Asigna el metodo introducido en la entidad para que se pueda utilizar mediante el metodo PlayCollisionMethod
		entity_->setCollisionMethod(LoseLifeMethod);
	}

	//Metodo de ejemplo que se llama al colisionar
	//IMPORTANTE: debe de ser estatico
	static void LoseLifeMethod() {
		std::cout << "OHHH NOOO Perdió vida\n";
	}
};