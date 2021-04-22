#include "EnemyTrigger.h"

void EnemyTrigger::init()
{
	entitytr_ = entity_->getComponent<Transform>();
	assert(entitytr_ != nullptr);
	//Inicializamos las componentes del trigger
	trigger = entity_->getMngr()->addEntity(); //TRIGGER DEL ENEMIGO (rango de deteccion)
	Vector2D triggerpos = entitytr_->getPos();
	trigger->addComponent<Transform>(triggerpos, Vector2D(0, 0), triggersize_.getX(), triggersize_.getY(), 0.0f);
	trigger->addComponent<BoxCollider>(KINEMATIC, ENEMY_ATTACK, ENEMY_ATTACK_MASK, true);
}

Entity* EnemyTrigger::getParent()
{
	return entity_;
}

Entity* EnemyTrigger::getTriggerEntity()
{
	return trigger;
}
