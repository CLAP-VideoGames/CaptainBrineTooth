#include "CameraFollow.h"

#define _CRTDBG_MAP_ALLOC
#include<iostream>
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include "../sdlutils/SDLUtils.h"
#include "AnimBlendGraph.h"

#include <list>
#include <iostream>

void CameraFollow::init(){
	target_ = entity_->getComponent<Transform>();
	assert(target_ != nullptr);
	up_time_ = down_time_ = 0;
}

void CameraFollow::update(){
	//Input
	if (ih().keyDownEvent()) {
		if (ih().isKeyDown(SDL_SCANCODE_S))
			down_time_ ++;
		if (ih().isKeyDown(SDL_SCANCODE_W))
			up_time_ ++;
	}
	if (ih().keyUpEvent()) {
		if (ih().isKeyUp(SDL_SCANCODE_S))
			down_time_ = 0;
		if (ih().isKeyUp(SDL_SCANCODE_W))
			up_time_ = 0;
	}
	//Actualizar la camara
	actPosAdvanced();
	//Asegurar que no se sale de los limites
	checkCameraFollowLimits();
}

void CameraFollow::actPosAdvanced(){
	//Flip del offset.x en funcion del flip del sprite 
	Vector2D offset = Vector2D(0, 0);
	//X Axis
	if (entity_->getComponent<AnimBlendGraph>()->isFlipX())
		offset.setX(offset_.getX());//mira derecha
	else
		offset.setX(-offset_.getX());//mira izquierda
	//Y axis
	if (down_time_ >= cd_down_)
		offset.setY(-offset_.getY());//mira abajo
	else if (up_time_ >= cd_up_)
		offset.setY(offset_.getY() * 1.5);//mira arriba
	else
		offset.setY(offset_.getY());

	int x = target_->getPos().getX()+(target_->getW() /2.0f) - (App::camera.w / 2.0f) + offset.getX();
	int y = target_->getPos().getY()+(target_->getH() /2.0f) - (App::camera.h / 2.0f) + offset.getY();

	lastDiff.setX(sdlutils().lerpPrecise(lastDiff.getX(), x, percentage_));
	differenceX += (lastDiff.getX() - (differenceX));


	lastDiff.setY(sdlutils().lerpPrecise(lastDiff.getY(), y, percentage_));
	differenceY += (lastDiff.getY() - (differenceY));

	App::camera.x = differenceX;
	App::camera.y = differenceY;
}

void CameraFollow::checkCameraFollowLimits(){
	if (!limitless_Horizontal_) {
		if (camMaxLimits_.getX()-camMinLimits_.getX() >= App::camera.w) {
			if (App::camera.x < camMinLimits_.getX())
				App::camera.x = camMinLimits_.getX();
			else if (App::camera.x + App::camera.w > camMaxLimits_.getX())
				App::camera.x = camMaxLimits_.getX() - App::camera.w;
			}
		else
			App::camera.x = camMinLimits_.getX();
	}

	if (!limitless_Vertical_) {
		if (camMaxLimits_.getY() - camMinLimits_.getY() >= App::camera.h) {
			if (App::camera.y < camMinLimits_.getY())
				App::camera.y = camMinLimits_.getY();
			else if (App::camera.y + App::camera.h > camMaxLimits_.getY())
				App::camera.y = camMaxLimits_.getY() - App::camera.h;
		}
		else
			App::camera.y = camMinLimits_.getY();
	}
}

void CameraFollow::setCamToEntity(const float& t){
	//Flip del offset.x en funcion del flip del sprite 
	Vector2D offset = Vector2D(0, 0);
	//X Axis
	if (entity_->getComponent<AnimBlendGraph>()->isFlipX())
		offset.setX(offset_.getX());//mira derecha
	else
		offset.setX(-offset_.getX());//mira izquierda
	//Y axis
	if (down_time_ >= cd_down_)
		offset.setY(-offset_.getY());//mira abajo
	else if (up_time_ >= cd_up_)
		offset.setY(offset_.getY() * 1.5);//mira arriba
	else
		offset.setY(offset_.getY());

	int x = target_->getPos().getX() + (target_->getW() / 2.0f) - (App::camera.w / 2.0f) + offset.getX();
	int y = target_->getPos().getY() + (target_->getH() / 2.0f) - (App::camera.h / 2.0f) + offset.getY();

	lastDiff.setX(sdlutils().lerpPrecise(lastDiff.getX(), x, t));
	differenceX += (lastDiff.getX() - (differenceX));


	lastDiff.setY(sdlutils().lerpPrecise(lastDiff.getY(), y, t));
	differenceY += (lastDiff.getY() - (differenceY));

	App::camera.x = differenceX;
	App::camera.y = differenceY;
}
