#include "GetAWeaponOnCollision.h"

GetAWeaponOnCollision::GetAWeaponOnCollision(int weaponToGive) : weaponToGive_(weaponToGive) {
}

void GetAWeaponOnCollision::init() {
	//Asigna el metodo introducido en la entidad para que se pueda utilizar mediante el metodo PlayCollisionMethod
	entity_->setCollisionMethod(GetWeapon);
}

void GetAWeaponOnCollision::GetWeapon(b2Contact* contact) {
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

void GetAWeaponOnCollision::GiveAWeapon(Entity* player) {
	player->getComponent<Inventory>()->addWeapon(weaponToGive_);
}
