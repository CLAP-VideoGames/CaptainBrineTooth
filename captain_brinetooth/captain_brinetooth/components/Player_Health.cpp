#include "../components/Player_Health.h"

void Player_Health::render()
{
	for (int i = 0; i < vidas; i++)
	{
		Vector2D aux = Vector2D(10 + 35 * i, 10);
		SDL_Rect dest = build_sdlrect(aux, 20, 27);
		tex_->render(dest);
	}

}