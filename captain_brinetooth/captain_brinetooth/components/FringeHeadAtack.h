#pragma once
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../utils/Vector2D.h"
#include "Transform.h"

class FringeHeadAtack : public Component
{
public:
	FringeHeadAtack();
	virtual ~FringeHeadAtack() { trigger->setActive(false); };
	void init() override;
	void update() override {};
	void render() override {};

protected:
	Transform* tr;
	Entity* trigger; //ACordarme de hacer un metodo para obtener el trigger para destruirlo cuando elñ enemigo pierda toda la vida 
	Vector2D triggersize; //Para el tamaño del trigger 
	Transform* enemytr;

};

