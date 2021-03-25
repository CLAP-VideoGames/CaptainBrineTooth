#pragma once
#include "GameState.h"
class Entity;

class PruebaState :public GameState
{
public:

	PruebaState(Game* a, Manager* m);
	void addStateEntityPrueba();

};

