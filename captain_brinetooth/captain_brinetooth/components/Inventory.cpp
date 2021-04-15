#include "Inventory.h"

void Inventory::init() {
	//Generar un arma aleatoria

}


void Inventory::addWeapon(int weapToAdd) {
	//Si hay espacio
	if (currentWeaponNumber < 2) {
		//Si no tiene ningun arma, añadimos su componente
		if (currentWeaponNumber == 0)
			addWeaponById(weapToAdd);
		weapArray_[currentWeaponNumber] = (PosibleWeapons)weapToAdd;
		currentWeaponNumber++;
	}
	//Si ya esta lleno y el jugador no tiene ese arma encima
	else if (weapArray_[0] != weapToAdd && weapArray_[1] != weapToAdd) {
		//Cambiamos el arma seleccionada por el nuevo arma
		removeWeaponById(weapArray_[currentSelectedWeapon]);
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
	case PosibleWeapons::TypeMachineGun:
		entity_->addComponent<MachineGun>();
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
	case PosibleWeapons::TypeMachineGun:
		entity_->removeComponent<MachineGun>();
		break;
	}
}