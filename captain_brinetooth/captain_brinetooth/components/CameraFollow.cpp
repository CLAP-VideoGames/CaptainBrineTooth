#include "CameraFollow.h"
#include "..//game/Game.h"

#include "../sdlutils/SDLUtils.h"
#include "AnimBlendGraph.h"

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
	Game::camera.x = entityT->getPos().getX() - (Game::camera.w / 2.0f) + offset_.getX();
	Game::camera.y = entityT->getPos().getY() - (Game::camera.h / 2.0f) + offset_.getY();
}

void CameraFollow::actPosAdvanced(){
	//Flip del offset.x en funcion del flip del sprite 
	Vector2D offset = Vector2D(0, offset_.getY());
	if (entity_->getComponent<AnimBlendGraph>()->isFlipX())
		offset.setX(offset_.getX());//mira derecha
	else
		offset.setX(-offset_.getX());//mira izquierda

	int x = entityT->getPos().getX()+(entityT->getW() /2.0f) - (Game::camera.w / 2.0f) + offset.getX();
	int y = entityT->getPos().getY()+(entityT->getH() /2.0f) - (Game::camera.h / 2.0f) + offset.getY();

	lastDiff.setX(sdlutils().lerpPrecise(lastDiff.getX(), x, percentage));
	differenceX += (lastDiff.getX() - (differenceX));


	lastDiff.setY(sdlutils().lerpPrecise(lastDiff.getY(), y, percentage));
	differenceY += (lastDiff.getY() - (differenceY));

	Game::camera.x = differenceX;
	Game::camera.y = differenceY;
}
