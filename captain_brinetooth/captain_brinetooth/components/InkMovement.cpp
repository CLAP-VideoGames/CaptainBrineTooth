#include "InkMovement.h"

InkMovement::InkMovement(bool lookingLeft)
{
	lookingLeft_ = lookingLeft;
	initialVelocity = 2;
	deacceleration = 0.985;
}

void InkMovement::init() {
	box = entity_->getComponent<BoxCollider>();

	box->getBody()->SetGravityScale(box->getBody()->GetGravityScale()*0.2f);

	float xVel;

	if (lookingLeft_)
		xVel = -1;
	else
		xVel = 1;

	currentVel = initialVelocity;
	dir = xVel;

	box->setSpeed(Vector2D(xVel * initialVelocity, 0));
}

void InkMovement::update() {
	currentVel *= deacceleration;
	box->setSpeed(Vector2D(dir * currentVel, 0));
}