#include "ThornFishAttack.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Manager.h"
#include "../game/CollisionLayers.h"
#include "BoxCollider.h"
#include "AnimBlendGraph.h"
#include "AnimBlendGraph.h"
#include "ContactDamage.h"

ThornFishAttack::~ThornFishAttack()
{
	if (attackTrigger_ != nullptr)
		attackTrigger_->setActive(false);
}

void ThornFishAttack::init()
{
	entitytr_ = entity_->getComponent<Transform>();
	assert(entitytr_ != nullptr);
	attackTriggerSize_ = Vector2D(entitytr_->getW(), entitytr_->getH() * 1.25);
	cd_attack_time = sdlutils().rand().teCuoto(2500, 5000);
	elapsed_time_lastAttack = sdlutils().currRealTime();
	attack_player = false;
}

void ThornFishAttack::update()
{
	if (entity_->getComponent<Enemy_Health>()->getHealth() > 0) {
		canAttack();
		attack();
		//Acabar el ataque
		if (attackTrigger_ != nullptr
			&& entity_->getComponent<AnimBlendGraph>()->isComplete() && entity_->getComponent<AnimBlendGraph>()->getCurrentAnimation()->getID() == "attack_end") {
			attackTrigger_->setActive(false);
			attackTrigger_ = nullptr;
			entity_->getComponent<AnimBlendGraph>()->setParamValue("Attack", 0);
		}
		if (!attack_player) {
			if (entity_->getMngr()->getHandler<Player>()->getComponent<Transform>()->getPos().getX() > entitytr_->getPos().getX())
				entity_->getComponent<AnimBlendGraph>()->flipX(true);
			else
				entity_->getComponent<AnimBlendGraph>()->flipX(false);
		}
	}
}

void ThornFishAttack::canAttack()
{
	if (cd_attack_time + elapsed_time_lastAttack < sdlutils().currRealTime()) {
		attack_player = true;
		entity_->getComponent<AnimBlendGraph>()->setParamValue("Attack", 1);
		elapsed_time_lastAttack = sdlutils().currRealTime();
		cd_attack_time = sdlutils().rand().teCuoto(2500, 5000);
	}
}

void ThornFishAttack::createAttackTrigger()
{
	float width_offset = entitytr_->getW() * 0.15;
	if (!entity_->getComponent<AnimBlendGraph>()->isFlipX()) width_offset = -width_offset;
	//Crea trigger de ataque
	attackTrigger_ = entity_->getMngr()->addEntity();
	attackTrigger_->addComponent<Transform>(Vector2D(entitytr_->getPos().getX() + width_offset, entitytr_->getPos().getY()),
		Vector2D(0, 0), attackTriggerSize_.getX(), attackTriggerSize_.getY(), 0.0f);
	attackTrigger_->addComponent<BoxCollider>(KINEMATIC, ENEMY_ATTACK, ENEMY_ATTACK_MASK, true);
	attackTrigger_->addComponent<ContactDamage>();
	// Reproducimos el sonido de la electircidad
	entity_->getMngr()->getSoundMngr()->playSoundEffect("mordisco_elfshark", 300);
}

void ThornFishAttack::attack()
{
	if (attack_player
		&& entity_->getComponent<AnimBlendGraph>()->isComplete() && entity_->getComponent<AnimBlendGraph>()->getCurrentAnimation()->getID() == "attack_ini") {
		entity_->getComponent<AnimBlendGraph>()->setParamValue("Attack", 2);
		createAttackTrigger();
		attack_player = false;
	}
}
