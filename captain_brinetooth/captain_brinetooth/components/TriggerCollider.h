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

class TriggerCollider : public Component
{
public:
	/// <summary>
	/// BoxTriggerCollider
	/// </summary>
	/// <param name="id"></param>
	/// <param name="collisionLayer"></param>
	/// <param name="collisionMask"></param>
	/// <param name="offsetTrigger"></param>
	/// <param name="triggersize"></param>
	TriggerCollider(std::string id, const uint16& collisionLayer = 0x0001, const uint16& collisionMask = 0xFFFF, Vector2D offsetTrigger = Vector2D(0.0, 0.0), Vector2D triggersize = Vector2D(500.0f, 500.0f)) :
		id_(id), colLayer_(collisionLayer), colMask_(collisionMask), offsetTrigger_(offsetTrigger), triggersize_(triggersize) {};
	virtual ~TriggerCollider() {};
	void init() override;
	void update() override;

	Entity* getParent();	//Metodo para obtener las componentes del padre en las colisiones
	Entity* getTriggerEntity();	//Metodo para obtener el propio trigger

	/// <summary>
	/// Identificador del trigger, devuelve un string
	/// </summary>
	const std::string& getID();
protected:
	Entity* trigger;
	Vector2D offsetTrigger_;
	Vector2D triggersize_;
	Transform* entitytr_;
	uint16 colLayer_, colMask_;
	std::string id_;
};
