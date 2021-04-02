// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../levels/Level0.h"
#include "tmxlite/Map.hpp"
#include "tmxlite/Layer.hpp"
#include "tmxlite/TileLayer.hpp"
#include "../GameStateMachine.h"
#include "../GameState.h"

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
#include "../PruebaState.h"
#include "..//components/CameraFollow.h"
#include "../CollisionLayers.h"
#include "../components/LoseLife.h"
#include "../MenuState.h"
#include "..//components/Sword.h"
#include "..//components/Hammer.h"

//tiledmap
const Vector2D window(1100, 900);

const auto MAP_PATH = "assets/maps/levelTest/levelTest.tmx";
SDL_Rect Game::camera = {0 ,0,window.getX(),window.getY()};

using namespace ColLayers;

Game::Game() {
	stateMachine = new GameStateMachine();
	//Creariamos el menu y hariamos un setManager dandole el valor a 
	//Hariamos un push del menu


	b2Vec2 gravity(0.0f, 9.8f);
	world_ = new b2World(gravity);


	PruebaState* prueba = new PruebaState(this, world_);
	stateMachine->pushState(prueba);
	//Ahora  cuando creemos un nuevo estado hay que hacer un reset del manager del juego poniendo el manager del estado en cuestion
	Manager* a = stateMachine->currentState()->getMan();
	mngr_.reset(a);
}

Game::~Game() {
}

void Game::init() {
	SDLUtils::init("Captain BrineTooth", window.getX(), window.getY (), "assets/config/base.resources.json");

	//auto* soundController = mngr_->addEntity(false);
	//soundController->addComponent<SoundManager>(75, "drunken", "tale");
	/*soundController->getComponent<SoundManager>()->playMainMusic();
	soundController->getComponent<SoundManager>()->playPauseMusic();
	soundController->getComponent<SoundManager>()->modifyVolume(-70);*/

	world_->SetContactListener(&collisionListener);

	createBackGround("Square", 11, 11);
	createLevel0();

	PruebaState* prueba = static_cast<PruebaState*>(stateMachine->currentState());
	prueba->addStateEntityPrueba();

	//Caja para hacer testeo con movimiento
	//createBoxTest(Vector2D(sdlutils().width() / 5.5f, sdlutils().height() / 7.0f), Vector2D(), Vector2D(150.0f, 80.0f), 0.5f, DYNAMIC, false, DEFAULT, DEFAULT_MASK, false, 0.0f);
	//Crea el suelo
	//createBoxTest2(Vector2D(sdlutils().width() / 2.0f, 700), Vector2D(), Vector2D(sdlutils().width()/1.2f, 80.0f), 2.0f, STATIC, false, DEFAULT, DEFAULT_MASK, false, 0.0f);

	//createMedusa(Vector2D(sdlutils().width() / 3.0f - 50.0, sdlutils().height() / 2.0f + 60.0f), Vector2D(), Vector2D(200.0f, 200.0f), 0.0f);

	//sdlutils().musics().at(mainMusic).setMusicVolume(volumen++);

	//createChain();

	createPlayer(Vector2D(sdlutils().width() / 2.5f, sdlutils().height() / 8.0f), Vector2D(0, 0), Vector2D(200.0f, 200.0f), 0.2f, true, 0.0f);

}

void Game::start() {
	// a boolean to exit the loop
	bool exit = false;
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
		  


		//Los estados son capaces de actualizar todo lo suyo gracias a la referencia del manager
		//mngr_->update();
		stateMachine->currentState()->update();
		//No tengo muy claro que la camara se actualize en base al juego , en base a cada estado individual
		//O en base a cualquier estado
		stateMachine->currentState()->refresh();

		sdlutils().clearRenderer();
		stateMachine->currentState()->render();
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

	//bg->addComponent<BoxCollider>(STATIC, 1, false, 1.0f, false, 0.0f);
	auto* anim_controller = bg->addComponent<AnimBlendGraph>();

	//bg->addComponent<FixedCameraPosition>();

	//id //Text //rows // cols //frames //frameRate //loop // startFrame //finalFrame
	//anim_controller->addAnimation("waves", &sdlutils().images().at(spriteId), fils, cols, 121, 24, -1, 0, 119);
	anim_controller->addAnimation("waves", &sdlutils().images().at(spriteId), fils, cols, 1, 1, -1);
}

void Game::createChain(){

	//auto* chain = mngr_->addEntity(false);
	//chain->addComponent<ChainCollider>();
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
void Game::createBoxTest(const Vector2D & pos, const  Vector2D & vel, const Vector2D & size, const float & friction, const TYPE physicType, const bool & isTrigger, const uint16& col, const uint16& colMask, const bool & fixedRotation, const float & rotation)
{
	auto* box = createBasicEntity(pos, size, rotation, vel);

	auto* anim_controller = box->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("run", &sdlutils().images().at("Square"), 1, 1, 1, 1, -1);

	box->addComponent<BoxCollider>(physicType, col, colMask, isTrigger, friction, fixedRotation, rotation);
	box->addComponent<CameraFollow>(box->getComponent<Transform>());

	if(physicType == 1 || physicType == 2)
		box->addComponent<KeyBoardCtrl>();
}

void Game::createBoxTest2(const Vector2D& pos, const  Vector2D& vel, const Vector2D& size, const float& friction, const TYPE physicType, const bool& isTrigger, const uint16& col, const uint16& colMask, const bool& fixedRotation, const float& rotation)
{
	auto* box = createBasicEntity(pos, size, rotation, vel);

	auto* anim_controller = box->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("run", &sdlutils().images().at("Square"), 1, 1, 1, 1, -1);

	box->addComponent<BoxCollider>(physicType, col, colMask, isTrigger, friction, fixedRotation, rotation);
	//box->addComponent<CameraFollow>(box->getComponent<Transform>());

	//if (physicType == 1 || physicType == 2)
	//	box->addComponent<KeyBoardCtrl>();
}

void Game::createPlayer(const Vector2D & pos, const Vector2D & vel, const Vector2D & size, const float & friction, const bool & fixedRotation, const float& rotation)
{
	auto* player = createBasicEntity(pos, size, rotation, vel);
#pragma region Animations
	//Plantilla de uso de ANIMATION CONTROLLER
	auto* anim_controller = player->addComponent<AnimBlendGraph>();
#pragma region run & jump
	//-run & jump---------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("run", &sdlutils().images().at("player_run"), 4, 5, 20, 24, -1);
	anim_controller->addAnimation("jump", &sdlutils().images().at("player_jump"), 4, 5, 20, 24, 0);
	anim_controller->addTransition("run", "jump", "NotOnFloor", 1, false);	//Anim fuente, anim destino, parametro, valor de parametro, esperar a que termine la animacion
	anim_controller->addTransition("jump", "run", "NotOnFloor", 0, true);
	anim_controller->setParamValue("NotOnFloor", 0);	//AVISO: Si no existe el parametro, no hara nada
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion
#pragma region Weapons
	//-WEAPONS------------------------------------------------------------------------------------------------------
#pragma region Chainsaw
	//---CHAINSAW---------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("chainsaw_attack1", &sdlutils().images().at("chainsaw_combo"), 6, 8, 47, 24, 0, 1, 8);
	anim_controller->addAnimation("chainsaw_attack2", &sdlutils().images().at("chainsaw_combo"), 6, 8, 47, 24, 0, 9, 18);
	anim_controller->addAnimation("chainsaw_attack3", &sdlutils().images().at("chainsaw_combo"), 6, 8, 47, 24, -1, 19, 46);

	anim_controller->addTransition("run", "chainsaw_attack1", "chainsaw_att", 1, false);
	anim_controller->addTransition("chainsaw_attack1", "run", "chainsaw_att", 0, true);
	anim_controller->addTransition("chainsaw_attack1", "chainsaw_attack2", "chainsaw_att", 2, true);
	anim_controller->addTransition("chainsaw_attack2", "run", "chainsaw_att", 0, true);
	anim_controller->addTransition("chainsaw_attack2", "chainsaw_attack3", "chainsaw_att", 3, true);
	anim_controller->addTransition("chainsaw_attack3", "run", "chainsaw_att", 0, false);

	anim_controller->setParamValue("chainsaw_att", 0);
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion

#pragma region Sword
	//---CHAINSAW---------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("sword_attack1", &sdlutils().images().at("sword_combo"), 6, 8, 48, 24, 0, 1, 11);
	anim_controller->addAnimation("sword_attack2", &sdlutils().images().at("sword_combo"), 6, 8, 48, 24, 0, 12, 22);
	anim_controller->addAnimation("sword_attack3", &sdlutils().images().at("sword_combo"), 6, 8, 48, 24, -1, 23, 47);

	anim_controller->addTransition("run", "sword_attack1", "sword_att", 1, false);
	anim_controller->addTransition("sword_attack1", "run", "sword_att", 0, true);
	anim_controller->addTransition("sword_attack1", "sword_attack2", "sword_att", 2, true);
	anim_controller->addTransition("sword_attack2", "run", "sword_att", 0, true);
	anim_controller->addTransition("sword_attack2", "sword_attack3", "sword_att", 3, true);
	anim_controller->addTransition("sword_attack3", "run", "sword_att", 0, false);

	anim_controller->setParamValue("sword_att", 0);
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion

#pragma region Hammer
//---CHAINSAW---------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("hammer_attack1", &sdlutils().images().at("hammer_combo"), 5, 7, 31, 24, 0, 1, 16);
	anim_controller->addAnimation("hammer_attack2", &sdlutils().images().at("hammer_combo"), 5, 7, 31, 24, 0, 17, 30);

	anim_controller->addTransition("run", "hammer_attack1", "hammer_att", 1, false);
	anim_controller->addTransition("hammer_attack1", "run", "hammer_att", 0, true);
	anim_controller->addTransition("hammer_attack1", "hammer_attack2", "hammer_att", 2, true);
	anim_controller->addTransition("hammer_attack2", "run", "hammer_att", 0, true);

	anim_controller->setParamValue("hammer_att", 0);
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion
#pragma endregion

	player->addComponent<BoxCollider>(DYNAMIC, PLAYER, PLAYER_MASK, false, friction, fixedRotation, rotation);
	player->addComponent<Player_Health>(&sdlutils().images().at("fullvida"), &sdlutils().images().at("mediavida"), &sdlutils().images().at("vacio"), 300.0f, this);
	player->addComponent<Armas_HUD>(&sdlutils().images().at("sierra"), &sdlutils().images().at("espada"));
	
	player->addComponent<SoundManager>(75, "FinalBoss");
	//player->getComponent<SoundManager>()->playMainMusic();
	
	player->addComponent<PlayerController>();

	player->addComponent<CameraFollow>(player->getComponent<Transform>(), Vector2D(0.0f, -300), 0.035f);
	player->addComponent<Sword>();
	
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


	//int alt1 = 2.3f;
	//int alt2 = 2.0f;

	//b2Vec2* vs = new b2Vec2[4];

	//vs[0] = b2Vec2((sdlutils().width() / 15.0f), (sdlutils().height() / alt1));
	//vs[1] = b2Vec2((sdlutils().width() / 5.0f), (sdlutils().height() / alt2));
	//vs[2] = b2Vec2((sdlutils().width() / 0.5f), (sdlutils().height() / alt2));
	//vs[3] = b2Vec2((sdlutils().width() / 0.1f), (sdlutils().height() / alt1));
	nivel->addComponent<ChainCollider>(nivel->getComponent<Level0>()->getVerticesList());
	//nivel->addComponent<ChainCollider>(vs);

	//nivel->getComponent<Level0>();
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
