// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <memory>
#include "box2d.h"
#include "SDL.h"

#include "../ecs/ecs.h"
#include "../ecs/Manager.h"
#include "../utils/Vector2D.h"
#include "../ecs/Entity.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "..//components/DamageCollisionCallbacks.h"
#include "../components/BoxCollider.h"

class GameState;
class StateMachine;
const Vector2D window(1100, 900);
// when you are not using the methods of a class, just
// say that it exists, that saves time when parsing files
class Manager;


class App {
public:
	const float camera_Zoom_Out = 2.0f;	//Zoom de la camara si asi soy, lo siento Joseda | pero por quÅEme tomais por un dictador, que no soy ¡ngel.

	App();
	virtual ~App();
	void init();
	void start();
	void ShakeCamera(int time);

	static SDL_Rect camera;

	//void createLevel0();
	void createBackGround(const std::string& spriteId, const int & fils, const int & cols);

	Entity* createBasicEntity(const Vector2D & pos, const Vector2D & size, const float & rotation, const Vector2D & vel);

	void createJointMedusa(Entity* ground);
	void createMedusa(Vector2D pos, Vector2D vel, Vector2D size, float rotation);

	StateMachine* getStateMachine() { return stateMachine; }
	void exitGame() { exit = true; }
	
	//update (Juan: Si esta en Mayusculas pq es una constante) Joseda: asi es gente, lo que sea en mayus tiene que ser constante
	//const Uint32 TICKS = 60;
	//const Uint32 MILLISECS_PER_TICK = 1000 / TICKS;
private:

	std::shared_ptr<b2World> world_;
	//b2World* world_;
	DamageCollisionCallbacks collisionListener;
	std::unique_ptr<Manager> mngr_;
	//Control de los estados en los que esta el juego 
	StateMachine* stateMachine;

	bool exit = false;

	MapProcedural* map;
};

