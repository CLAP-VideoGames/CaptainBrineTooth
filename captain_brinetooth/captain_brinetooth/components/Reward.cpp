#include "Reward.h"
#include "BoxCollider.h"
#include "Gancho.h"
#include "Transform.h"
#include "AnimBlendGraph.h"

Reward::Reward()
{
	catched = false;
	//Falta incializar los limites para tener una referencia cuando el cebo se mueva 
}
void Reward::init()
{
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);

	collider_ = entity_->getComponent<BoxCollider>();
	assert(collider_ != nullptr);

	collider_->getBody()->SetGravityScale(0.0f);
	collider_->setSpeed(Vector2D(-2, 0));
	collider_->setPhysicalTransform(tr_->getPos().getX(), tr_->getPos().getY(), 0.0f);
	entity_->setCollisionMethod(baitCollision);
}
void Reward::update()
{

	if (!catched) //Bait remains moving if not catched 
	{
		if (tr_->getPos().getX() < 0) {
			adjustIfLimits();
		}


	}
	else
	{
		Vector2D hookpos = hook->getComponent<BoxCollider>()->getPhysicalPos(); //We obtain hook position in order to set bait position
		entity_->getComponent<BoxCollider>()->setPhysicalTransform(hookpos.getX(), hookpos.getY(), 0); //Set position of our bait
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
				if (!hook->getComponent<Gancho>()->hasBaitRef()) bait->getComponent<Reward>()->baitCatched(hook);
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
	AnimBlendGraph* baitanim = entity_->getComponent<AnimBlendGraph>();
	baitanim->keepProportion("idle", Vector2D(baitTransform->getW() / 2, baitTransform->getH() / 2));
	baitanim->scaleAnimation();

	catched = true;
}
void Reward::adjustIfLimits()
{
	//We have to check limits in orden to set position if neccesary
	//If not  we just use speed in x in order to move it 
	tr_->getPos().set(Vector2D(sdlutils().width() * 2, tr_->getPos().getY()));
	collider_->setPhysicalTransform(tr_->getPos().getX(), tr_->getPos().getY(), 0.0f);

}
void Reward::giveReward()
{
	int pollagordamaster; //Comprobacion de que entra en el metodo
	//We should make a component to give reward
	//Player gets the object 

	//entity_->GetComponent<Premio>()->giveObjecttoPlayer()
	//{
	// return objetAssociated;
	//}
}