#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"
#include "../game/App.h"


class Armas_HUD : public Component {
public:
	Armas_HUD(App* game) :
		mainWeapon(nullptr),
		secondWeapon(nullptr),
		g(game),
		weaponFrame(&sdlutils().images().at("weaponSelection"))
	{
	}
	virtual ~Armas_HUD() {}

	void init() override {}

	void render() override;

	void changeWeapon();

	void actualizarNewWeapon(int opcion, Texture* newWeapon);

	void changeWeaponSelected() { currentSelectedWeapon = !currentSelectedWeapon; };

private:
	Texture* mainWeapon;
	Texture* secondWeapon;
	Texture* weaponFrame;

	bool currentSelectedWeapon = 0;

	App* g;
};

