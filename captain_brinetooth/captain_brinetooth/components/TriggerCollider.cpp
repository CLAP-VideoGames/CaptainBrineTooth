#include "TriggerCollider.h"

void TriggerCollider::init()
{
	entitytr_ = entity_->getComponent<Transform>();
	assert(entitytr_ != nullptr);
	//Inicializamos las componentes del trigger
	trigger = entity_->getMngr()->addEntity(); //TRIGGER DEL ENEMIGO (rango de deteccion)
	Vector2D triggerpos = Vector2D(entitytr_->getPos().getX() + offsetTrigger_.getX(), entitytr_->getPos().getY() + offsetTrigger_.getY());
	trigger->addComponent<Transform>(triggerpos, Vector2D(0, 0), triggersize_.getX(), triggersize_.getY(), 0.0f);
	trigger->addComponent<BoxCollider>(KINEMATIC, colLayer_, colMask_, true);
}

void TriggerCollider::update()
{
	b2Vec2 speed = entity_->getComponent<BoxCollider>()->getBody()->GetLinearVelocity();
	auto& pos = entity_->getComponent<BoxCollider>()->getBody()->GetTransform().p;
	trigger->getComponent<BoxCollider>()->getBody()->SetTransform(b2Vec2(pos.x + offsetTrigger_.getX(),pos.y + offsetTrigger_.getY()), 0.0f);
	
}

Entity* TriggerCollider::getParent()
{
	return entity_;
}

Entity* TriggerCollider::getTriggerEntity()
{
	return trigger;
}

const std::string& TriggerCollider::getID()
{
	return id_;
}
