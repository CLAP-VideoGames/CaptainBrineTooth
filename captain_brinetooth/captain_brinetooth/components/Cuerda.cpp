#include "Cuerda.h"
Cuerda::Cuerda() {};
void Cuerda::init() {
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);
	collider_ = entity_->getComponent<BoxCollider>();
	assert(collider_ != nullptr);
	collider_->getFixture()->SetSensor(true);
	
}
void Cuerda::update() {
	cuerdaMovement();
}
void Cuerda::cuerdaMovement() {
	collider_->setSpeed(Vector2D(collider_->getBody()->GetLinearVelocity().x, 0.33f));
	tr_->setH(tr_->getH() + 2.2f);
}