#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Manager.h"
#include "../game/CollisionLayers.h"
#include "BoxCollider.h"
#include "Transform.h"
using namespace ColLayers;


class EnemyTrigger : public Component 
{
public:
	EnemyTrigger(Vector2D triggersize = Vector2D(500.0f, 500.0f)) :triggersize_(triggersize) {};
	virtual ~EnemyTrigger();
	void init() override; 
	void update() override;

	Entity* getParent();	//Metodo para obtener las componentes del padre en las colisiones

	Entity* getTriggerEntity();	//Metodo para obtener el propio trigger
	
	//Anade cualquier componente necesario al trigger Entity
	template<typename T, typename ...Ts>
	void addTriggerComponent(Ts&&... args) { trigger->addComponent<T>(std::forward<Ts>(args)...); }

protected:
	Entity* trigger;
	Vector2D triggersize_;
	Transform* entitytr_;
};