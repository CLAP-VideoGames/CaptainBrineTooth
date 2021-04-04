#include "ContactDamage.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
#include "../ecs/Manager.h"
#include "Player_Health.h"



void ContactDamage::init()
{
	entity_->setCollisionMethod(callDamage);
}
void ContactDamage::callDamage(b2Contact* contact)
{
	Entity* bullet = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	if (bullet != nullptr)bullet->getComponent<ContactDamage>()->makeDamage();
}
void ContactDamage::makeDamage()
{
	entity_->getMngr()->getHandler<Player>()->getComponent<Player_Health>()->loseLife(); //El jugador pierde una vida
}
