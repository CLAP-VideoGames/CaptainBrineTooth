#include "PruebaState.h"
#include "../ecs/Entity.h"
#include "../game/Game.h"
#include "../utils/Vector2D.h"
#include "../ecs/Manager.h"
#include "../components/Transform.h"
#include "../components/BoxCollider.h"
#include "../game/CollisionLayers.h"
#include "../components/AnimBlendGraph.h"
#include "../components/EnemyTrigger.h"
#include "../components/FringeHeadAtack.h"
#include "../components/ElfSharkAttack.h"
#include "../components/Enemy_Health.h"
using namespace ColLayers;

PruebaState::PruebaState(Game* a ,b2World* mundo, SoundManager* snd) : GameState(a,mundo,snd)
{
	//app->createPlayer(Vector2D(sdlutils().width() / 2.0f, sdlutils().height() / 6.0f), Vector2D(0, 0), Vector2D(200.0f, 200.0f), 0.2f, false, 0.0f);
	//auto* player = manager_->getHandler<Player>();
	//stage.push_back(player); //Para el manejo de los objetos que se duermen
	
	//e->addComponent<FramedImage>(&sdlutils().images().at("Medusa"), 7, 6, 200.0f, 4);
	//app->createBackGround("Square", 11, 11);
	app->createLevel0();

	Game::Config playerConfig {};
	playerConfig.pos = Vector2D(sdlutils().width() / 2.5f, sdlutils().height() / 8.0f);
	playerConfig.vel = Vector2D(0,0);
	playerConfig.size = Vector2D(200.0f, 200.0f);
	playerConfig.friction = 0.2f;
	playerConfig.physicType = KINEMATIC;
	playerConfig.fixedRotation = true;
	playerConfig.rotation = 0.0f;

	app->createPlayer(playerConfig);
}

void PruebaState::init() {
	//Añadir el enemigo 

	//auto* enemy = app->createBasicEntity(Vector2D(sdlutils().width() / 2.5f, sdlutils().height() / 8.0f), Vector2D(50.0f, 50.0f),0.0f, Vector2D(0,0));
	//Vector2D pos, Vector2D vel, float width, float height,float rotation
#pragma region FringeHead
	/*auto* enemy = manager_->addEntity();
	Transform* t= enemy->addComponent<Transform>(Vector2D(sdlutils().width() / 1.7f, sdlutils().height() / 1.65f), Vector2D(0, 0), 70.0f, 70.0f, 0.0f);
	enemy->addComponent<BoxCollider>(STATIC, ENEMY, ENEMY_MASK);
	AnimBlendGraph* anim_controller = enemy->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("idle", &sdlutils().images().at("Medusa"), 7, 6, 38, 40, -1, 0, 37);
	enemy->addComponent<FringeHeadAtack>();
	enemy->addComponent<Enemy_Health>(200);*/
#pragma endregion

	//Creacion de un componente que tenga un metodo estatico para asignarselo a la colision del trigger del enemigo con el jugador 

	Game::Config elfShark{};
	elfShark.pos = Vector2D(sdlutils().width() * 1.6f, sdlutils().height() * 0.3f);
	elfShark.vel = Vector2D(0, 0);
	elfShark.size = Vector2D(180.0f, 180.0f);
	elfShark.friction = 0.2f;
	elfShark.physicType = KINEMATIC;
	elfShark.fixedRotation = true;
	elfShark.rotation = 0.0f;
	elfShark.col = ENEMY;
	elfShark.colMask = ENEMY_MASK;

	app->createElfShark(elfShark);


	// if(tiempo pasado + tiempo minimo < tiempo actual)
	// Generar la bala
	// tiempo pasado = tiempo actual
	//

}

