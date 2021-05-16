#include "DestroyOnCollision.h"

void DestroyOnCollision::init()
{
	entity_->setCollisionMethod(destroyOnCollision);
}

void DestroyOnCollision::destroyOnCollision(b2Contact* contact)
{
	Entity* bodyA = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	if (bodyA != nullptr) {
		if (bodyA->hasComponent<DestroyOnCollision>())
			bodyA->setActive(false);
		else {
			bodyA = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
			if (bodyA->hasComponent<DestroyOnCollision>())
				bodyA->setActive(false);
		}
	}
}
