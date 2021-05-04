#include "InkMovement.h"

void InkMovement::init() {
	box = entity_->getComponent<BoxCollider>();

	box->getBody()->SetGravityScale(0);

	float xVel;

	if (lookingLeft_)
		xVel = -1;
	else
		xVel = 1;

	box->setSpeed(Vector2D(xVel * initialVelocity, 0));

}