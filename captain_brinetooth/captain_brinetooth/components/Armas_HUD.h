#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"
#include "../game/Game.h"


class Armas_HUD : public Component {
public:
	Armas_HUD(Texture* weapon1, Texture* weapon2, Game* game) :
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
	Game* g;
};

