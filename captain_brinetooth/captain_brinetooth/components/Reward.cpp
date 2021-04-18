#include "Reward.h"
#include "../ecs/Entity.h"
#include "BoxCollider.h"

Reward::Reward()
{

}
void Reward::init()
{
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);
	//entity_->getComponent<BoxCollider>()->setSpeed();
}
void Reward::update()
{







}
void Reward::baitCollision(b2Contact* contact)
{
	Entity* entity = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	if (entity != nullptr) entity->getComponent<Reward>()->baitCatched((Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer);
}
void Reward::baitCatched(Entity* hook)
{
	Vector2D hookpos = hook->getComponent<BoxCollider>()->getPhysicalPos();
	entity_->getComponent<BoxCollider>()->setSpeed(0);
	//Modificamos la posicion de la entidad para que se situe donde esta el gancho 
	entity_->getComponent<BoxCollider>()->setPhysicalTransform(hookpos.getX(), hookpos.getY(), 0);
}

