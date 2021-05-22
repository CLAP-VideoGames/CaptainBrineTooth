#include "WeaponDamageDetection.h"
#include "SkillTree.h"

WeaponDamageDetection::WeaponDamageDetection(int damageToApply, int typeOfDamage, bool destroyOnCollision) : damageToApply_(damageToApply), typeOfDamage_(typeOfDamage), destroyOnCollision_(destroyOnCollision) {
}

WeaponDamageDetection::~WeaponDamageDetection()
{
}

void WeaponDamageDetection::init()
{
	entity_->setCollisionMethod(ContactEnemy);
}

void WeaponDamageDetection::ContactEnemy(b2Contact* contact) {
	//Llamar a otro metodo
	Entity* enemy = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;

	if (enemy != nullptr)
		if (enemy->getComponent<Enemy_Health>() != nullptr) {
			Entity* entidad = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

			if (entidad != nullptr) {
				entidad->getComponent<WeaponDamageDetection>()->ApplyDamage(enemy);
			}
		}
		else {
			enemy = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
			if (enemy->getComponent<Enemy_Health>() != nullptr) {
				Entity* entidad = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;

				if (entidad != nullptr) {
					entidad->getComponent<WeaponDamageDetection>()->ApplyDamage(enemy);
				}
			}
		}
}

void WeaponDamageDetection::ApplyDamage(Entity* enemy) {
	//Obtenemos el SkillTree al estilo Samir
	SkillTree* sklTree = entity_->getMngr()->getHandler<Player>() != nullptr ? entity_->getMngr()->getHandler<Player>()->getComponent<SkillTree>() : nullptr;

	damageToApply_ *= sklTree->getAttackModifier();

	enemy->getComponent<Enemy_Health>()->loseLife(damageToApply_, typeOfDamage_);
	if(entity_->getMngr()->getHandler<Player>()->hasComponent<Player_Health>())
		entity_->getMngr()->getHandler<Player>()->getComponent<Player_Health>()->chargeHeal(damageToApply_);
	if (destroyOnCollision_)
		entity_->setActive(false);
}
