#include "MenuState.h"
#include "ecs/Entity.h"
#include "game/Game.h"
#include "utils/Vector2D.h"
#include "ecs/Manager.h"
#include "components/Animation.h"
#include "components/AnimBlendGraph.h"
#include "components/Button.h"

MenuState::MenuState(Game* a, b2World* mundo) : GameState(a, mundo)
{
	//app->createPlayer(Vector2D(sdlutils().width() / 2.0f, sdlutils().height() / 6.0f), Vector2D(0, 0), Vector2D(200.0f, 200.0f), 0.2f, false, 0.0f);
	//auto* player = manager_->getHandler<Player>();
	//stage.push_back(player); //Para el manejo de los objetos que se duermen

	//e->addComponent<FramedImage>(&sdlutils().images().at("Medusa"), 7, 6, 200.0f, 4);
	auto* newP = manager_->addEntity();
	newP->addComponent<Transform>(Vector2D(100, 100), Vector2D(0, 0), 600.0f, 200.0f, 0.0f);
	auto* anim_controller = newP->addComponent<AnimBlendGraph>();

	anim_controller->addAnimation("waves", &sdlutils().images().at("Square"), 11, 11, 1, 1, -1);

	auto botonIniciar = newP->addComponent<Button>(&sdlutils().images().at("boton"),changeToGame);

	//anim_controller->addAnimation("run", &sdlutils().images().at("Player_run"), 4, 5, 20, 24, -1);
}

void MenuState::addStateEntityMenu() {
	/*auto* e = manager_->addEntity(false);


	e->addComponent<Transform>(Vector2D(sdlutils().width() / 2.0f, sdlutils().height() / 6.0f), Vector2D(0, 0), 200.0f, 200.0f, 0.0f);
	//e->addComponent<FramedImage>(&sdlutils().images().at("Medusa"), 7, 6, 200.0f, 4);*/
}

void MenuState::changeToGame()
{
	std:cout << "Soy tremendamente inutil :D";
}