#pragma once
#include "GameState.h"
#include "box2d.h"
class Entity;

class PruebaState :public GameState
{
public:

	PruebaState(Game* a, b2World* mundo);
	void addStateEntityPrueba();

};

