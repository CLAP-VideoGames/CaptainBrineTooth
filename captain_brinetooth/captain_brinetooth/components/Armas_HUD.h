#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"


class Armas_HUD : public Component {
public:
	Armas_HUD(Texture* weapon1, Texture* weapon2) :
		mainWeapon(weapon1),
		secondWeapon(weapon2)
	{
	}
	virtual ~Armas_HUD() {
	}

	void init() override {
	}

	void render() override {
		Vector2D pos1 = Vector2D(10, 500);
		SDL_Rect dest1 = build_sdlrect(pos1, 100, 100);
		mainWeapon->render(dest1);

		Vector2D pos2 = Vector2D(dest1.x + 100, 500);
		SDL_Rect dest2 = build_sdlrect(pos2, 100, 100);
		secondWeapon->render(dest2);

	}

	void changeWeapon()
	{
		Texture* aux = mainWeapon;
		mainWeapon = secondWeapon;
		secondWeapon = mainWeapon;
	}

	int actualizarNewWeapon(int opcion, Texture* newWeapon)
	{
		if (opcion = 1)
		{
			mainWeapon = newWeapon;
		}
		else
		{
			secondWeapon = newWeapon;
		}
	}

private:
	Texture* mainWeapon;
	Texture* secondWeapon;
};

