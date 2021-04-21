#include "Gancho.h"
#include "../sdlutils/SDLUtils.h"
#include "Reward.h"
#include "PescaController.h"

Gancho::Gancho()
{

	move = true;
	contactid = 1; //This means that fishing starts now , and that hook hasnt collided with anithing
}

void  Gancho::init()
{
	hookBody = entity_->getComponent<BoxCollider>();
	assert(hookBody != nullptr);
	//hookBody->getBody()->SetGravityScale(0.0f);
	//No se Exactamente donde va para poder hacer el timer


	speed = Vector2D(hookBody->getBody()->GetLinearVelocity().x, 0.65f);
	hookMovement();
	entity_->setCollisionMethod(contactWithSomething);
	baitRef = nullptr; //No ref of cattched bait 
}
void Gancho::update()
{
	if (baitRef != nullptr)
	{
		//Si ha tocado con el tope de la cuerda
		if (contactid != 3)
		{
			speed = Vector2D(hookBody->getBody()->GetLinearVelocity().x, -0.65f);
			hookMovement();
		}
	
	}
}
void Gancho::contactWithSomething(b2Contact* contact)
{
	Entity* hook = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	if (hook != nullptr) {
		if (hook->getComponent<Gancho>() != nullptr) {
			hook->getComponent<Gancho>()->collisionAnswer((Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer);
		}
		else {
			hook = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
			if (hook != nullptr) hook->getComponent<Gancho>()->collisionAnswer((Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer);
		}
	}



}
bool Gancho::hasBaitRef()
{
	if (baitRef != nullptr)
	{
		contactid = 2; //Movimiento hacia arriba de la cuerda
		return true;
	}
	else return false;
	
}

void Gancho::hookMovement()
{
	//Speed has now a value in order to move again 
	hookBody->setSpeed(speed);
}
void Gancho::collisionAnswer(Entity* contactedfloor)
{
	//Here we need to check if its the top of the rod (It Would have a Rod component name or handler) or the floor
   //If it is on floor , speed= -speed 
   //If its on top of the rod -> We need to detect if there is bait trapped , if yes, i will give reward to player 
   //And movement will stop

	//If we contact with the top of the rod we give reward to the player , using other component that has the ObjectReward

	if (entity_->getMngr()->getHandler<Rod>() == contactedfloor)// Si con lo que se ha colisionado es el tope de la cuerda 
	{
		contactid = 3; //Paramos el movimiento de la cuerda
		speed = Vector2D(hookBody->getBody()->GetLinearVelocity().x, 0);
		hookMovement();
		if (hasBaitRef())
		{
			baitRef->getComponent<Reward>()->giveReward();
		}

	}
	else if (contactedfloor->getComponent<PescaController>() == nullptr) //Esto es para que solo detecte el suelo 
	{
		contactid = 2; //Movimiento hacia arriba de la cuerda
		speed = Vector2D(hookBody->getBody()->GetLinearVelocity().x, -0.65f);
		hookMovement();
	}



}