
#pragma once
class Manager;
class Game;
class Entity;
#include <vector>
#include "box2d.h"
#include "components/SoundManager.h"


class GameState
{
protected:
	
	Manager* manager_;
	Game* app;
	SoundManager* soundController;
	std::vector<Entity*> stage;

	GameState(Game* a, b2World* mundo, SoundManager* snd);



public:
	virtual ~GameState();
	virtual void render() const;
	virtual void update();
	virtual void refresh();
	Manager* getMan();
};

