#pragma once
#include "ecs/Component.h"
#include "ecs/Entity.h"
#include "utils/Vector2D.h"

class FringeHeadAtack : public Component
{
public:
	FringeHeadAtack();
	virtual ~FringeHeadAtack() {};
	void init() override;
	void update() override {};
	void render() override {};

protected:
	Transform* tr;
	Entity* trigger;
	Vector2D triggersize; //Para el tamaño del trigger 


};

