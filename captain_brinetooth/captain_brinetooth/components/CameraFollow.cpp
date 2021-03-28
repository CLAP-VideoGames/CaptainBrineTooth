#include "CameraFollow.h"
#include "..//game/Game.h"

#include "../sdlutils/SDLUtils.h"

#include <iostream>

void CameraFollow::init(){
	entities_ = entity_->getMngr()->getEnteties();
	difference = 0;
	last = 0;
}

void CameraFollow::update(){
	if (entityT != nullptr){
		actPos();
		//actPosAdvanced();
	}


	/*if (Game::camera.x < 0) 
		Game::camera.x = 0;
	else if (Game::camera.x > Game::camera.w) Game::camera.x = Game::camera.w;
	 
	if (Game::camera.y < 0) 
		Game::camera.y = 0;
	else if (Game::camera.y > Game::camera.h) Game::camera.y = Game::camera.h;*/
}

void CameraFollow::actPos(){
	Game::camera.x = entityT->getPos().getX() - (Game::camera.w / 2.0f);
	Game::camera.y = entityT->getPos().getY() - (Game::camera.h / 2.0f);

	std::cout << Game::camera.x << Game::camera.y << std::endl;
}

void CameraFollow::actPosAdvanced(){
	int x = entityT->getPos().getX() - (Game::camera.w / 2.0f);
	int y = entityT->getPos().getY() - (Game::camera.h / 2.0f);

	if (std::abs(difference) < std::abs(x)){
		last = sdlutils().lerpPrecise(last, x, percentage);
		difference += (last - (difference));
	}
	else{
		last = 0;
	}

	Game::camera.x = difference;
	//Game::camera.y = y;


	std::cout << Game::camera.x << " " << x;
	std::cout << " " << entityT->getPos().getX() << std::endl;
}
