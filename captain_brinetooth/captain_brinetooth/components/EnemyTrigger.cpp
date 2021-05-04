#include "EnemyTrigger.h"

EnemyTrigger::~EnemyTrigger()
{
	if (trigger != nullptr)
		trigger->setActive(false);
}

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

void EnemyTrigger::update()
{
	auto& pos = getParent()->getComponent<BoxCollider>()->getBody()->GetTransform().p;
	trigger->getComponent<BoxCollider>()->getBody()->SetTransform(b2Vec2(pos.x, pos.y), 0.0f);
}

Entity* EnemyTrigger::getParent()
{
	return entity_;
}

Entity* EnemyTrigger::getTriggerEntity()
{
	return trigger;
}
