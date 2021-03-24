// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../levels/Level0.h"
#include "tmxlite/Map.hpp"
#include "tmxlite/Layer.hpp"
#include "tmxlite/TileLayer.hpp"
#include "../GameStateMachine.h"
#include "../GameState.h"


#include "../components/Image.h"
#include "../components/Animation.h"
#include "../components/AnimBlendGraph.h"
#include "../components/Rotate.h"
#include "../components/Bounce.h"
#include "../components/Armas_HUD.h"
#include "../components/Transform.h"
#include "../components/Rectangle.h"
#include "../components/FramedImage.h"
#include "../components/KeyBoardCtrl.h"
#include "../components/Player_Health.h"
#include "..//components/enemyMovement.h"
#include "..//components/PlayerController.h"
#include "..//components/Chainsaw.h"


//tiledmap
const auto MAP_PATH = "assets/maps/level0.tmx";
SDL_Rect Game::camera = {0 ,0,1100,900};

Game::Game() {
	Manager* a = new Manager();
	stateMachine = new GameStateMachine();
	//Creariamos el menu y hariamos un setManager dandole el valor a 
	//Hariamos un push del menu
	mngr_.reset(a);
	world_ = mngr_->getWorld();
}

Game::~Game() {
}

void Game::init() {
	SDLUtils::init("Captain BrineTooth", 1100, 900, "assets/config/base.resources.json");

	world_->SetContactListener(&collisionListener);

	createBackGround("fondo", 11, 11, 0.1f, 2);
	//createLevel0();

	//Caja para hacer testeo con movimiento
	//createBoxTest(Vector2D(sdlutils().width() / 1.7f, sdlutils().height() / 7.0f), Vector2D(), Vector2D(150.0f, 80.0f), 0.5f, DYNAMIC, false, 1, false, 0.0f);

	//Crea el suelo
	createBoxTest(Vector2D(sdlutils().width() / 2.0f, 700), Vector2D(), Vector2D(sdlutils().width()/1.2f, 80.0f), 2.0f, STATIC, false, 1, false, 0.0f);

	//createMedusa(Vector2D(sdlutils().width() / 3.0f - 50.0, sdlutils().height() / 2.0f + 60.0f), Vector2D(), 50.0f, 50.0f, 0.0f);

	//Creamos al player
	createPlayer(Vector2D(sdlutils().width() / 2.0f, sdlutils().height() / 6.0f), Vector2D(0, 0), Vector2D(200.0f, 200.0f), 0.2f, false, 0.0f);
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
		UpdateCamera();
		mngr_->refresh();

		sdlutils().clearRenderer();
		mngr_->render();
		sdlutils().presentRenderer();
		
		Uint32 frameTime = sdlutils().currRealTime() - startTime;


		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}

}
//Metodos propios de game 
void Game::UpdateCamera() 
{
	//Seguimiento del jugador en base a la camara y ajuste de los limites
	if (mngr_->getHandler<Player>() != nullptr)
	{
		camera.x = mngr_->getHandler<Player>()->getComponent<Transform>()->getPos().getX()-  camera.w/2.0f;
		camera.y = mngr_->getHandler<Player>()->getComponent<Transform>()->getPos().getY() - camera.h/ 2.0f;
	}


	//if (camera.x < 0) camera.x = 0;
	//if (camera.y < 0) camera.y = 0;
	//if (camera.x > camera.w) camera.x = camera.w;
	//if (camera.y > camera.h) camera.y = camera.h;

}

void Game::ShakeCamera(int time)
{
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





//Metodos que todo GameState deberia de tener 
void Game::createBackGround(const std::string& spriteId, const int & fils, const int & cols, const float & tanim, const int & empty)
{
	auto* bg = createBasicEntity(Vector2D(0, 0), Vector2D(sdlutils().width(), sdlutils().height()), 0.0f, Vector2D());
	bg->addComponent<FramedImage>(&sdlutils().images().at(spriteId), fils, cols, tanim, empty);
}

/// <summary>
/// Crea un entidad básica, con el componente Transform
/// </summary>
/// <param name="pos">Posición</param>
/// <param name="height">Altura en pixeles</param>
/// <param name="width">Anchura en pixeles</param>
/// <param name="rotation">Rotacion (por defecto es cero)</param>
/// <param name="vel">Velocidad (por defecto es cero)</param>
/// <returns></returns>
Entity* Game::createBasicEntity(const Vector2D & pos, const Vector2D & size, const float & rotation = 0.0f, const Vector2D & vel = Vector2D(0.0f,0.0f))
{
	auto* entity_ = mngr_->addEntity(false);
	entity_->addComponent<Transform>(pos, vel, size.getX(), size.getY(), rotation);

	return entity_;
}

/// <summary>
/// Crea una caja roja que posee físicas y controlador de teclado
/// </summary>
/// <param name="pos">Posición</param>
/// <param name="vel">Velocidad (por defecto es cero)</param>
/// <param name="height">Altura en pixeles</param>
/// <param name="width">Anchura en pixeles</param>
/// <param name="rotation">Rotacion (por defecto es cero)</param>
/// <param name="physicType">Determina el tipo físico del objeto (STATIC, DYNAMIC, KINEMATIC)</param>
void Game::createBoxTest(const Vector2D & pos, const  Vector2D & vel, const Vector2D & size, const float & friction, const TYPE physicType, const bool & isTrigger, const int & col, const bool & fixedRotation, const float & rotation)
{
	auto* box = createBasicEntity(pos, size, rotation, vel);
	box->addComponent<Image>(&sdlutils().images().at("Square"));
	box->addComponent<BoxCollider>(physicType, col, isTrigger, friction, fixedRotation, rotation);

	if(physicType == 1 || physicType == 2)
		box->addComponent<PlayerController>();
}

void Game::createPlayer(const Vector2D & pos, const Vector2D & vel, const Vector2D & size, const float & friction, const bool & fixedRotation, const float& rotation)
{
	auto* player = createBasicEntity(pos, size, rotation, vel);
#pragma region Animations
	//Plantilla de uso de ANIMATION CONTROLLER
	auto* anim_controller = player->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("run", &sdlutils().images().at("Player_run"), 4, 5, 20, 24, -1);
	anim_controller->addAnimation("jump", &sdlutils().images().at("Player_jump"), 4, 5, 20, 24, 0);
	anim_controller->addTransition("run", "jump", "NotOnFloor", 1, false);	//Anim fuente, anim destino, parametro, valor de parametro, esperar a que termine la animacion
	anim_controller->addTransition("jump", "run", "NotOnFloor", 0, true);
	anim_controller->setParamValue("NotOnFloor", 0);	//AVISO: Si no existe el parametro, no hara nada
#pragma endregion

	player->addComponent<BoxCollider>(DYNAMIC , 1, false, friction, fixedRotation, rotation);
	player->addComponent<Player_Health>(&sdlutils().images().at("fullvida"), &sdlutils().images().at("mediavida"), &sdlutils().images().at("vacio"), 300.0f, this);
	player->addComponent<Armas_HUD>(&sdlutils().images().at("sierra"), &sdlutils().images().at("espada"));
	player->addComponent<PlayerController>();

	player->addComponent<Chainsaw>();

	//Seteamos al Player como MainHandler
	mngr_->setHandler<Player>(player);
}

void Game::createMedusa(Vector2D pos, Vector2D vel, Vector2D size, float rotation)
{
	auto* enemy1 = createBasicEntity(pos, size, rotation, vel);
	enemy1->addComponent<FramedImage>(&sdlutils().images().at("Medusa"), 7, 6, 200.0f, 4);
	enemy1->addComponent<BoxCollider>();
	enemy1->addComponent<EnemyMovement>(Vector2D(1, 0));
}

/// <summary>
/// Crea el tile nivel 0 con físicas
/// </summary>
void Game::createLevel0()
{
	auto* nivel = mngr_->addEntity(false);
	nivel->addComponent<Level0>(MAP_PATH, world_);
}

void Game::createJointMedusa(Entity* ground)
{
	//Creacion de una medusa fisica que va a estar anclada al techo
	auto* physBody = mngr_->addEntity(false);
	physBody->addComponent<Transform>(Vector2D(50.0f, 50.0f), Vector2D(), 50.0f, 50.0f, 0.0f);
	physBody->addComponent<FramedImage>(&sdlutils().images().at("Medusa"), 7, 6, 200.0f, 4);
	physBody->addComponent<BoxCollider>(0.0f, 1);


	b2RevoluteJointDef* b2joint = new b2RevoluteJointDef();
	//Asignar a que cuerpos esta asociado el joint 
	b2joint->bodyA = physBody->getComponent<BoxCollider>()->getBody();
	b2joint->bodyB = ground->getComponent<BoxCollider>()->getBody();
	//Si sus colisiones estan o no estan conectadas 
	b2joint->collideConnected = true;
	//No se del todo como van las anclas 
	b2joint->localAnchorA.Set(1, 0);
	//Mas o menos en lamitad de su anclaje 
	b2joint->localAnchorB.Set(2, 0);
	// Faltan los atributos -> Motor speed(Como de rapido va) , MaxmotorTorque (como de poderoso es) 
	world_->CreateJoint(b2joint);
}
