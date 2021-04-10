// This file is part of the course TPV2@UCM - Samir Genaim

#include "App.h"

#include "../levels/Level0.h"

#include "../states/StateMachine.h"
#include "../states/GameState.h"
#include "../states/TestZoneState.h"
#include "../states/MenuState.h"
//tiledmap

const auto MAP_PATH = "assets/maps/levelTest/levelTest.tmx";
SDL_Rect App::camera = {0 ,0,window.getX(),window.getY()};

using namespace ColLayers;

//Comentario para probar un commit
App::App() {
	stateMachine = new StateMachine();
	//Creariamos el menu y hariamos un setManager dandole el valor a 
	//Hariamos un push del menu

	b2Vec2 gravity(0.0f, 9.8f);
	world_ = new b2World(gravity);
	SoundManager* sndProvisional = new SoundManager(0, "Menu");

	stateMachine->pushState(new MenuState(this, world_, sndProvisional));
}

App::~App() {
}

void App::init() {
	SDLUtils::init("Captain BrineTooth", window.getX(), window.getY(), "assets/config/base.resources.json");


	stateMachine->currentState()->init();
	world_->SetContactListener(&collisionListener);

	//auto* soundController = mngr_->addEntity();
	//soundController->addComponent<SoundManager>(75, "Menu");

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

void App::start() {
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
  
		stateMachine->currentState()->update();
		stateMachine->currentState()->refresh();

		sdlutils().clearRenderer();
		SDL_RenderSetLogicalSize(sdlutils().renderer(), window.getX() * camera_Zoom_Out, window.getY()* camera_Zoom_Out);
		stateMachine->currentState()->render();
		sdlutils().presentRenderer();
		
		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}
}
//Metodos propios de game 

void App::ShakeCamera(int time){
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

void App::createBackGround(const std::string& spriteId, const int & fils, const int & cols){
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
Entity* App::createBasicEntity(const Vector2D & pos, const Vector2D & size, const float & rotation = 0.0f, const Vector2D & vel = Vector2D(0.0f,0.0f))
{
	auto* entity_ = mngr_->addEntity();
	entity_->addComponent<Transform>(pos, vel, size.getX(), size.getY(), rotation);
	return entity_;
}

void App::createMedusa(Vector2D pos, Vector2D vel, Vector2D size, float rotation)
{
	auto* enemy1 = createBasicEntity(pos, size, rotation, vel);

	auto* anim_controller = enemy1->addComponent<AnimBlendGraph>();

								//id //Text //rows // cols //frames //frameRate //loop // startFrame //finalFrame
	anim_controller->addAnimation("idle", &sdlutils().images().at("Medusa"), 7, 6, 38, 40, -1, 0, 37);
	enemy1->addComponent<BoxCollider>();
	//enemy1->addComponent<EnemyMovement>(Vector2D(1, 0));
}

void App::createJointMedusa(Entity* ground)
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
