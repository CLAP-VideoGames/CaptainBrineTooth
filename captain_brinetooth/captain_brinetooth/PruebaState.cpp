#include "PruebaState.h"
#include "ecs/Entity.h"
#include "game/Game.h"
#include "utils/Vector2D.h"
#include "ecs/Manager.h"

PruebaState::PruebaState(Game* a, Manager* m) : GameState(a,m)
{
	//app->createPlayer(Vector2D(sdlutils().width() / 2.0f, sdlutils().height() / 6.0f), Vector2D(0, 0), Vector2D(200.0f, 200.0f), 0.2f, false, 0.0f);
	//auto* player = manager_->getHandler<Player>();
	//stage.push_back(player); //Para el manejo de los objetos que se duermen
	
	//e->addComponent<FramedImage>(&sdlutils().images().at("Medusa"), 7, 6, 200.0f, 4);

}

void PruebaState::addStateEntityPrueba() {
	auto* e = manager_->addEntity(false);

	
	e->addComponent<Transform>(Vector2D(sdlutils().width() / 2.0f, sdlutils().height() / 6.0f), Vector2D(0, 0), 200.0f, 200.0f, 0.0f);
	//e->addComponent<FramedImage>(&sdlutils().images().at("Medusa"), 7, 6, 200.0f, 4);
}