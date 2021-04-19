#include "../components/Armas_HUD.h"


void Armas_HUD::render()
{
	float zoom = g->camera_Zoom_Out;
	int alt = g->camera.h;

	if (mainWeapon != nullptr) {
		Vector2D pos1 = Vector2D(10, (alt * zoom) - (alt * (zoom / 10)));


		SDL_Rect dest1 = build_sdlrect(pos1, g->camera.w * 0.09 * zoom, g->camera.h * 0.09 * zoom);
		mainWeapon->render(dest1);

		if (secondWeapon != nullptr) {
			Vector2D pos2 = Vector2D(dest1.x + (100 * zoom), (alt * zoom) - (alt * (zoom / 10)));



			SDL_Rect dest2 = build_sdlrect(pos2, g->camera.w * 0.09 * zoom, g->camera.h * 0.09 * zoom);
			secondWeapon->render(dest2);
		}
	}
}

void Armas_HUD::changeWeapon()
{
	Texture* aux = mainWeapon;
	mainWeapon = secondWeapon;
	secondWeapon = mainWeapon;
}

void Armas_HUD::actualizarNewWeapon(int opcion, Texture* newWeapon)
{
	if (opcion == 0)
	{
		mainWeapon = newWeapon;
	}
	else
	{
		secondWeapon = newWeapon;
	}
}
