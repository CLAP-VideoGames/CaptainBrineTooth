#pragma once

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../components/Inventory.h"
#include <iostream>


//This component functions as an example of a component that can be used to trigger an effect when said entity collides with something

class GetAWeaponOnCollision : public Component {
public:
	GetAWeaponOnCollision(int weaponToGive);
	virtual ~GetAWeaponOnCollision() {
	}

	void init() override;

	//Metodo de ejemplo que se llama al colisionar
	//IMPORTANTE: debe de ser estatico
	static void GetWeapon(b2Contact* contact);

private:
	void GiveAWeapon(Entity* player);

	int weaponToGive_;
};