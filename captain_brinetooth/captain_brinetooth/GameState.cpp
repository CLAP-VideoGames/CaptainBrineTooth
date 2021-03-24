#include "GameState.h"
#include "ecs/Manager.h"

GameState::~GameState() {
	//Para borrar las entidades , se pueden poner a todas las entidades del juego a dormir para que el manager 
	//Se encargue de borrarlas , el unico problema seria que por ejemplo cada vez que accedes al menu deberia de ser uno nuevo 

	for (auto o : stage)
	{
		o->gotoSleep();
	}
	stage.clear();
}
void GameState::render() const {
	manager_->render();
}
void GameState::update() {
	manager_->update();
}
