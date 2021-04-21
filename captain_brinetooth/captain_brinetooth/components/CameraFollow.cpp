#include "CameraFollow.h"

#include "../sdlutils/SDLUtils.h"
#include "AnimBlendGraph.h"

#include <list>
#include <iostream>

void CameraFollow::init(){
	entities_ = entity_->getMngr()->getEnteties();
	//difference = 0;
	//last = 0;

	std::cout << (*wLimit) << " " << (*hLimit);
}

void CameraFollow::update(){
	if (entityT != nullptr){
		//actPos();
		actPosAdvanced();
	}

	if (!limitless_Horizontal) {
		if (App::camera.x < 0)
			App::camera.x = 0;
		else if (App::camera.x + (App::camera.w * zoom) > (*wLimit))
			App::camera.x = (*wLimit) - (App::camera.w * zoom);
	}

	std::cout << App::camera.x + App::camera.w << " " << App::camera.y << std::endl;

	if (!limitless_Vertical) {
		if (App::camera.y < 0)
			App::camera.y = 0;
		else if (App::camera.y + App::camera.h > (*hLimit))
			App::camera.y = (*hLimit);
	}

	//std::cout << Game::camera.x << " " << Game::camera.y << std::endl;
}

void CameraFollow::actPos(){
	App::camera.x = entityT->getPos().getX() - (App::camera.w / 2.0f) + offset_.getX();
	App::camera.y = entityT->getPos().getY() - (App::camera.h / 2.0f) + offset_.getY();
}

void CameraFollow::actPosAdvanced(){
	//Flip del offset.x en funcion del flip del sprite 
	Vector2D offset = Vector2D(0, offset_.getY());
	if (entity_->getComponent<AnimBlendGraph>()->isFlipX())
		offset.setX(offset_.getX());//mira derecha
	else
		offset.setX(-offset_.getX());//mira izquierda

	int x = entityT->getPos().getX()+(entityT->getW() /2.0f) - (App::camera.w / 2.0f) + offset.getX();
	int y = entityT->getPos().getY()+(entityT->getH() /2.0f) - (App::camera.h / 2.0f) + offset.getY();

	lastDiff.setX(sdlutils().lerpPrecise(lastDiff.getX(), x, percentage));
	differenceX += (lastDiff.getX() - (differenceX));


	lastDiff.setY(sdlutils().lerpPrecise(lastDiff.getY(), y, percentage));
	differenceY += (lastDiff.getY() - (differenceY));

	App::camera.x = differenceX;
	App::camera.y = differenceY;
}
