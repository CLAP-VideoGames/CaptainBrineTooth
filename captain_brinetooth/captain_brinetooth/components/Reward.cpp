#include "Reward.h"
#include "../ecs/Entity.h"
#include "BoxCollider.h"

Reward::Reward()
{
	catched = false;
}
void Reward::init()
{
	//tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);
	//entity_->getComponent<BoxCollider>()->setSpeed();
}
void Reward::update()
{
	if (!catched)
	{


	}






}
void Reward::baitCollision(b2Contact* contact)
{
	Entity* entity = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	//if (entity != nullptr) entity->getComponent<Reward>()->baitCatched((Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer);
}
void Reward::baitCatched(Entity* h)
{
	
	hook = h; //When collide with hook , we save its reference in order to
	Vector2D hookpos = hook->getComponent<BoxCollider>()->getPhysicalPos();
	entity_->getComponent<BoxCollider>()->setSpeed(0);
	//Modificamos la posicion de la entidad para que se situe donde esta el gancho 
	entity_->getComponent<BoxCollider>()->setPhysicalTransform(hookpos.getX(), hookpos.getY(), 0);
	catched = true;
}
void Reward::checkLimits()
{



}
