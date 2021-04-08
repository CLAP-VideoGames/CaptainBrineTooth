
#pragma once
class Manager;
class App;
class Entity;
#include <vector>
#include "box2d.h"
#include "../components/SoundManager.h"


class GameState
{
protected:
	
	Manager* manager_;
	App* app;
	SoundManager* soundController;
	std::vector<Entity*> stage;

	GameState(App* a, b2World* mundo, SoundManager* snd);



public:
	virtual ~GameState();
	virtual void render() const;
	virtual void update();
	virtual void refresh();
	virtual void init() {};
	Manager* getMan();
};

