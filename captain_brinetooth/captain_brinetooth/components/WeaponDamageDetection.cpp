#include "WeaponDamageDetection.h"

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
	enemy->getComponent<Enemy_Health>()->loseLife(damageToApply_, typeOfDamage_);
	if (destroyOnCollision_)
		entity_->setActive(false);
}
