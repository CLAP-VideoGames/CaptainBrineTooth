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

	virtual ~Inventory();

	virtual void init() override;
	virtual void render() override;
	virtual void update() override;

	void addWeapon(int weapToAdd);
	void switchSelectedWeapon();
	bool hasWeapon(int weapontoSearch);
	int getWeapon(int numofweapon);
	bool hasOneWeapon();
	bool emptyInventory();
	bool playerAttacking();

	//Coins & Baits
	const int& getCoins();
	void addCoins(int n);
	void substractCoins(int n);
	const int& getBaits();
	void addBaits(int n);
	void substractBaits(int n);
	void renderNumber(Vector2D pos, Vector2D size, int n);

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

	//Coins & Bait
	int coins_, baits_;
	std::vector<Texture*> textures_;
	Texture* coin_tex_;
	Texture* bait_tex_;
};