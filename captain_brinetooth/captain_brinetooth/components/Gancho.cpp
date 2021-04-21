#include "Gancho.h"
#include "../sdlutils/SDLUtils.h"
#include "Reward.h"
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
}
void Gancho::update()
{
	//if (move)hookMovement();
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
			if(hook != nullptr) hook->getComponent<Gancho>()->collisionAnswer((Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer);
		}
	}
	
	

}
bool Gancho::hasBaitRef()
{
	if (baitRef != NULL) return true;
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



	if (entity_->getMngr()->getHandler<Rod>() != contactedfloor)
	{
		contactid++;
		if (contactid == 3) {
			speed = Vector2D(hookBody->getBody()->GetLinearVelocity().x, 0);
			hookMovement();
		}
		else {
			speed = Vector2D(hookBody->getBody()->GetLinearVelocity().x, -0.65f);
			hookMovement();
		}
		

	}
	/*else {
		move = false;
		if (hasBaitRef())baitRef->getComponent<Reward>()->giveReward();
		//Wee need to turn of all this components , stop the action of fishing 
	}*/ 


}