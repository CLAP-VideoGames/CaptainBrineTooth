#pragma once

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "Enemy_Health.h"
#include <iostream>


//This component functions as an example of a component that can be used to trigger an effect when said entity collides with something

class WeaponDamageDetection : public Component {
public:
	WeaponDamageDetection(int damageToApply, int typeOfDamage = 0);
	virtual ~WeaponDamageDetection();

	void init() override;

	//Metodo de ejemplo que se llama al colisionar
	//IMPORTANTE: debe de ser estatico
	static void ContactEnemy(b2Contact* contact);

	//Aplica danio al enemigo
	void ApplyDamage(Entity* enemy);
private:
	int damageToApply_, typeOfDamage_;
};