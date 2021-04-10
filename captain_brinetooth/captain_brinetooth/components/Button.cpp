#include "Button.h"

void Button::render()
{
	Transform* t = entity_->getComponent<Transform>();

	Vector2D aux = t->getPos();
	SDL_Rect dest = build_sdlrect(aux, t->getW()/2, t->getH()/2);

	tex->render(dest);
}

bool Button::handleEvent()
{
	Transform* t = entity_->getComponent<Transform>();
	std::pair<Sint32, Sint32> pos = ih().getMousePos();

	if (pos.first >= t->getPos().getX() && pos.first < t->getPos().getX() + t->getW() - (t->getW() / 2)
		&& pos.second >= t->getPos().getY() && pos.second < t->getPos().getY() + t->getH() - (t->getH() / 2)
		)
	{

		// Cambiar textura y renderizar
		Transform* t = entity_->getComponent<Transform>();

		Vector2D aux = t->getPos();
		SDL_Rect dest = build_sdlrect(aux, t->getW() / 2, t->getH() / 2);

		/*sdlutils().clearRenderer();
		
		tex2->render(dest);
		sdlutils().presentRenderer();*/
		

		if (ih().mouseButtonEvent())
		{
			cboq(game, soundController);
			return true;
		}

	}
	
	return false;

}