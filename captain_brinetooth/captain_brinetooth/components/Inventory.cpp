#include "Inventory.h"

void Inventory::init() {
	//Generar un arma aleatoria
	hud = entity_->getComponent<Armas_HUD>();
}

void Inventory::update() {
	//Cambio de arma
	if (ih().keyDownEvent()) {
		if (ih().isKeyDown(SDL_SCANCODE_F)) {
			switchSelectedWeapon();
		}
	}
}


void Inventory::addWeapon(int weapToAdd) {
	//Si hay espacio
	if (currentWeaponNumber < 2) {
		//Si no tiene ningun arma, añadimos su componente
		if (currentWeaponNumber == 0)
			addWeaponById(weapToAdd);
		hud->actualizarNewWeapon(currentWeaponNumber, textureById(weapToAdd));
		weapArray_[currentWeaponNumber] = (PosibleWeapons)weapToAdd;
		currentWeaponNumber++;
	}
	//Si ya esta lleno y el jugador no tiene ese arma encima
	else if (weapArray_[0] != weapToAdd && weapArray_[1] != weapToAdd) {
		//Cambiamos el arma seleccionada por el nuevo arma
		removeWeaponById(weapArray_[currentSelectedWeapon]);
		hud->actualizarNewWeapon(currentSelectedWeapon, textureById(weapToAdd));
		addWeaponById(weapToAdd);
		weapArray_[currentSelectedWeapon] = (PosibleWeapons)weapToAdd;
	}
}

/// <summary>
/// Cambia el arma a utilizar si tiene dos armas
/// </summary>
void Inventory::switchSelectedWeapon() {
	if (currentWeaponNumber == 2) {
		removeWeaponById(weapArray_[currentSelectedWeapon]);
		currentSelectedWeapon = !currentSelectedWeapon;
		addWeaponById(weapArray_[currentSelectedWeapon]);
		hud->changeWeaponSelected();
	}
}

/// <summary>
/// Añade el componente de un arma al jugador
/// </summary>
/// <param name="weapToAdd"></param>
void Inventory::addWeaponById(int weapToAdd) {
	switch (weapToAdd) {
	case PosibleWeapons::TypeSword:
		entity_->addComponent<Sword>();
		break;
	case PosibleWeapons::TypeHammer:
		entity_->addComponent<Hammer>();
		break;
	case PosibleWeapons::TypeChainsaw:
		entity_->addComponent<Chainsaw>();
		break;
	case PosibleWeapons::TypeCrab:
		entity_->addComponent<Crab>();
		break;
	case PosibleWeapons::TypeMachineGun:
		entity_->addComponent<MachineGun>();
		break;
	}
}

/// <summary>
/// Selecciona una textura por su Id
/// </summary>
/// <param name="weapToAdd"></param>
Texture* Inventory::textureById(int weapToAdd) {
	switch (weapToAdd) {
	case PosibleWeapons::TypeSword:
		return &sdlutils().images().at("espada");
		break;
	case PosibleWeapons::TypeHammer:
		return &sdlutils().images().at("martillo");
		break;
	case PosibleWeapons::TypeChainsaw:
		return &sdlutils().images().at("sierra");
		break;
	case PosibleWeapons::TypeCrab:
		return &sdlutils().images().at("crab");
		break;
	case PosibleWeapons::TypeMachineGun:
		return &sdlutils().images().at("sierra");
		break;
	}
}

/// <summary>
/// Quita el componente de un arma al jugador
/// </summary>
/// <param name="weapToAdd"></param>
void Inventory::removeWeaponById(int weapToAdd) {
	switch (weapToAdd) {
	case PosibleWeapons::TypeSword:
		entity_->removeComponent<Sword>();
		break;
	case PosibleWeapons::TypeHammer:
		entity_->removeComponent<Hammer>();
		break;
	case PosibleWeapons::TypeChainsaw:
		entity_->removeComponent<Chainsaw>();
		break;
	case PosibleWeapons::TypeCrab:
		entity_->removeComponent<Crab>();
		break;
	case PosibleWeapons::TypeMachineGun:
		entity_->removeComponent<MachineGun>();
		break;
	}
}