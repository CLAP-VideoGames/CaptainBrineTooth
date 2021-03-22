// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../levels/Level0.h"
#include "tmxlite/Map.hpp"
#include "tmxlite/Layer.hpp"
#include "tmxlite/TileLayer.hpp"

#include "../ecs/ecs.h"
#include "../ecs/Entity.h"
#include "../ecs/Manager.h"
#include "../components/Image.h"
#include "../components/Animation.h"
#include "../components/AnimBlendGraph.h"
#include "../components/Rotate.h"
#include "../components/Bounce.h"
#include "../components/Armas_HUD.h"
#include "../components/Transform.h"
#include "../components/Rectangle.h"
#include "../components/FramedImage.h"
#include "../components/BoxCollider.h"
#include "../components/KeyBoardCtrl.h"
#include "../components/Player_Health.h"
#include "..//components/enemyMovement.h"
#include "..//components/Chainsaw.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"

//tiledmap
const auto MAP_PATH = "assets/maps/level0.tmx";
SDL_Rect Game::camera = {0 ,0,1100,900};

Game::Game() {
	mngr_.reset(new Manager());
	world_ = mngr_->getWorld();
}

Game::~Game() {
}

void Game::init() {
	SDLUtils::init("Captain BrineTooth", 1100, 900, "assets/config/base.resources.json");

	//auto* bg = mngr_->addEntity();
	//bg->addComponent<Transform>(Vector2D(0,0), Vector2D(), sdlutils().width(), sdlutils().height(), 0.0f);
	//bg->addComponent<FramedImage>(&sdlutils().images().at("fondo"), 11, 11, 0.1f, 2);
	//auto* nivel = mngr_->addEntity();
	//nivel->addComponent<Level0>(MAP_PATH, world_);

	//Test ground
	auto* box = mngr_->addEntity();
	box->addComponent<Transform>(Vector2D(sdlutils().width() / 1.7f, sdlutils().height() / 7.0f), Vector2D(), 150.0f, 80.0f, 0.0f);
	box->addComponent<Image>(&sdlutils().images().at("Square"));
	box->addComponent<BoxCollider>(0.0f, true);
	box->addComponent<KeyBoardCtrl>();
	
	auto* ground = mngr_->addEntity();
	ground->addComponent<Transform>(Vector2D(0 , 700), Vector2D(), sdlutils().width() * 10, 10.0f, 0.0f);
	ground->addComponent<Image>(&sdlutils().images().at("Square"));
	ground->addComponent<BoxCollider>(0.0f, false);

		//auto* enemy1 = mngr_->addEntity();
		//enemy1->addComponent<Transform>(
		//Vector2D(sdlutils().width() / 3.0f - 50.0, sdlutils().height() / 2.0f + 60.0f),
		//Vector2D(), 50.0f, 50.0f, 0.0f);
		//enemy1->addComponent<FramedImage>(&sdlutils().images().at("Medusa"), 7, 6, 200.0f, 4);
		//enemy1->addComponent<BoxCollider>(0.0f, true);
		//enemy1->addComponent<EnemyMovement>(Vector2D(1, 0));


	/*auto* player = mngr_->addEntity();   
	player->addComponent<Transform>(Vector2D(sdlutils().width() / 2.0f, sdlutils().height() / 6.0f ), Vector2D(), 200.0f, 200.0f, 0.0f);
	//Plantilla de uso de ANIMATION CONTROLLER
	auto* anim_controller = player->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("run", &sdlutils().images().at("Player_run"), 4, 5, 20, 24, -1);
	anim_controller->addAnimation("jump", &sdlutils().images().at("Player_jump"), 4, 5, 20, 24, 0);
	anim_controller->addTransition("run", "jump", "NotOnFloor", 1, false);	//Anim fuente, anim destino, parametro, valor de parametro, esperar a que termine la animacion
	anim_controller->addTransition("jump", "run", "NotOnFloor", 0, true);
	anim_controller->setParamValue("NotOnFloor", 0);	//AVISO: Si no existe el parametro, no hara nada

	player->addComponent<BoxCollider>(0.0f, true);
	
	player->addComponent<Player_Health>(&sdlutils().images().at("fullvida"), &sdlutils().images().at("mediavida"), &sdlutils().images().at("vacio"), 300.0f, this);
	player->addComponent<Armas_HUD>(&sdlutils().images().at("sierra"), &sdlutils().images().at("espada"));

	player->addComponent<KeyBoardCtrl>();


	player->addComponent<Chainsaw>();

	mngr_->setHandler<Player>(player);*/

	
	//Creacion de una medusa fisica que va a estar anclada al techo
	//auto* physBody = mngr_->addEntity();
	//physBody->addComponent<Transform>(Vector2D(50.0f,50.0f),Vector2D(), 50.0f, 50.0f, 0.0f);
	//physBody->addComponent<FramedImage>(&sdlutils().images().at("Medusa"), 7, 6, 200.0f, 4);
	//physBody->addComponent<BoxCollider>(0.0f, true);


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


	mLastUpdateTime = sdlutils().currRealTime();
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

		/*//Update rate refresh
		Uint32 frameTime = sdlutils().currRealTime() - mLastUpdateTime;
		
		if (frameTime >= MILLISECS_PER_TICK) {
			mngr_->update();
			mngr_->refresh();

			UpdateCamera();

			mLastUpdateTime = sdlutils().currRealTime();
		}*/
		mngr_->update();
		mngr_->refresh();

		sdlutils().clearRenderer();
		mngr_->render();
		sdlutils().presentRenderer();
		
		Uint32 frameTime = sdlutils().currRealTime() - startTime;


		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}

}
void Game::UpdateCamera() 
{
	//Seguimiento del jugador en base a la camara y ajuste de los limites
	camera.x = mngr_->getHandler<Player>()->getComponent<Transform>()->getPos().getX()-  camera.w/2.0f;
	camera.y = mngr_->getHandler<Player>()->getComponent<Transform>()->getPos().getY() - camera.h/ 2.0f;


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
	for (int i = 0; i < time; i++)
	{
		if (slow % 2 == 0)
		{
			if (aux == 0)
			{
				camera.x -= 15; 
				camera.y -= 15;
				aux++;
			}
			else
			{
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