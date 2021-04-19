#pragma once

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../components/Inventory.h"
#include <iostream>


//This component functions as an example of a component that can be used to trigger an effect when said entity collides with something

class GetAWeaponOnCollision : public Component {
public:
	GetAWeaponOnCollision(int weaponToGive) : weaponToGive_(weaponToGive) {
	}
	virtual ~GetAWeaponOnCollision() {
	}

	void init() override {
		//Asigna el metodo introducido en la entidad para que se pueda utilizar mediante el metodo PlayCollisionMethod
		entity_->setCollisionMethod(GetWeapon);
	}

	//Metodo de ejemplo que se llama al colisionar
	//IMPORTANTE: debe de ser estatico
	static void GetWeapon(b2Contact* contact) {
		//Llamar a otro metodo
		Entity* cosaA = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
		Entity* cosaB = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

		if (cosaB != nullptr && cosaA != nullptr)
			if (cosaA->getComponent<Inventory>() != nullptr) {
				cosaB->getComponent<GetAWeaponOnCollision>()->GiveAWeapon(cosaA);
				cosaB->setActive(false);
			}
			else if (cosaB->getComponent<Inventory>() != nullptr) {
				cosaA->getComponent<GetAWeaponOnCollision>()->GiveAWeapon(cosaA);
				cosaA->setActive(false);
			}
	}
private:
	void GiveAWeapon(Entity* player) {
		player->getComponent<Inventory>()->addWeapon(weaponToGive_);
	}

	int weaponToGive_;
};