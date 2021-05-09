#include "Player_Health.h"

#define _CRTDBG_MAP_ALLOC
#include<iostream>
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#include "../states/PlayState.h"

void Player_Health::init()
{
	frame = Vector2D(0, 15);
	auto w = fVida->width() / 8;
	auto h = fVida->height();

	src = build_sdlrect(frame, w, h);
	time_ = 0;
	frameSize = Vector2D(w, h);

	resetLifes();
	invulnerability_ = false;
	elpased_time_invul_ = 0;
	deadCountdown = 0;
}

void Player_Health::render()
{
	float vidaRestante = vidas;
	float currentVidas = vidas+0.5;
	src = build_sdlrect(frame, frameSize.getX(), frameSize.getY());
	Vector2D aux = Vector2D(10 + 35 * 0, 10);
	float zoom = g->camera_Zoom_Out;

	for (int i = 0; i < vidas-0.5; i++)
	{
		aux = Vector2D(10 + 35 * i * zoom, 10);
		SDL_Rect dest = build_sdlrect(aux, (src.w / 4) * zoom, (src.h / 4)* zoom);

		fVida->render(src, dest);
		vidaRestante--;
	}
	
	// Si hay media vida, renderizamos medio anzuelo
	if (vidaRestante > 0)
	{
		aux = Vector2D(10 + 35 * (vidas-0.5) * zoom, 10);
		SDL_Rect dest = build_sdlrect(aux, (src.w / 4) * zoom, (src.h / 4)* zoom);
		hVida->render(src, dest);
	}
	
	if (currentVidas < maxVidas)
	{
		aux = Vector2D(aux.getX() + (zoom * 50), 10);
		SDL_Rect dest = build_sdlrect(aux, (src.w / 6) * zoom, (src.h / 5) * zoom);
		vVida->render(dest);
		for (int i = 1; i < (maxVidas - currentVidas); i++)
		{
			aux = Vector2D(aux.getX() + (zoom * 50) - 10, 10);
			SDL_Rect dest = build_sdlrect(aux, (src.w / 6 )* zoom, (src.h / 5)*zoom);
			vVida->render(dest);
		}
	}
	// Si pasa mas de X tiempo, pasamos al siguiente frame
	if (sdlutils().currRealTime() > time_ + tiempoanimacion)
	{
		time_ = sdlutils().currRealTime();

		// Avanzamos de frame
		frame = Vector2D(frame.getX() + 194, frame.getY()); // 194 es la cantidad exacta a avanzar al siguiente frame sin que ocurra nada
		nFrame++;

		// Si llegamos al ultimo frame, volvemos al primero
		if (nFrame > 7) { frame = Vector2D(0, 15); nFrame = 0; }
	}

	//Invulnerabilidad
	if (invulnerability_ && sdlutils().currRealTime() > elpased_time_invul_ + cd_invul_)
		invulnerability_ = false;
	
}

void Player_Health::update()
{
	if (ih().keyDownEvent())
		if (ih().isKeyDown(SDL_SCANCODE_0)) {
			vidas = 0.5;
			loseLife();
		}
	//Tiempo despues de muerto
	if (entity_->getComponent<AnimBlendGraph>()->getCurrentAnimation()->getID() == "death" && entity_->getComponent<AnimBlendGraph>()->isComplete() && deadCountdown == 0)
		deadCountdown = sdlutils().currRealTime();

	if(deadCountdown + dead_time < sdlutils().currRealTime() && deadCountdown != 0)
		respawn();
}

void Player_Health::loseLife() 
{
	int x = sdlutils().rand().teCuoto(0, 2);

	switch (x)
	{
	case 0:
		entity_->getMngr()->getSoundMngr()->playSoundEffect("player_hurt2", 0);
		break;
	case 1:
		entity_->getMngr()->getSoundMngr()->playSoundEffect("player_hurt1", 0);
		break;
	
	default:
		break;
	}
	if (vidas >= 0.5) { vidas -= 0.5f;  g->ShakeCamera(20); }

	invulnerability_ = true;
	elpased_time_invul_ = sdlutils().currRealTime();

	if (vidas <= 0)
	{
		entity_->getComponent<AnimBlendGraph>()->setParamValue("Dead", 1);
	}
}

int Player_Health::getLife()
{
	return vidas;
}

void Player_Health::resetLifes()
{  
	vidas = maxVidas;
}

const bool& Player_Health::getInvulnerable()
{
	return invulnerability_;
}


void Player_Health::respawn()
{
	StateMachine* sM = g->getStateMachine();
	Manager* mngr = entity_->getMngr();
	mngr->getSoundMngr()->ChangeMainMusic("Nivel1");

	sM->popState();
	sM->changeState(new PlayState(g, mngr->getWorld(), mngr->getSoundMngr()));

}