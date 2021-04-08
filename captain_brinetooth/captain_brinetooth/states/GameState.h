
#pragma once
#include <vector>
#include "box2d.h"
#include "../components/SoundManager.h"

class Manager;
class App;
class Entity;


class GameState
{
protected:
	
	Manager* manager_;
	App* app;
	SoundManager* soundController;
	std::vector<Entity*> stage;




public:
	GameState(App* a, b2World* mundo, SoundManager* snd);
	virtual ~GameState();
	virtual void render() const;
	virtual void update();
	virtual void refresh();
	virtual void init() {};
	Manager* getMan();
};

