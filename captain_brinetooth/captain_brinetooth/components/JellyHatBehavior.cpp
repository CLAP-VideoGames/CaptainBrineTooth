#include "JellyHatBehavior.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Manager.h"
#include "../game/CollisionLayers.h"
#include "BoxCollider.h"
#include "AnimBlendGraph.h"
#include "AnimBlendGraph.h"
#include "ContactDamage.h"
#include "PlayerController.h"
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

	entitycollider_ = entity_Parent_->getComponent<BoxCollider>();
	assert(entitycollider_ != nullptr);

	anims_ = entity_Parent_->getComponent<AnimBlendGraph>();
	assert(anims_ != nullptr);

	entity_->setCollisionMethod(collisionPlayer);
}

void JellyHatBehavior::update() {
	Vector2D distJugador = entitytr_->getPos() - playertr_->getPos();
	
	float newSize = 100 - ((abs(distJugador.getX()) + abs(distJugador.getY())) * 0.1);
	if (newSize > 0) {
		anims_->render();
		entitytr_->setH(iniH + newSize);
		entitytr_->setW(iniW + newSize);
		anims_->keepProportion("idle", Vector2D(entitytr_->getW(), entitytr_->getH()));
		anims_->scaleAnimation();
		entityhealth_->setHealth(iniHealth + newSize);
		//entitycollider_->Resize(Vector2D(iniH + (newSize / sdlutils().getPPM()), iniW + (newSize / sdlutils().getPPM())));
		entity_->getMngr()->getSoundMngr()->playSoundEffect("crecimiento_medusa", 5000); // Hay que ajustar este valor
	}
	if (entitycollider_->getBody()->GetLinearVelocity().y == 0 && sdlutils().currRealTime() > lastJump + 1500) {
		lastJump = sdlutils().currRealTime();
		entitycollider_->setSpeed(Vector2D(0, 0));
		entitycollider_->applyForce(Vector2D(0, -1), 60.0f);
	}
}

void JellyHatBehavior::collisionPlayer(b2Contact* contact) {
	Entity* player = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	if (player != nullptr) {
		if (player->getMngr()->getHandler<Player>() == player)
			player->getComponent<PlayerController>()->Paralize();
		else {
			player = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
			if (player->getMngr()->getHandler<Player>() == player)
				player->getComponent<PlayerController>()->Paralize();
		}
	}
}