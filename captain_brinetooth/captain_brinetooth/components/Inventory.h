#pragma once

#include <vector>
#include "../ecs/Entity.h"
#include "../ecs/Component.h"
#include "Sword.h"
#include "Hammer.h"
#include "Chainsaw.h"
#include "MachineGun.h"


class Inventory : public Component {
public:
	Inventory() : currentWeaponNumber(0), currentSelectedWeapon(0){}

	virtual ~Inventory() {
	}

	void addWeapon(int weapToAdd);
	void switchSelectedWeapon();
	virtual void init() override;

private:
	enum PosibleWeapons {TypeSword, TypeHammer, TypeChainsaw, TypeCrab, TypeMachineGun, TypeEel, TypeInk};

	void addWeaponById(int weapToAdd);
	void removeWeaponById(int weapToRemove);

	int currentWeaponNumber;
	const int maxWeaponNumber = 2;
	bool currentSelectedWeapon;
	std::array<PosibleWeapons, 2> weapArray_;
};