#include "FixedCameraPosition.h"
#include "../game/Game.h"


void FixedCameraPosition::init()
{
	entityT_ = entity_->getComponent<Transform>();
	assert(entityT_ != nullptr);

	bC_ = entity_->getComponent<BoxCollider>();
}

void FixedCameraPosition::update(){

	//Si no tiene el componente de BoxCollider, es decir que no es collisionable, luego, actualizamos su posicion del transform
	if (bC_ != nullptr){
		bC_->actPhyscialPos(-Game::camera.x, -Game::camera.y);
	}
	else{
		entityT_->getPos().set(entityT_->getPos().getX() - Game::camera.x, entityT_->getPos().getY() - Game::camera.y);
	}

}

