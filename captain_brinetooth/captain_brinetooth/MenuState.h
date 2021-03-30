#pragma once
#include "GameState.h"
class Entity;

class MenuState : public GameState
{
public:
	MenuState(Game* a, Manager* m);
	void addStateEntityMenu();

};

