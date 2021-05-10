#include "Reward.h"
#include "BoxCollider.h"
#include "Gancho.h"
#include "../game/App.h"
#include "Transform.h"
#include "AnimBlendGraph.h"
#include "Inventory.h"
#include "../game/App.h"
#include "../states/StateMachine.h"
#include "../states/PescaState.h"

Reward::Reward(int w, Entity* p, App* a)
{
	weapontoGive = (PosibleWeapons)w;
	catched = false;
	playerRef = p;
	app = a;
	//Falta incializar los limites para tener una referencia cuando el cebo se mueva 
}
void Reward::init()
{
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);

	collider_ = entity_->getComponent<BoxCollider>();
	assert(collider_ != nullptr);

	collider_->getBody()->SetGravityScale(0.0f);
	collider_->setSpeed(Vector2D(-App::camera_Zoom_Out, 0));
	collider_->setPhysicalTransform(tr_->getPos().getX(), tr_->getPos().getY(), 0.0f);
	entity_->setCollisionMethod(baitCollision);
}
void Reward::update()
{

	if (!catched) //Bait remains moving if not catched 
	{
		if (tr_->getPos().getX() < tr_->getW())
			adjustIfLimits();
	}
	else
	{
		b2Vec2 hookpos = hook->getComponent<BoxCollider>()->getBody()->GetTransform().p; //We obtain hook position in order to set bait position
		entity_->getComponent<BoxCollider>()->getBody()->SetTransform(b2Vec2(hookpos.x, hookpos.y), 0);
		//Añadir algo para que no colisione

	}

}
void Reward::baitCollision(b2Contact* contact)
{
	//if (bait != nullptr)
	//{
	//	Entity* hook = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	//	if (!hook->getComponent<Gancho>()->hasBaitRef()) bait->getComponent<Reward>()->baitCatched(hook);
	//	//if true, hook already has catched bait so we ignore collision
	//}

	Entity* bait = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	if (bait != nullptr) {
		if (bait->getComponent<Reward>() != nullptr) {
			Entity* hook = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
			if (hook->getComponent<Gancho>() != nullptr)
			{
				if (!hook->getComponent<Gancho>()->hasBaitRef())bait->getComponent<Reward>()->baitCatched(hook);
			}
		}
		else {

			bait = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
			Entity* hook = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
			if (hook->getComponent<Gancho>() != nullptr)
			{
				if (!hook->getComponent<Gancho>()->hasBaitRef()) bait->getComponent<Reward>()->baitCatched(hook);
			}

		}
	}
}
void Reward::baitCatched(Entity* h)
{
	hook = h; //Now we obtain the reference of the hook

	hook->getComponent<Gancho>()->setBaitReference(entity_); //Now hook has reference of the catchedBait

	Vector2D hookpos = hook->getComponent<BoxCollider>()->getPhysicalPos(); //We obtain hook position in order to set bait position
	entity_->getComponent<BoxCollider>()->setSpeed(Vector2D(0, 0)); //Now bait doesnt move and gets stick in the same position as hook
	//Modificamos la posicion de la entidad para que se situe donde esta el gancho 
	entity_->getComponent<BoxCollider>()->setPhysicalTransform(hookpos.getX(), hookpos.getY(), 0);
	//Now bait is trigger
	entity_->getComponent<BoxCollider>()->getFixture()->SetSensor(true);

	//Reward gets smaller when catched by hook 
	Transform* baitTransform = entity_->getComponent<Transform>(); //We get bait transform
	baitTransform->setW(baitTransform->getW() / 2);
	baitTransform->setW(baitTransform->getH() / 2);

	catched = true;
}
void Reward::adjustIfLimits()
{
	//We have to check limits in orden to set position if neccesary
	//If not  we just use speed in x in order to move it 
	tr_->getPos().setX(sdlutils().width() * App::camera_Zoom_Out);
	collider_->setPhysicalTransform(tr_->getPos().getX(), tr_->getPos().getY(), 0.0f);
}
void Reward::giveReward()
{
	//Del 0- hasta nArmas = armas del juego
	//De nArmas+1 hasta 20-> monedas
	//Si no , es una piedra y directamente no se da nada 
	if ((int)weapontoGive < 10)
	{
		playerRef->getComponent<Inventory>()->addWeapon((int)weapontoGive);
		//Cambiar animacion de player
	}
	//As finished fishing we go back to playstate 


	GameState* pState = app->getStateMachine()->currentState();

	pState->getMngr()->getSoundMngr()->resumeMainMusic();

	app->getStateMachine()->popState();
}