#include "JellyHatBehavior.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Manager.h"
#include "../game/CollisionLayers.h"
#include "BoxCollider.h"
#include "AnimBlendGraph.h"
#include "AnimBlendGraph.h"
#include "ContactDamage.h"
using namespace ColLayers;


void JellyHatBehavior::init()
{
	entitytr_ = entity_Parent_->getComponent<Transform>();
	assert(entitytr_ != nullptr);
	iniH = entity_->getComponent<Transform>()->getH();
	iniW = entity_->getComponent<Transform>()->getW();

	entityhealth_ = entity_Parent_->getComponent<Enemy_Health>();
	iniHealth = entityhealth_->getHealth();

	playertr_ = entity_->getMngr()->getHandler<Player>()->getComponent<Transform>();
	assert(playertr_ != nullptr);
}

void JellyHatBehavior::update() {
	Vector2D distJugador = entitytr_->getPos() - playertr_->getPos();
	
	float newSize = 500 - (abs(distJugador.getX()) + abs(distJugador.getY()));
	if (newSize > 0) {
		entitytr_->setH(iniH + newSize);
		entitytr_->setW(iniW + newSize);
		entityhealth_->setHealth(iniHealth + newSize);
	}
}