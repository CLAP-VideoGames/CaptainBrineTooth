#include "GameState.h"
#include "../ecs/Manager.h" //Manager ya incluye Entity
#include "../game/App.h"
#include "../utils/Vector2D.h"

GameState::GameState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) {
	app = a;
	popped = false;
	manager_ = new Manager(mundo, snd, &popped);
}
GameState::~GameState() {
	//Se tiene que borrar el manager de cada Estado
	delete manager_;
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
