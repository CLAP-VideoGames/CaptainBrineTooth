#include "Inventory.h"



Inventory::~Inventory()
{
	currentWeapon = nullptr;
	for (Texture* e : textures_) {
		e = nullptr;
	}
	textures_.clear();
}

void Inventory::init() {
	//Generar un arma aleatoria
	hud = entity_->getComponent<Armas_HUD>();

	//Adimos los pus
	if (currentWeaponNumber == 0) {
		//Adimos los pus sin que cuenten en el array de armas
		currentWeapon = entity_->addComponent<Fists>();
		hud->actualizarNewWeapon(0, &sdlutils().images().at("fist"));
	}
	//init Coins & Baits
	coins_ = 0;
	baits_ = 0;
	coin_tex_ = &sdlutils().images().at("coin");
	textures_.push_back(coin_tex_);
	bait_tex_ = &sdlutils().images().at("bait");
	textures_.push_back(bait_tex_);
}

void Inventory::render()
{
	//Coins Symbol
	Vector2D destPos = Vector2D(25*App::camera_Zoom_Out, 175 * App::camera_Zoom_Out);
	SDL_Rect src = build_sdlrect(0 * coin_tex_->width() / 3, 0 * coin_tex_->height() / 2,
		coin_tex_->width() / 3, coin_tex_->height() / 2);
	SDL_Rect dest = build_sdlrect(destPos.getX(), destPos.getY(), 32, 32);
	coin_tex_->render(src, dest); 
	//Numbers
	renderNumber(Vector2D(destPos.getX() + 36, destPos.getY() + 5), Vector2D(16, 21), coins_);
	//Bait Symbol
	destPos = Vector2D(destPos.getX(), (destPos.getY() + 64) * App::camera_Zoom_Out);
	dest = build_sdlrect(destPos.getX(), destPos.getY(), 32, 14);
	bait_tex_->render(dest);
	//Numbers
	renderNumber(Vector2D(destPos.getX() + 36, destPos.getY() - 3), Vector2D(16, 21), baits_);
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
		//Si no tiene ningun arma, adimos su componente
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
/// Ade el componente de un arma al jugador
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
	case PosibleWeapons::TypeInk:
		entity_->removeComponent<Bellow>();
		break;
	}
}

bool Inventory::playerAttacking() {
	return currentWeapon->getIsAttacking();
}

const int& Inventory::getCoins()
{
	return coins_;
}

const int& Inventory::getBaits()
{
	return baits_;
}

void Inventory::addBaits(int n)
{
	baits_ += n;
}

void Inventory::substractBaits(int n)
{
	baits_ -= n;
	if (baits_ <= 0) baits_ = 0;
}

void Inventory::addCoins(int n)
{
	coins_ += n;
}

void Inventory::substractCoins(int n)
{
	coins_ -= n;
	if (coins_ <= 0) coins_ = 0;
}

void Inventory::renderNumber(Vector2D pos, Vector2D size, int n)
{
	//Sacar los digitos del numero
	std::vector<int> dig;
	if (n > 0) {
		int num = n;
		while (num > 0) {
			dig.push_back(num % 10);
			num = num / 10;
		}
	}
	else
		dig.push_back(0);

	//Dibujado
	int i = 0;
	for (int j = dig.size()-1; j >= 0; j--)
	{
		int aux = dig[j];
		Texture* number = &sdlutils().images().at("numbers");
		textures_.push_back(number);
		SDL_Rect src = build_sdlrect((aux % 5) * number->width() / 5, (aux / 5) * number->height() / 2,
			number->width() / 5, number->height() / 2);
		SDL_Rect dest = build_sdlrect(pos.getX() + 15 * i, pos.getY(), size.getX(), size.getY());
		number->render(src, dest);
		i++;
	}
}
