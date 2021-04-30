#include "../components/Armas_HUD.h"


void Armas_HUD::render()
{
	float zoom = g->camera_Zoom_Out;
	int alt = g->camera.h;

	if (mainWeapon != nullptr) {
		Vector2D pos1 = Vector2D(40, (alt * zoom) - (alt * (zoom / 10)));


		SDL_Rect dest1 = build_sdlrect(pos1, g->camera.w * 0.07f * zoom, g->camera.h * 0.07f * zoom);
		mainWeapon->render(dest1);

		if (secondWeapon != nullptr) {
			Vector2D pos2 = Vector2D(dest1.x + (100 * zoom), (alt * zoom) - (alt * (zoom / 10)));



			SDL_Rect dest2 = build_sdlrect(pos2, g->camera.w * 0.07f * zoom, g->camera.h * 0.07f * zoom);
			secondWeapon->render(dest2);

			if (currentSelectedWeapon) {
				pos2 = Vector2D(5 + (100 * zoom), (alt * zoom) - (alt * (zoom / 10)));
				dest2 = build_sdlrect(pos2, g->camera.w * 0.10f * zoom, g->camera.h * 0.10f * zoom);
				weaponFrame->render(dest2);
			}
			else {
				pos1 = Vector2D(5, (alt * zoom) - (alt * (zoom / 10)));
				dest1 = build_sdlrect(pos1, g->camera.w * 0.10f * zoom, g->camera.h * 0.10f * zoom);
				weaponFrame->render(dest1);
			}
		}
		else {
			pos1 = Vector2D(5, (alt * zoom) - (alt * (zoom / 10)));
			dest1 = build_sdlrect(pos1, (int)(g->camera.w * 0.10f * zoom), (int)(g->camera.h * 0.10f * zoom));
			weaponFrame->render(dest1);
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
	entity_->getMngr()->getSoundMngr()->playSoundEffect("pick_weapon", 500);

	if (opcion == 0)
	{
		mainWeapon = newWeapon;
	}
	else
	{
		secondWeapon = newWeapon;
	}
}
