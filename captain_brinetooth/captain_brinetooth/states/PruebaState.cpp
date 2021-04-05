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
	//app->createLevel0();
	//app->createPlayer(Vector2D(sdlutils().width() / 2.0f, sdlutils().height() / 8.0f), Vector2D(0, 0), Vector2D(200.0f, 200.0f), 0.2f, true, 0.0f);
}

void PruebaState::addStateEntityPrueba() {
	
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
#pragma region ElfShark
	auto* elf1 = manager_->addEntity();
	Transform* t = elf1->addComponent<Transform>(Vector2D(sdlutils().width() * 1.6f, sdlutils().height() * 0.55f), Vector2D(0, 0), 180.0f, 180.0f, 0.0f);
	elf1->addComponent<BoxCollider>(KINEMATIC, ENEMY, ENEMY_MASK);
	AnimBlendGraph* elf1_anim_controller = elf1->addComponent<AnimBlendGraph>();
	elf1_anim_controller->addAnimation("idle", &sdlutils().images().at("Elf_Shark"), 1, 3, 1, 1, -1);
	elf1_anim_controller->addAnimation("attack", &sdlutils().images().at("Elf_Shark"), 1, 3, 3, 8, 0);
	elf1_anim_controller->addTransition("idle", "attack", "Attack", 1, false);
	elf1_anim_controller->addTransition("attack", "idle", "Attack", 0, true);
	elf1->addComponent<Animation>("1", &sdlutils().images().at("Square"), 1, 1, 1, 1, 0);
	auto* trigger_elf1 = elf1->addComponent<EnemyTrigger>(Vector2D(1000.0f, 600.0f));
	trigger_elf1->addTriggerComponent<ElfSharkAttack>(elf1);
	elf1->addComponent<Enemy_Health>(300);
#pragma endregion
	//Creacion de un componente que tenga un metodo estatico para asignarselo a la colision del trigger del enemigo con el jugador 


	// if(tiempo pasado + tiempo minimo < tiempo actual)
	// Generar la bala
	// tiempo pasado = tiempo actual
	//


}