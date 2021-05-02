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
//#include "../components/BoxCollider.h"

class GameState;
class StateMachine;
const Vector2D window(1100, 900);
// when you are not using the methods of a class, just
// say that it exists, that saves time when parsing files
class Manager;


class App {
public:
	static float camera_Zoom_Out;	//Zoom de la camara si asi soy, lo siento Joseda | pero por qu�Eme tomais por un dictador, que no soy �ngel.
	static SDL_Rect camera;
	static const int FPS = 15;


	App();
	virtual ~App();
	void init();
	void start();
	void ShakeCamera(int time);
	void createBackGround(const std::string& spriteId, const int & fils, const int & cols);
	void createJointMedusa(Entity* ground);
	void createMedusa(Vector2D pos, Vector2D vel, Vector2D size, float rotation);
	static void fullScreen();
	void exitGame() { exit = true; }
	void changeToPesca();

	Entity* createBasicEntity(const Vector2D & pos, const Vector2D & size, const float & rotation, const Vector2D & vel);
	StateMachine* getStateMachine() { return stateMachine; }
	inline float getCameraZooOut() const { return camera_Zoom_Out; }

private:
	
	std::shared_ptr<b2World> world_;
	//b2World* world_;
	DamageCollisionCallbacks collisionListener;
	std::unique_ptr<Manager> mngr_;
	//Control de los estados en los que esta el juego 
	StateMachine* stateMachine;

	bool exit = false;

	MapProcedural* map;

	float brightness = 0;

	SoundManager* sndProvisional;
};