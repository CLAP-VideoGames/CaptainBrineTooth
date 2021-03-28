#include "enemyMovement.h"

void EnemyMovement::init() {
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);

	box = entity_->getComponent<BoxCollider>();
	setVel(sdlutils().rand().teCuoto(0, 1));		//Ponemos una velocidad incial aleatoria

}

void EnemyMovement::update() {
	Vector2D v(dir.getX() * vel.getX(), dir.getY() * vel.getY());	//Le damos la vel en cada frame para que no se pare por el rozamiento
	box->setSpeed(v);

	//Solo funciona con el transform
	int w = tr_->getW();
	//Comprobamos los limites de pantalla y cambiamos la direción
	if (tr_->getPos().getX() + w> sdlutils().width()|| tr_->getPos().getX() < 0) {

		box->setSpeed(Vector2D(-box->getBody()->GetLinearVelocity().x, 0));
		dir.setX(-dir.getX());
	}
}

void EnemyMovement::setVel(int velIni) {
	int x, y;
	y = 0;

	if (velIni == 0)x = -1;
	else x = 1;

	dir.setX(x);
	dir.setY(y);

	Vector2D v(x * vel.getX(), y * vel.getY());
	box->setSpeed(v);
}