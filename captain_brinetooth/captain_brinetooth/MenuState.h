#pragma once
#include "GameState.h"
#include "box2d.h"
class Entity;

class MenuState : public GameState
{
public:
	MenuState(Game* a,b2World* mundo);
	void addStateEntityMenu();

};

