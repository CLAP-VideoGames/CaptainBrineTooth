#include "GameState.h"
#include "../ecs/Manager.h" //Manager ya incluye Entity
#include "../game/App.h"
#include "../utils/Vector2D.h"

GameState::GameState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) {
	app = a;
	manager_ = new Manager(mundo); 
	soundController = snd;
}
GameState::~GameState() {
	//Para borrar las entidades , se pueden poner a todas las entidades del juego a dormir para que el manager 
	//Se encargue de borrarlas , el unico problema seria que por ejemplo cada vez que accedes al menu deberia de ser uno nuevo 
	manager_->desactiveEnteties();
	//delete manager_;
}

Entity* GameState::createBasicEntity(const Vector2D& pos, const Vector2D& size, const float& rotation, const Vector2D& vel){
	auto* entity_ = manager_->addEntity();
	entity_->addComponent<Transform>(pos, vel, size.getX(), size.getY(), rotation);
	return entity_;
}

void GameState::render() const {
	manager_->render();
}
void GameState::update() {
	manager_->update();
}
void GameState::refresh() {
	manager_->refresh();
}
