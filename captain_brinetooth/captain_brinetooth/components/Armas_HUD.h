#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"
#include "../game/App.h"


class Armas_HUD : public Component {
public:
	Armas_HUD(Texture* weapon1, Texture* weapon2, App* game) :
		mainWeapon(weapon1),
		secondWeapon(weapon2),
		g(game)
	{
	}
	virtual ~Armas_HUD() {}

	void init() override {}

	void render() override;

	void changeWeapon();

	void actualizarNewWeapon(int opcion, Texture* newWeapon);

private:
	Texture* mainWeapon;
	Texture* secondWeapon;
	App* g;
};

