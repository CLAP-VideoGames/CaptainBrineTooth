#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Manager.h"
#include "../game/CollisionLayers.h"
#include "BoxCollider.h"
#include "Transform.h"
#include "ElfSharkAttack.h"
using namespace ColLayers;


class EnemyTrigger : public Component 
{
public:
	EnemyTrigger(Vector2D triggersize = Vector2D(500.0f, 500.0f)) :triggersize_(triggersize) {};
	virtual ~EnemyTrigger() {};
	void init() override {
		trigger = entity_->getMngr()->addEntity(); //TRIGGER DEL ENEMIGO (rango de deteccion)
		entitytr_ = entity_->getComponent<Transform>();
		assert(entitytr_ != nullptr);
		//Inicializamos las componentes del trigger
		Vector2D triggerpos = entitytr_->getPos();
		trigger->addComponent<Transform>(triggerpos, Vector2D(0, 0), triggersize_.getX(), triggersize_.getY(), 0.0f);
		trigger->addComponent<BoxCollider>(STATIC, ENEMY_ATTACK, ENEMY_ATTACK_MASK, true);
		trigger->addComponent<Animation>("1", &sdlutils().images().at("Square"), 1, 1, 1, 1, 0);
		trigger->addComponent<ElfSharkAttack>(entity_);	//Necesario crear un template para anadir componentes
	}; 


	Entity* getParent() { return entity_; }	//Metodo para obtener las componentes del padre en las colisiones
protected:
	Entity* trigger;
	Vector2D triggersize_;
	Transform* entitytr_;
};