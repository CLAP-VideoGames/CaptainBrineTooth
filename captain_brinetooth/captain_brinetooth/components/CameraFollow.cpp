#include "CameraFollow.h"
#include "..//game/Game.h"

void CameraFollow::init()
{
	entities_ = entity_->getMngr()->getEnteties();
}

void CameraFollow::update()
{
	//Mover la camara hacia donde se mueva el player.
	//Mover las posicion de todos los objetos en pantalla en funcion de la posicion de la camara

	if (entityT != nullptr) {
		Game::camera.x = entityT->getPos().getX() - (Game::camera.w / 2.0f);
		Game::camera.y = entityT->getPos().getY() - (Game::camera.h / 2.0f);
	}

	/*if (Game::camera.x < 0) Game::camera.x = 0;
	else if (Game::camera.y < 0) Game::camera.y = 0;
	else if (Game::camera.x > Game::camera.w) Game::camera.x = Game::camera.w;
	else if (Game::camera.y > Game::camera.h) Game::camera.y = Game::camera.h;*/

}
