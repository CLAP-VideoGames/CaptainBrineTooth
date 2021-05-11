#include "GetStuckOnWall.h"

void GetStuckOnWall::init() {
	collider = entity_->getComponent<BoxCollider>();
}

void GetStuckOnWall::ContactGround(b2Contact* contact) {
	//Llamar a otro metodo
	Entity* cosaA = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	Entity* cosaB = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

	if (cosaA != nullptr) {
		if (cosaA->getComponent<BoxCollider>() != nullptr) {
			if (cosaA->getComponent<BoxCollider>()->getColLayer() == GROUND) {
				if (cosaB != nullptr) {
					cosaB->getComponent<GetStuckOnWall>()->getStuck();
				}
			}
			else if (cosaB->getComponent<BoxCollider>()->getColLayer() == GROUND) {
				cosaA->getComponent<GetStuckOnWall>()->getStuck();
			}
		}
	}
}

void GetStuckOnWall::getStuck() {
	collider->getBody()->SetType(b2BodyType::b2_kinematicBody);
	collider->setSpeed(Vector2D());
}