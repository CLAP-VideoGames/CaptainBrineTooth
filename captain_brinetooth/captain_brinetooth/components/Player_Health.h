#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/SDLUtils.h"
#include "../game/Game.h"



class Player_Health : public Component {
public:
	Player_Health(Texture* tex, Texture* tex2, float tanim, Game* game) : fvida(tex), hvida(tex2), time(0), g(game)
	{
		frame = Vector2D(0, 15);
		auto w = fvida->width() / 8;
		auto h = fvida->height();

		src = build_sdlrect(frame, w, h);
		tiempoanimacion = tanim;
		frameSize = Vector2D(w, h);
	}
	virtual ~Player_Health() {}

	void init() override {}

	void render() override;

	void loseLife() { vidas -= 0.5f;  g->ShakeCamera(20); 
	sdlutils().soundEffects().at("player_hurt").play(); }

	int getLife(){ return vidas; }

	void resetLifes() { vidas = 10; }

private:
	float vidas = 10;
	Texture* fvida;
	Texture* hvida;
	Vector2D frame;	//Posicion en x y en y del frame actual
	Vector2D frameSize; //Ancho y alto de un frame
	SDL_Rect src; //Rectangulo que se renderiza
	Uint32 time; // Tiempo inicial

	Game* g;
	float tiempoanimacion; // Tiempo entre frame y frame
	int nFrame = 0;




};

