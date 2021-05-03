// This file is part of the course TPV2@UCM - Samir Genaim

#include "App.h"

#include "../levels/Level0.h"
#include "../states/StateMachine.h"
#include "../states/GameState.h"
#include "../states/TestZoneState.h"
#include "../states/PlayState.h"
#include "../states/PasueState.h"
#include "../states/MenuState.h"
#include "../states/IntroState.h"
#include "../states/PescaState.h"
#include "../states/OptionsState.h"
//tiledmap

const auto MAP_PATH = "assets/maps/levelTest/levelTest.tmx";
float App::camera_Zoom_Out = 2.0f;
SDL_Rect App::camera = { 0 ,0,(int)(window.getX() * camera_Zoom_Out),(int)(window.getY() * camera_Zoom_Out) };

using namespace ColLayers;

//Comentario para probar un commit
App::App() {
	stateMachine = new StateMachine();

	b2Vec2 gravity(0.0f, 9.8f);
	world_ = make_shared<b2World>(gravity);

	sndProvisional = new SoundManager(0, "Menu");
	////----Inicio de Intro----
	//stateMachine->pushState(new IntroState(this, world_, sndProvisional));
	//----Inicio por defecto---
	//stateMachine->pushState(new MenuState(this, world_, sndProvisional));
	//----Inicio de Options----
	stateMachine->pushState(new OptionsState(this, world_, sndProvisional));
	//----Inicio de Juego------
	//stateMachine->pushState(new PlayState(this, world_, sndProvisional));
	//-----Zona de pruebas-----
	//stateMachine->pushState(new TestZoneState(this, world_, sndProvisional));
	//stateMachine->pushState(new PescaState(this, world_, sndProvisional));

}

App::~App() {
	//Setteamos de nuevo el brillo por defecto
	SDL_SetWindowBrightness(sdlutils().window(), brightness);
	delete stateMachine;
	delete sndProvisional;
}

void App::init() {
	SDLUtils::init("Captain BrineTooth", window.getX(), window.getY(), "assets/config/base.resources.json");
	brightness = SDL_GetWindowBrightness(sdlutils().window());

	//Refrescamos los estados pusheados
	stateMachine->refreshStates();

	// Iniciamos el juego con X volumen
	stateMachine->currentState()->getMngr()->getSoundMngr()->setGeneralVolume(10);

	world_->SetContactListener(&collisionListener);
	//Habilitamos el hecho de poder realizar colores de SDL con Alpha. Solo los Colores de SDL
	//Las texturas ya vienen por defecto configuradas para aceptar alpha
	SDL_SetRenderDrawBlendMode(sdlutils().renderer(), SDL_BLENDMODE_BLEND);
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

		if (ih().isKeyDown(SDL_SCANCODE_ESCAPE)|| event.type == SDL_QUIT) {
			exit = true;
			continue;
		}

		if (ih().isKeyDown(SDL_SCANCODE_A)) {
			sdlutils().toggleFullScreen();
		}

		//Procesamos la entrada del usuario y cualquier cambio de entidad
		stateMachine->currentState()->update();
		//Refrescamos la máquina de estados
		stateMachine->refreshStates();
		//Refrescamos las entidades
		stateMachine->currentState()->refresh();

		sdlutils().clearRenderer();
		SDL_RenderSetLogicalSize(sdlutils().renderer(), camera.w, camera.h);
		//Renderizamos las entidades
		stateMachine->currentState()->render();

		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < FPS)
			SDL_Delay(FPS - frameTime);
	}

	int n = 0;

}
//Metodos propios de game 

void App::ShakeCamera(int time) {
	int aux = 0;
	SDL_Rect aux2 = camera;
	int slow = 0;
	// Movemos la camara de forma random y rapidamente
	for (int i = 0; i < time; i++) {
		if (slow % 2 == 0) {
			if (aux == 0) {
				camera.x -= 15;
				camera.y -= 15;
				aux++;
			}
			else {
				camera.x += 15;
				camera.y += 15;
				aux--;
			}
		}
		slow++;
		sdlutils().clearRenderer();
		stateMachine->currentState()->getMngr()->render();
		sdlutils().presentRenderer();
	}

	// Devolvemos la camara a la posicion original
	camera = aux2;
}

void App::fullScreen()
{
	//sdlutils().toggleFullScreen(); // Esto de aqui hace que explote el ordenador

	// Actualizamos la camara
	camera = { 0 ,0,(int)(window.getX()),(int)(window.getY()) };


	camera_Zoom_Out = 1.0f;
}

void App::createBackGround(const std::string& spriteId, const int& fils, const int& cols) {
	auto* bg = createBasicEntity(Vector2D(300, 300), Vector2D(sdlutils().width(), sdlutils().height()), 0.0f, Vector2D());
	auto* anim_controller = bg->addComponent<AnimBlendGraph>();

	//id //Text //rows // cols //frames //frameRate //loop // startFrame //finalFrame
	//anim_controller->addAnimation("waves", &sdlutils().images().at(spriteId), fils, cols, 121, 24, -1, 0, 119);
	anim_controller->addAnimation("waves", &sdlutils().images().at(spriteId), fils, cols, 1, 1, -1);
}

void App::changeToPesca()
{
	b2Vec2 gravity(0.0f, 9.8f);
	std::shared_ptr<b2World>w = std::make_shared<b2World>(gravity);
	stateMachine->pushState(new PescaState(this, w, sndProvisional));
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
Entity* App::createBasicEntity(const Vector2D& pos, const Vector2D& size, const float& rotation = 0.0f, const Vector2D& vel = Vector2D(0.0f, 0.0f))
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
