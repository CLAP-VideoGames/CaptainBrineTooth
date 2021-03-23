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
	//Entity* createBasicEntity(Vector2D pos, float height, float width, float rotation, Vector2D vel);

	void createLevel0();
	static SDL_Rect camera;
	void createBackGround(const std::string& spriteId, int fils, int cols, float tanim, int empty);
	Entity* createBasicEntity(Vector2D pos, float height, float width, float rotation, Vector2D vel);
	void createBoxTest(Vector2D pos, Vector2D vel, float height, float width, float rotation, int physicType);
	void createJointMedusa(Entity* ground);
	void createMedusa(Vector2D pos, Vector2D vel, float height, float width, float rotation);
	void createPlayer(Vector2D pos, Vector2D vel, float height, float width, float rotation);

	//update (Juan: Si esta en Mayusculas pq es una constante) Joseda: asi es gente, lo que sea en mayus tiene que ser constante
	//const Uint32 TICKS = 60;
	//const Uint32 MILLISECS_PER_TICK = 1000 / TICKS;
private:
	b2World* world_;
	DamageCollisionCallbacks collisionListener;
	std::unique_ptr<Manager> mngr_;
};

