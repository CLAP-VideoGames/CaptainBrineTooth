#include "CameraFollow.h"
#include "..//game/Game.h"

void CameraFollow::update()
{
	if (player != nullptr) {
		Game::camera.x = player->getPos().getX() - (Game::camera.w / 2.0f);
		Game::camera.y = player->getPos().getY() - (Game::camera.h / 2.0f);
	}
}
