#include "../components/Armas_HUD.h"


void Armas_HUD::render()
{
	Vector2D pos1 = Vector2D(10, 500);
	SDL_Rect dest1 = build_sdlrect(pos1, 100, 100);
	mainWeapon->render(dest1);

	Vector2D pos2 = Vector2D(dest1.x + 100, 500);
	SDL_Rect dest2 = build_sdlrect(pos2, 100, 100);
	secondWeapon->render(dest2);
}

void Armas_HUD::changeWeapon()
{
	Texture* aux = mainWeapon;
	mainWeapon = secondWeapon;
	secondWeapon = mainWeapon;
}

void Armas_HUD::actualizarNewWeapon(int opcion, Texture* newWeapon)
{
	if (opcion == 1)
	{
		mainWeapon = newWeapon;
	}
	else
	{
		secondWeapon = newWeapon;
	}
}
