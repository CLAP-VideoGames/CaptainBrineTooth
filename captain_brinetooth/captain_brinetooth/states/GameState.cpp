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

SDL_Rect GameState::ScaleSDL_Rect(Texture* text, const Vector2D& pos, float zoomFactor, float sizeFactor, bool centered){
	SDL_Rect rect;

	rect.w = text->width() * zoomFactor * sizeFactor;
	rect.h = text->height() * zoomFactor * sizeFactor;

	if (centered){
		rect.x = pos.getX() - (rect.w / 2);
		rect.y = pos.getY();
	}
	else{
		rect.x = pos.getX();
		rect.y = pos.getY();
	}
	return rect;
}
