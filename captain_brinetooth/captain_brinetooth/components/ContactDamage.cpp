#include "ContactDamage.h"
#include "Enemy_Health.h"
#include "SkillTree.h"
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
	Entity* player = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	if (bullet != nullptr) {
		if (bullet->getComponent<ContactDamage>() != nullptr) {
			if (player != nullptr) {
				if (player->getComponent<Player_Health>() != nullptr)
					bullet->getComponent<ContactDamage>()->makeDamage();
			}
		}
		else {
			bullet = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
			player = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
			if (bullet != nullptr) {
				if (bullet->getComponent<ContactDamage>() != nullptr) {
					if (player != nullptr) {
						if (player->getComponent<Player_Health>() != nullptr)
							bullet->getComponent<ContactDamage>()->makeDamage();
					}
				}
			}
		}
	}
}
void ContactDamage::makeDamage()
{
	Entity* p = entity_->getMngr()->getHandler<Player>();
	Player_Health* pH = p->getComponent<Player_Health>(); //El jugador pierde una vida;

	if (!pH->getInvulnerable())
	{
		pH->loseLife();
 		SkillTree* sklTree = p->getComponent<SkillTree>();
		if (sklTree->hasSkill(Spines)){
			float damagePoints = sklTree->getCounterAttackPercentage();
			Enemy_Health* enemyH = enemy_->getComponent<Enemy_Health>();
			if (enemyH) {
				enemyH->loseLife(damagePoints * enemyH->getHealth());
			}
		}
	}
}
