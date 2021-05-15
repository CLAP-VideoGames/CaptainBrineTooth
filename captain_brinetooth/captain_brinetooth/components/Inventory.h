#pragma once

#include <vector>
#include "../ecs/Entity.h"
#include "../ecs/Component.h"
#include "Sword.h"
#include "Hammer.h"
#include "Chainsaw.h"
#include "MachineGun.h"
#include "Crab.h"
#include "ElectricWhip.h"
#include "Bellow.h"
#include "Fists.h"
#include "Armas_HUD.h"
#include "WeaponComponent.h"


class Inventory : public Component {
public:
	Inventory() : currentWeaponNumber(0), currentSelectedWeapon(0) {}

	virtual ~Inventory() {
		currentWeapon = nullptr;
	}

	void addWeapon(int weapToAdd);
	void switchSelectedWeapon();
	bool hasWeapon(int weapontoSearch);
	int getWeapon(int numofweapon);
	bool hasOneWeapon();
	bool emptyInventory();
	virtual void init() override;
	virtual void update() override;

	bool playerAttacking();

private:
	enum PosibleWeapons { TypeSword, TypeHammer, TypeChainsaw, TypeCrab, TypeMachineGun, TypeInk };
	void addWeaponById(int weapToAdd);
	void removeWeaponById(int weapToRemove);

	Texture* textureById(int weapToAdd);

	Armas_HUD* hud;

	int currentWeaponNumber;
	const int maxWeaponNumber = 2;
	bool currentSelectedWeapon;
	std::array<PosibleWeapons, 2> weapArray_;

	WeaponComponent* currentWeapon;
};