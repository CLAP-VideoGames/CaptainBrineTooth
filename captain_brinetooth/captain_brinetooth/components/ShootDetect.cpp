#include "ShootDetect.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Component.h"
#include "../ecs/Manager.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
#include "AnimBlendGraph.h"
#include "DisableOnExit.h"
#include "../game/CollisionLayers.h"
#include "ContactDamage.h"
using namespace ColLayers;


void ShootDetect::init()
{
	minTime = 4000;
	passedTime = 0;
	entity_->setCollisionMethod(shoot);
	entity_->setEndCollisionMethod(unShoot);
	shootenabled = false;
	offsetbala = 8.0f;
	velocity = 1;
}

void ShootDetect::shoot(b2Contact* contact)
{
	Entity* enemy = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	if (enemy != nullptr)enemy->getComponent<ShootDetect>()->shootatTime();


}
void ShootDetect::shootatTime()
{
	shootenabled = true;
}
void ShootDetect::update()
{
	if (shootenabled)
	{
		if (passedTime + minTime < sdlutils().currRealTime())
		{

			//Instanciar una entidad bala que cuando salga de la pantalla se destruya 
			createBullet();

			passedTime = sdlutils().currRealTime();

		}
	}
}
void ShootDetect::unShoot(b2Contact* contact) {
	Entity* enemy = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	if (enemy != nullptr)enemy->getComponent<ShootDetect>()->setShootFalse();

}
void ShootDetect::setShootFalse() { shootenabled = false; }

//if(tiempo pasado + tiempo minimo < tiempo actual)
// Generar la bala
// tiempo pasado = tiempo actual
//
void ShootDetect::createBullet()
{
	Entity* bullet = entity_->getMngr()->addEntity();//Añadimos una bala 

		   //Pos del obj disparador- pos del jugador / Normalizado
	Transform* playertransform = entity_->getMngr()->getHandler<Player>()->getComponent<Transform>();

	Vector2D bulletpos;
	bulletpos.set(Vector2D(enemytransform->getPos().getX()+offsetbala, enemytransform->getPos().getY()));


	Vector2D bulletvel= (playertransform->getPos()-enemytransform->getPos());
	bulletvel= bulletvel.normalize();
	//Si el player esta a la derecha 


	//Dotamos a la bala de todos los componentes 
	bullet->addComponent<Transform>(bulletpos, Vector2D(0,0), 10.0f, 10.0f, 0.0f);
	AnimBlendGraph* anim_controller = bullet->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("iddle", &sdlutils().images().at("Square"), 1, 1, 1, 1, 1);
	bullet->addComponent<DisableOnExit>();
	bullet->addComponent<BoxCollider>(DYNAMIC, ENEMY_ATTACK, ENEMY_ATTACK_MASK);
	bullet->getComponent<BoxCollider>()->applyForce(bulletvel,velocity);
	bullet->addComponent<ContactDamage>();
}