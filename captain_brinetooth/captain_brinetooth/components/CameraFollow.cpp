#include "CameraFollow.h"
#include "..//game/Game.h"

#include "../sdlutils/SDLUtils.h"


#include <list>
#include <iostream>

void CameraFollow::init(){
	entities_ = entity_->getMngr()->getEnteties();
	//difference = 0;
	//last = 0;
}

void CameraFollow::update(){
	if (entityT != nullptr){
		//actPos();
		actPosAdvanced();
	}

	if (Game::camera.x < 0) 
		Game::camera.x = 0;
	else if (Game::camera.x > Game::camera.w) Game::camera.x = Game::camera.w;
	 
	if (Game::camera.y < 0) 
		Game::camera.y = 0;
	else if (Game::camera.y > Game::camera.h) Game::camera.y = Game::camera.h;

	//std::cout << Game::camera.x << " " << Game::camera.y << std::endl;
}

void CameraFollow::actPos(){
	Game::camera.x = entityT->getPos().getX() - (Game::camera.w / 2.0f) + offset.getX();
	Game::camera.y = entityT->getPos().getY() - (Game::camera.h / 2.0f) + offset.getY();
}

void CameraFollow::actPosAdvanced(){
	int x = entityT->getPos().getX() - (Game::camera.w / 2.0f) + offset.getX();
	int y = entityT->getPos().getY() - (Game::camera.h / 2.0f) + offset.getY();

	lastDiff.setX(sdlutils().lerpPrecise(lastDiff.getX(), x, percentage));
	differenceX += (lastDiff.getX() - (differenceX));


	lastDiff.setY(sdlutils().lerpPrecise(lastDiff.getY(), y, percentage));
	differenceY += (lastDiff.getY() - (differenceY));

	Game::camera.x = differenceX;
	Game::camera.y = differenceY;
}
