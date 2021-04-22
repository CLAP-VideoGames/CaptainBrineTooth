#include "TrigggerCollider.h"

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

Entity* TriggerCollider::getParent()
{
	return entity_;
}

Entity* TriggerCollider::getTriggerEntity()
{
	return trigger;
}

const std::string& TriggerCollider::getID() const
{
	return id_;
}
