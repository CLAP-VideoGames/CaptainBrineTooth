#include "DestroyOnCollision.h"

void DestroyOnCollision::init()
{
	entity_->setCollisionMethod(destroyOnCollision);
}

void DestroyOnCollision::destroyOnCollision(b2Contact* contact)
{
	Entity* entity = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	if (entity != nullptr) {
		if (entity->hasComponent<DestroyOnCollision>())
			entity->setActive(false);
	}
}
