// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../levels/Level0.h"
#include "tmxlite/Map.hpp"
#include "tmxlite/Layer.hpp"
#include "tmxlite/TileLayer.hpp"
#include "../states/GameStateMachine.h"
#include "../states/GameState.h"

#include "../components/Animation.h"
#include "../components/AnimBlendGraph.h"
#include "../components/Rotate.h"
#include "../components/ChainCollider.h"
#include "../components/Bounce.h"
#include "../components/Armas_HUD.h"
#include "../components/Transform.h"
#include "../components/Rectangle.h"
#include "../components/KeyBoardCtrl.h"
#include "../components/Player_Health.h"
#include "..//components/enemyMovement.h"
#include "..//components/PlayerController.h"
#include "..//components/SoundManager.h"
#include "..//components/Chainsaw.h"
#include "../states/PruebaState.h"
#include "..//components/CameraFollow.h"
#include "CollisionLayers.h"
#include "../components/LoseLife.h"
#include "../states/MenuState.h"
#include "..//components/Sword.h"
#include "..//components/Hammer.h"
#include "../components/ElfSharkAttack.h"
#include "../components/EnemyTrigger.h"
#include "..//components/MapProcedural.h"

//tiledmap

const auto MAP_PATH = "assets/maps/levelTest/levelTest.tmx";
SDL_Rect Game::camera = {0 ,0,window.getX(),window.getY()};

using namespace ColLayers;

//Comentario para probar un commit
Game::Game() {
	stateMachine = new GameStateMachine();
	//Creariamos el menu y hariamos un setManager dandole el valor a 
	//Hariamos un push del menu

	b2Vec2 gravity(0.0f, 9.8f);
	world_ = new b2World(gravity);
	SoundManager* sndProvisional = new SoundManager(75, "Menu");

	MenuState* prueba = new MenuState(this, world_, sndProvisional);

	stateMachine->pushState(prueba);
	//Ahora  cuando creemos un nuevo estado hay que hacer un reset del manager del juego poniendo el manager del estado en cuestion
	Manager* a = stateMachine->currentState()->getMan();
	mngr_.reset(a);
}

Game::~Game() {
}

void Game::init() {
	SDLUtils::init("Captain BrineTooth", window.getX(), window.getY(), "assets/config/base.resources.json");


	stateMachine->currentState()->init();
	world_->SetContactListener(&collisionListener);

	auto* soundController = mngr_->addEntity();
	soundController->addComponent<SoundManager>(75, "Menu");

	//PruebaState* prueba = static_cast<PruebaState*>(stateMachine->currentState());
	//prueba->addStateEntityPrueba();
	//createBackGround("Square", 11, 11);
	//createLevel0();

	///*MenuState* aux = static_cast<MenuState*>(stateMachine->currentState());
	//aux->addStateEntityMenu();
	//aux->setSoundController(soundController->getComponent<SoundManager>());*/

	////Caja para hacer testeo con movimiento
	//Config boxConfig {};
	//boxConfig.pos = Vector2D(sdlutils().width() / 5.5f, sdlutils().height() / 7.0f);
	//boxConfig.vel = Vector2D();
	//boxConfig.size = Vector2D(150.0f, 80.0f);
	//boxConfig.friction = 0.5f;
	//boxConfig.fixedRotation = false;
	//boxConfig.isTrigger = false;
	//boxConfig.rotation = 0.0f;
	//boxConfig.physicType = DYNAMIC;
	//boxConfig.col= DEFAULT;
	//boxConfig.col= DEFAULT_MASK;
	//createBoxTest(boxConfig);
	////Crea el suelo
	////createMedusa(Vector2D(sdlutils().width() / 3.0f - 50.0, sdlutils().height() / 2.0f + 60.0f), Vector2D(), Vector2D(200.0f, 200.0f), 0.0f);

	////sdlutils().musics().at(mainMusic).setMusicVolume(volumen++);

	//Config playerConfig {};
	//playerConfig.pos = Vector2D(sdlutils().width() / 2.5f, sdlutils().height() / 8.0f);
	//playerConfig.vel = Vector2D(0,0);
	//playerConfig.size = Vector2D(200.0f, 200.0f);
	//playerConfig.friction = 0.2f;
	//playerConfig.fixedRotation = true;
	//playerConfig.rotation = 0.0f;

	//createPlayer(playerConfig);
}

void Game::start() {
	// a boolean to exit the loop
	exit = false;
	SDL_Event event;

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		ih().clearState();
		while (SDL_PollEvent(&event))
			ih().update(event);

		if (ih().isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}

		world_->Step(1.0f / 60.0f, 6, 2);
		  

		mngr_->update();
		mngr_->refresh();
		//Los estados son capaces de actualizar todo lo suyo gracias a la referencia del manager
		stateMachine->currentState()->update();
		//No tengo muy claro que la camara se actualize en base al juego , en base a cada estado individual
		//O en base a cualquier estado
		stateMachine->currentState()->refresh();

		sdlutils().clearRenderer();
		SDL_RenderSetLogicalSize(sdlutils().renderer(), window.getX() * camera_Zoom_Out, window.getY()* camera_Zoom_Out);
		mngr_->render();
		//stateMachine->currentState()->render();
		sdlutils().presentRenderer();
		
		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}

}
//Metodos propios de game 

void Game::ShakeCamera(int time){
	int aux = 0;
	SDL_Rect aux2 = camera;
	int slow = 0;
	// Movemos la camara de forma random y rapidamente
	for (int i = 0; i < time; i++){
		if (slow % 2 == 0){
			if (aux == 0){
				camera.x -= 15; 
				camera.y -= 15;
				aux++;
			}
			else{
				camera.x += 15;
				camera.y += 15;
				aux--;
			}
		}
		slow++;
		sdlutils().clearRenderer();
		mngr_->render();
		sdlutils().presentRenderer();
	}

	// Devolvemos la camara a la posicion original
	camera = aux2;
}

void Game::createBackGround(const std::string& spriteId, const int & fils, const int & cols){
	auto* bg = createBasicEntity(Vector2D(300, 300), Vector2D(sdlutils().width(), sdlutils().height()), 0.0f, Vector2D());
	auto* anim_controller = bg->addComponent<AnimBlendGraph>();

	//id //Text //rows // cols //frames //frameRate //loop // startFrame //finalFrame
	//anim_controller->addAnimation("waves", &sdlutils().images().at(spriteId), fils, cols, 121, 24, -1, 0, 119);
	anim_controller->addAnimation("waves", &sdlutils().images().at(spriteId), fils, cols, 1, 1, -1);
}

/// <summary>
/// Crea un entidad b�sica, con el componente Transform
/// </summary>
/// <param name="pos">Posici�n</param>
/// <param name="height">Altura en pixeles</param>
/// <param name="width">Anchura en pixeles</param>
/// <param name="rotation">Rotacion (por defecto es cero)</param>
/// <param name="vel">Velocidad (por defecto es cero)</param>
/// <returns></returns>
Entity* Game::createBasicEntity(const Vector2D & pos, const Vector2D & size, const float & rotation = 0.0f, const Vector2D & vel = Vector2D(0.0f,0.0f))
{
	auto* entity_ = mngr_->addEntity();
	entity_->addComponent<Transform>(pos, vel, size.getX(), size.getY(), rotation);
	return entity_;
}

/// <summary>
/// Crea una caja roja que posee f�sicas y controlador de teclado
/// </summary>
/// <param name="pos">Posici�n</param>
/// <param name="vel">Velocidad (por defecto es cero)</param>
/// <param name="height">Altura en pixeles</param>
/// <param name="width">Anchura en pixeles</param>
/// <param name="rotation">Rotacion (por defecto es cero)</param>
/// <param name="physicType">Determina el tipo f�sico del objeto (STATIC, DYNAMIC, KINEMATIC)</param>
void Game::createBoxTest(const Config& entityConfig)
{
	auto* box = createBasicEntity(entityConfig.pos, entityConfig.size, entityConfig.rotation, entityConfig.vel);

	auto* anim_controller = box->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("run", &sdlutils().images().at("Square"), 1, 1, 1, 1, -1);

	box->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask, entityConfig.isTrigger, entityConfig.friction, entityConfig.fixedRotation, entityConfig.rotation);
	box->addComponent<CameraFollow>(box->getComponent<Transform>());

	if (entityConfig.physicType == 1 || entityConfig.physicType == 2)
		box->addComponent<KeyBoardCtrl>();
}


void Game::createPlayer(const Config& playerConfig)
{
	auto* player = createBasicEntity(playerConfig.pos, playerConfig.size, playerConfig.rotation, playerConfig.vel);

	#pragma region Animations
	//Plantilla de uso de ANIMATION CONTROLLER
	auto* anim_controller = player->addComponent<AnimBlendGraph>();
#pragma region idle, run & jump
	//-idle, run & jump---------------------------------------------------------------------------------------------------
	//Animations
	anim_controller->addAnimation("idle", &sdlutils().images().at("player_idle"), 4, 6, 24, 24, -1);
	anim_controller->addAnimation("run", &sdlutils().images().at("player_run"), 4, 5, 20, 24, -1);
	anim_controller->addAnimation("jump", &sdlutils().images().at("player_jump"), 4, 5, 20, 24, 0);
	//Transitions
	anim_controller->addTransition("idle", "run", "Speed", 1, false);
	anim_controller->addTransition("run", "idle", "Speed", 0, false);
	anim_controller->addTransition("run", "jump", "NotOnFloor", 1, false);	//Anim fuente, anim destino, parametro, valor de parametro, esperar a que termine la animacion
	anim_controller->addTransition("jump", "run", "NotOnFloor", 0, true);
	anim_controller->addTransition("idle", "jump", "NotOnFloor", 1, false);
	anim_controller->addTransition("jump", "idle", "NotOnFloor", 0, true);
	anim_controller->setParamValue("NotOnFloor", 0);	//AVISO: Si no existe el parametro, no hara nada
	anim_controller->setParamValue("Speed", 0);
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion
#pragma region Weapons
	//-WEAPONS------------------------------------------------------------------------------------------------------
#pragma region Chainsaw
	//---CHAINSAW---------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("chainsaw_attack1", &sdlutils().images().at("chainsaw_combo"), 6, 8, 47, 24, 0, 1, 8);
	anim_controller->addAnimation("chainsaw_attack2", &sdlutils().images().at("chainsaw_combo"), 6, 8, 47, 24, 0, 9, 18);
	anim_controller->addAnimation("chainsaw_attack3", &sdlutils().images().at("chainsaw_combo"), 6, 8, 47, 24, -1, 19, 46); // provisional, habria que hacer una de mantener

	anim_controller->addTransition("run", "chainsaw_attack1", "chainsaw_att", 1, false);
	anim_controller->addTransition("chainsaw_attack1", "run", "chainsaw_att", 0, true);
	anim_controller->addTransition("chainsaw_attack1", "chainsaw_attack2", "chainsaw_att", 2, true);
	anim_controller->addTransition("run", "chainsaw_attack2", "chainsaw_att", 2, false);
	anim_controller->addTransition("chainsaw_attack2", "run", "chainsaw_att", 0, true);
	anim_controller->addTransition("chainsaw_attack2", "chainsaw_attack3", "chainsaw_att", 3, true);
	anim_controller->addTransition("run", "chainsaw_attack3", "chainsaw_att", 3, false);
	anim_controller->addTransition("chainsaw_attack3", "run", "chainsaw_att", 0, false);
	anim_controller->addTransition("chainsaw_attack3", "chainsaw_attack1", "chainsaw_att", 4, true);

	anim_controller->setParamValue("chainsaw_att", 0);
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion

#pragma region Sword
	//---SWORD------------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("sword_attack1", &sdlutils().images().at("sword_combo"), 6, 8, 48, 24, 0, 1, 9);
	anim_controller->addAnimation("sword_attack2", &sdlutils().images().at("sword_combo"), 6, 8, 48, 24, 0, 12, 20);
	anim_controller->addAnimation("sword_attack3", &sdlutils().images().at("sword_combo"), 6, 8, 48, 24, -1, 27, 44);

	anim_controller->addTransition("run", "sword_attack1", "sword_att", 1, false);
	anim_controller->addTransition("sword_attack1", "run", "sword_att", 0, true);
	anim_controller->addTransition("sword_attack1", "sword_attack2", "sword_att", 2, true);
	anim_controller->addTransition("run", "sword_attack2", "sword_att", 2, false);
	anim_controller->addTransition("sword_attack2", "run", "sword_att", 0, true);
	anim_controller->addTransition("sword_attack2", "sword_attack3", "sword_att", 3, true);
	anim_controller->addTransition("run", "sword_attack3", "sword_att", 3, false);
	anim_controller->addTransition("sword_attack3", "run", "sword_att", 0, false);
	anim_controller->addTransition("sword_attack3", "sword_attack1", "sword_att", 4, true);

	anim_controller->setParamValue("sword_att", 0);
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion

#pragma region Hammer
//---HAMMER---------------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("hammer_attack1", &sdlutils().images().at("hammer_combo"), 5, 7, 31, 24, 0, 1, 16);
	anim_controller->addAnimation("hammer_attack2", &sdlutils().images().at("hammer_combo"), 5, 7, 31, 24, 0, 17, 30);

	anim_controller->addTransition("run", "hammer_attack1", "hammer_att", 1, false);
	anim_controller->addTransition("hammer_attack1", "run", "hammer_att", 0, true);
	anim_controller->addTransition("hammer_attack1", "hammer_attack2", "hammer_att", 2, true);
	anim_controller->addTransition("run", "hammer_attack2", "hammer_att", 2, false);
	anim_controller->addTransition("hammer_attack2", "run", "hammer_att", 0, true);
	anim_controller->addTransition("hammer_attack2", "hammer_attack1", "hammer_att", 1, false);

	anim_controller->setParamValue("hammer_att", 0);
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion
#pragma endregion

	player->addComponent<BoxCollider>(playerConfig.physicType, PLAYER, PLAYER_MASK, false, playerConfig.friction, playerConfig.fixedRotation, playerConfig.rotation);
	player->addComponent<Player_Health>(&sdlutils().images().at("fullvida"), &sdlutils().images().at("mediavida"), &sdlutils().images().at("vacio"), 300.0f, this);
	player->addComponent<Armas_HUD>(&sdlutils().images().at("sierra"), &sdlutils().images().at("espada"), this);
	player->addComponent<Animation>("1", &sdlutils().images().at("Square"), 1, 1, 1, 1, 0);

	player->addComponent<SoundManager>(75, "FinalBoss");

	player->addComponent<PlayerController>();

	player->addComponent<CameraFollow>(player->getComponent<Transform>(), Vector2D(250.0f, -300.0f), 0.06f); //Vector2D offset y porcentaje de la velocidad de la camara, mas bajo mas lento sigue
	player->addComponent<Hammer>();

	player->addComponent<LoseLife>();

	//Seteamos al Player como MainHandler
	mngr_->setHandler<Player>(player);
}

void Game::createMedusa(Vector2D pos, Vector2D vel, Vector2D size, float rotation)
{
	auto* enemy1 = createBasicEntity(pos, size, rotation, vel);

	auto* anim_controller = enemy1->addComponent<AnimBlendGraph>();

								//id //Text //rows // cols //frames //frameRate //loop // startFrame //finalFrame
	anim_controller->addAnimation("idle", &sdlutils().images().at("Medusa"), 7, 6, 38, 40, -1, 0, 37);
	enemy1->addComponent<BoxCollider>();
	//enemy1->addComponent<EnemyMovement>(Vector2D(1, 0));
}

/// <summary>
/// Crea el tile nivel 0 con f�sicas
/// </summary>
void Game::createLevel0()
{
	auto* nivel = mngr_->addEntity();
	nivel->addComponent<Level0>(MAP_PATH, world_);
	nivel->addComponent<ChainCollider>(nivel->getComponent<Level0>()->getVerticesList());
	nivel->addComponent<MapProcedural>(10);
	nivel->getComponent<MapProcedural>()->debug();
}


void Game::createJointMedusa(Entity* ground)
{
	////Creacion de una medusa fisica que va a estar anclada al techo
	//auto* physBody = mngr_->addEntity(false);
	//physBody->addComponent<Transform>(Vector2D(50.0f, 50.0f), Vector2D(), 50.0f, 50.0f, 0.0f);
	//physBody->addComponent<FramedImage>(&sdlutils().images().at("Medusa"), 7, 6, 200.0f, 4);
	//physBody->addComponent<BoxCollider>(0.0f, 1);


	//b2RevoluteJointDef* b2joint = new b2RevoluteJointDef();
	////Asignar a que cuerpos esta asociado el joint 
	//b2joint->bodyA = physBody->getComponent<BoxCollider>()->getBody();
	//b2joint->bodyB = ground->getComponent<BoxCollider>()->getBody();
	////Si sus colisiones estan o no estan conectadas 
	//b2joint->collideConnected = true;
	////No se del todo como van las anclas 
	//b2joint->localAnchorA.Set(1, 0);
	////Mas o menos en lamitad de su anclaje 
	//b2joint->localAnchorB.Set(2, 0);
	//// Faltan los atributos -> Motor speed(Como de rapido va) , MaxmotorTorque (como de poderoso es) 
	//world_->CreateJoint(b2joint);
}

void Game::createElfShark(const Config& entityConfig) {
#pragma region ElfShark
	//auto* elf1 = createBasicEntity(entityConfig.pos, entityConfig.size, entityConfig.rotation, entityConfig.vel);
	auto* elf1 = mngr_->addEntity();
	Transform* t = elf1->addComponent<Transform>(Vector2D(sdlutils().width() * 1.6f, sdlutils().height() * 0.3f), Vector2D(0, 0), 180.0f, 180.0f, 0.0f);
	//elf1->addComponent<BoxCollider>(KINEMATIC, ENEMY, ENEMY_MASK);
	elf1->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);
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
}