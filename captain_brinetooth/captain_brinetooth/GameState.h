
#pragma once
class Manager;
class Game;
class Entity;
#include <vector>

class GameState
{
protected:
	
	Manager* manager_;
	Game* app;
	std::vector<Entity*> stage;

	GameState(Game* a, Manager* m) { app = a; manager_ = m; };



public:
	virtual ~GameState();
	virtual void render() const;
	virtual void update();
	virtual void refresh();
	//void setManager(Manager* m) { manager_ = m; };
};

