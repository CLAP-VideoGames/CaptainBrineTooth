#include "../components/Player_Health.h"

void Player_Health::render()
{
	float prueba = vidas;
	src = build_sdlrect(frame, frameSize.getX(), frameSize.getY());

	for (int i = 0; i < vidas-1; i++)
	{
		Vector2D aux = Vector2D(10 + 35 * i, 10);
		SDL_Rect dest = build_sdlrect(aux, src.w / 4, src.h / 4);
		fvida->render(src, dest);
		prueba--;
	}
	
	// Si hay media vida, renderizamos medio anzuelo
	if (prueba > 0)
	{
		Vector2D aux = Vector2D(10 + 35 * (vidas-1), 10);
		SDL_Rect dest = build_sdlrect(aux, src.w / 4, src.h / 4);
		hvida->render(src, dest);
	}
	
	if (sdlutils().currRealTime() > time + tiempoanimacion)
	{
		time = sdlutils().currRealTime();
		// Avanzamos de frame
		frame = Vector2D(frame.getX() + 194, frame.getY());
		nFrame++;

		// Si llegamos al ultimo frame, volvemos al primero
		if (nFrame > 7)
		{
			frame = Vector2D(0, 15);
			nFrame = 0;
		}
	}
	
	//Game::ShakeCamera(100);


}