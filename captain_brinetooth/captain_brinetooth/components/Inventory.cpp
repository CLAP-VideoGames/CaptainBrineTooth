#include "Inventory.h"



void Inventory::init() {
	//Generar un arma aleatoria
	hud = entity_->getComponent<Armas_HUD>();

	//Añadimos los puños
	if (currentWeaponNumber == 0) {
		//Añadimos los puños sin que cuenten en el array de armas
		currentWeapon = entity_->addComponent<Fists>();
		hud->actualizarNewWeapon(0, &sdlutils().images().at("fist"));
	}
}

void Inventory::update() {
	//Cambio de arma
	if (ih().mouseButtonEvent()) {
		if (ih().getMouseButtonState(InputHandler::MOUSEBUTTON::RIGHT)){
			switchSelectedWeapon();
		}
	}
}
int Inventory::getWeapon(int numofweapon) {
	
	return weapArray_[numofweapon];
}

bool Inventory::hasOneWeapon()
{
	if (currentWeaponNumber == 1)return true;
	else return false;
}
bool Inventory::emptyInventory()
{
	if (currentWeaponNumber == 0)return true;
	else return false;
}

void Inventory::addWeapon(int weapToAdd) {
	//Si hay espacio
	if (currentWeaponNumber < 2) {
		//Si no tiene ningun arma, añadimos su componente
		if (currentWeaponNumber == 0) {
			if (entity_->hasComponent<Fists>())
				entity_->removeComponent<Fists>();
			addWeaponById(weapToAdd);
		}
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
		if (!currentWeapon->getIsAttacking()) {
			removeWeaponById(weapArray_[currentSelectedWeapon]);
			currentSelectedWeapon = !currentSelectedWeapon;
			addWeaponById(weapArray_[currentSelectedWeapon]);
			hud->changeWeaponSelected();
		}
	}
}
bool Inventory::hasWeapon(int searchWeapon)
{
	bool weaponfound = false;
	int i = 0;
	while (!weaponfound && i < currentWeaponNumber)
	{
		if ((int)weapArray_[i] == searchWeapon) weaponfound = true;
		else i++;
	}
	return weaponfound;

}
/// <summary>
/// Añade el componente de un arma al jugador
/// </summary>
/// <param name="weapToAdd"></param>
void Inventory::addWeaponById(int weapToAdd) {
	switch (weapToAdd) {
	case PosibleWeapons::TypeSword:
		currentWeapon = entity_->addComponent<Sword>();
		break;
	case PosibleWeapons::TypeHammer:
		currentWeapon = entity_->addComponent<Hammer>();
		break;
	case PosibleWeapons::TypeChainsaw:
		currentWeapon = entity_->addComponent<Chainsaw>();
		break;
	case PosibleWeapons::TypeCrab:
		currentWeapon = entity_->addComponent<Crab>();
		break;
	case PosibleWeapons::TypeMachineGun:
		currentWeapon = entity_->addComponent<MachineGun>();
		break;
	case PosibleWeapons::TypeEel:
		currentWeapon = entity_->addComponent<ElectricWhip>();
		break;
	case PosibleWeapons::TypeInk:
		currentWeapon = entity_->addComponent<Bellow>();
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
		return &sdlutils().images().at("machine_gun");
		break;
	case PosibleWeapons::TypeEel:
		return &sdlutils().images().at("anguila");
		break;
	case PosibleWeapons::TypeInk:
		return &sdlutils().images().at("escupetintas");
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
	case PosibleWeapons::TypeEel:
		entity_->removeComponent<ElectricWhip>();
		break;
	case PosibleWeapons::TypeInk:
		entity_->removeComponent<Bellow>();
		break;
	}
}

bool Inventory::playerAttacking() {
	return currentWeapon->getIsAttacking();
}