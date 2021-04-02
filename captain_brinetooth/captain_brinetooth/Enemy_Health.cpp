#include "Enemy_Health.h"
#include  "ecs/Entity.h"
#include  "ecs/Manager.h"
#include "FringeHeadAtack.h"

void Enemy_Health::loseLife(int damage)
{
	lifes = lifes - damage;
	if (lifes <= 0)
	{
		entity_->getComponent<FringeHeadAtack>()->getTrigger()->setActive(false);
		entity_->setActive(false);
	}
}