#include "PruebaState.h"
#include "ecs/Entity.h"
#include "game/Game.h"
#include "utils/Vector2D.h"
#include "ecs/Manager.h"
#include "components/Transform.h"
#include "components/BoxCollider.h"
#include "CollisionLayers.h"
#include "components/AnimBlendGraph.h"
#include "FringeHeadAtack.h"
#include "Enemy_Health.h"
#include "components/SoundManager.h"

using namespace ColLayers;

PruebaState::PruebaState(Game* a ,b2World* mundo, SoundManager* snd) : GameState(a,mundo,snd)
{
	auto* soundController = manager_->addEntity();
	soundController->addComponent<SoundManager>(75, "Menu");

	addStateEntityPrueba();
	/*app->createPlayer(Vector2D(sdlutils().width() / 2.0f, sdlutils().height() / 6.0f), Vector2D(0, 0), Vector2D(200.0f, 200.0f), 0.2f, false, 0.0f);
	auto* player = manager_->getHandler<Player>();
	stage.push_back(player); //Para el manejo de los objetos que se duermen
	
	//e->addComponent<FramedImage>(&sdlutils().images().at("Medusa"), 7, 6, 200.0f, 4);
	app->createBackGround("Square", 11, 11);
	app->createLevel0();
	app->createPlayer(Vector2D(sdlutils().width() / 2.0f, sdlutils().height() / 8.0f), Vector2D(0, 0), Vector2D(200.0f, 200.0f), 0.2f, true, 0.0f);*/
}

void PruebaState::addStateEntityPrueba() {
	
	//Añadir el enemigo 
	auto* enemy = manager_->addEntity();
	Transform* t= enemy->addComponent<Transform>(Vector2D(sdlutils().width() / 1.7f, sdlutils().height() / 1.65f), Vector2D(0, 0), 70.0f, 70.0f, 0.0f);
	enemy->addComponent<BoxCollider>(STATIC, ENEMY, ENEMY_MASK);
	AnimBlendGraph* anim_controller = enemy->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("iddle", &sdlutils().images().at("Medusa"), 7, 6, 38, 40, -1, 0, 37);
	enemy->addComponent<FringeHeadAtack>();

}