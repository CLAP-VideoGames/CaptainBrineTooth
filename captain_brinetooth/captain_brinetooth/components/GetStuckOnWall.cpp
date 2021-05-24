#include "GetStuckOnWall.h"

void GetStuckOnWall::init() {
	collider = entity_->getComponent<BoxCollider>();
	entity_->setCollisionMethod(ContactGround);
}

void GetStuckOnWall::ContactGround(b2Contact* contact) {
	//Llamar a otro metodo
	Entity* cosaA = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	Entity* cosaB = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

	if (cosaA != nullptr) {
		if (cosaA->getComponent<MapCollider>() != nullptr) {
			if (cosaB != nullptr) {
				cosaB->getComponent<GetStuckOnWall>()->getStuck();
			}
		}
	}
}

void GetStuckOnWall::getStuck() {
	collider->getBody()->SetType(b2BodyType::b2_kinematicBody);
	collider->setSpeed(Vector2D());
}