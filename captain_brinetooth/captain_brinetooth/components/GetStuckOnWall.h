#pragma once

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "BoxCollider.h"
#include "../game/CollisionLayers.h"
#include <iostream>

using namespace ColLayers;

//This component functions as an example of a component that can be used to trigger an effect when said entity collides with something

class GetStuckOnWall : public Component {
public:
	GetStuckOnWall() {};

	void init() override;

	//Metodo de ejemplo que se llama al colisionar
	//IMPORTANTE: debe de ser estatico
	static void ContactGround(b2Contact* contact);

	//Aplica danio al enemigo
	void getStuck();
private:
	BoxCollider* collider;
};