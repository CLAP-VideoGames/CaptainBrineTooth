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

	entity_->setCollisionMethod(contactWithSomething);
	speed = Vector2D(hookBody->getBody()->GetLinearVelocity().x, 0.65f);
}
void Gancho::update()
{
	if (move)hookMovement();
}
void Gancho::contactWithSomething(b2Contact* contact)
{
	Entity* floor = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	Entity* hook = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	if (hook != nullptr)hook->getComponent<Gancho>()->collisionAnswer(floor);

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



	//if (entity_->getMngr()->getHandler<Rod>() == contactedfloor)
	//{
	//	move = false;
	//	if(hasBaitRef())baitRef->getComponent<Reward>()->giveReward();
	//	//Wee need to turn of all this components , stop the action of fishing 
	//}
	//else speed = Vector2D(hookBody->getBody()->GetLinearVelocity().x, -0.65f);


}