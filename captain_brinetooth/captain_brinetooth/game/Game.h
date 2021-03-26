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
class GameStateMachine;




// when you are not using the methods of a class, just
// say that it exists, that saves time when parsing files
class Manager;

class Game {
public:
	Game();
	virtual ~Game();
	void init();
	void start();
	void UpdateCamera();
	void ShakeCamera(int time);

	static SDL_Rect camera;

	void createLevel0();
	void createBackGround(const std::string& spriteId, const int & fils, const int & cols, const float & tanim, const int & empty);

	Entity* createBasicEntity(const Vector2D & pos, const Vector2D & size, const float & rotation, const Vector2D & vel);
	void createPlayer(const Vector2D & pos, const Vector2D & vel, const Vector2D & size, const float & friction, const bool & fixedRotation, const float& rotation);
	void createBoxTest(const Vector2D & pos, const  Vector2D & vel, const Vector2D & size, const float & friction, const TYPE physicType, const bool & isTrigger, const int & col, const bool & fixedRotation, const float & rotation);

	void createJointMedusa(Entity* ground);
	void createMedusa(Vector2D pos, Vector2D vel, Vector2D size, float rotation);
	
	//update (Juan: Si esta en Mayusculas pq es una constante) Joseda: asi es gente, lo que sea en mayus tiene que ser constante
	//const Uint32 TICKS = 60;
	//const Uint32 MILLISECS_PER_TICK = 1000 / TICKS;
private:
	b2World* world_;
	DamageCollisionCallbacks collisionListener;
	std::unique_ptr<Manager> mngr_;
	//Control de los estados en los que esta el juego 
	GameStateMachine* stateMachine;

};

