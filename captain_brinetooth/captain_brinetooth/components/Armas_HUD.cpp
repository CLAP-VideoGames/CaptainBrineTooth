#include "../components/Armas_HUD.h"


void Armas_HUD::render()
{
	float zoom = g->camera_Zoom_Out;
	int alt = g->camera.h;

	if (mainWeapon != nullptr) {
		Vector2D size1 = Vector2D((int)sdlutils().width() * 0.07 * App::camera_Zoom_Out, (int)sdlutils().height() * 0.1 * App::camera_Zoom_Out);
		Vector2D pos1 = Vector2D((int)((App::camera.w - size1.getX()) * 0.03), (int)((App::camera.h - size1.getY()) * 0.95));


		SDL_Rect dest1 = build_sdlrect(pos1, size1.getX(), size1.getY());
		mainWeapon->render(dest1);

		if (secondWeapon != nullptr) {
			Vector2D size2 = size1;
			Vector2D pos2 = Vector2D((int)((App::camera.w - size2.getX()) * 0.15), (int)((App::camera.h - size2.getY()) * 0.95));



			SDL_Rect dest2 = build_sdlrect(pos2, size2.getX(), size2.getY());
			secondWeapon->render(dest2);

			if (currentSelectedWeapon) {
				pos2 = Vector2D((int)((App::camera.w - size2.getX()) * 0.14), pos2.getY());
				dest2 = build_sdlrect(pos2, size2.getX() * 1.2, size2.getY() * 1.2);
				weaponFrame->render(dest2);
			}
			else {
				pos1 = Vector2D((int)((App::camera.w - size1.getX()) * 0.02), (int)((App::camera.h - size1.getY()) * 0.95));
				dest1 = build_sdlrect(pos1, size1.getX() * 1.2, size1.getY() * 1.2);
				weaponFrame->render(dest1);
			}
		}
		else {
			pos1 = Vector2D((int)((App::camera.w - size1.getX()) * 0.02), (int)((App::camera.h - size1.getY()) * 0.95));
			dest1 = build_sdlrect(pos1, size1.getX() * 1.2, size1.getY() * 1.2);
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
