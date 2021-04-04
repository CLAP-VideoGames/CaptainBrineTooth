
#pragma once
class Manager;
class Game;
class Entity;
#include <vector>
#include "box2d.h"

class GameState
{
protected:
	
	Manager* manager_;
	Game* app;
	std::vector<Entity*> stage;

	GameState(Game* a, b2World* mundo);



public:
	virtual ~GameState();
	virtual void render() const;
	virtual void update();
	virtual void refresh();
	Manager* getMan();
};

