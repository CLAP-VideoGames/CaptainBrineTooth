#include "Cuerda.h"
#include "Gancho.h"

Cuerda::Cuerda(Entity* h) { hook = h; };
void Cuerda::init() {
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);
	collider_ = entity_->getComponent<BoxCollider>();
	assert(collider_ != nullptr);
	collider_->getFixture()->SetSensor(true);
	
}
void Cuerda::update() {
	
	if (hook->getComponent<Gancho>()->getContactId() == 1) cuerdaDownMovement(); //When the action of fishing starts 
	else if (hook->getComponent<Gancho>()->getContactId() == 2)cuerdaUpMovement(); //When we have collided with floor
	else //When we reach top of the road 
	{
		collider_->setSpeed(Vector2D(0, 0));
	}

}
void Cuerda::cuerdaDownMovement() {
	collider_->setSpeed(Vector2D(collider_->getBody()->GetLinearVelocity().x, 0.33f));
	tr_->setH(tr_->getH() + 2.2f);
}
void Cuerda::cuerdaUpMovement() {
	collider_->setSpeed(Vector2D(collider_->getBody()->GetLinearVelocity().x, -0.33f));
	tr_->setH(tr_->getH() - 2.2f);
}