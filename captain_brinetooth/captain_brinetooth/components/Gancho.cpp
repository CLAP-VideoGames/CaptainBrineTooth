#include "Gancho.h"
#include "../sdlutils/SDLUtils.h"
Gancho::Gancho()
{


}

void  Gancho::init()
{
	hookBody = entity_->getComponent<BoxCollider>();
	assert(hookBody != nullptr);
	//hookBody->getBody()->SetGravityScale(0.0f);
	//No se Exactamente donde va para poder hacer el timer

	entity_->setCollisionMethod(contactWithSomething);

	//Here we should give an amount of speed that applied when timeTomove Passes
	//speed= X
	
}
void Gancho::update()
{
	hookMovement();
}
void Gancho::contactWithSomething(b2Contact* contact)
{
	Entity* entity = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	if (entity != nullptr)
	{
		//Here we need to check if its the top of the rod (It Would have a Rod component name or handler) or the floor
		//If it is on floor , speed= -speed 
		//If its on top of the rod -> Check if bool catched = true , 
	
	}

}
bool Gancho::hasBaitRef()
{
	if (baitRef != nullptr) return true;
}

void Gancho::hookMovement()
{
	//Speed has now a value in order to move again 
	hookBody->setSpeed(Vector2D(hookBody->getBody()->GetLinearVelocity().x, 0.65f));
}
