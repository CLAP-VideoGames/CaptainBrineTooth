#include "Button.h"

void Button::render()
{
	Transform* t = entity_->getComponent<Transform>();

	Vector2D aux = Vector2D(150, 60);
	SDL_Rect dest = build_sdlrect(aux, t->getW(), t->getH());

	tex->render(dest);
}

bool Button::handleEvent(SDL_Event& event)
{
	if (ih().keyDownEvent())
	{
		if (ih().isKeyDown(SDL_MOUSEBUTTONDOWN))
		{
			SDL_Point p;
			p.x = event.button.x;
			p.y = event.button.y;

			Transform* t = entity_->getComponent<Transform>();

			SDL_Rect r;
			r.x = t->getPos().getX();
			r.y = t->getPos().getY();

			r.w = t->getW();
			r.h = t->getH();

			// P ES RATON Y R EL RECTANGULO DEL BOTON
			if (SDL_PointInRect(&p, &r) == SDL_TRUE)
			{
				cboq();
				return true;
			}
		}
		return false;
	}
}