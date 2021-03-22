// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <memory>
#include "box2d.h"
#include "SDL.h"


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

	//update (Si esta en Mayusculas pq es una constante)
	const Uint32 FRAME_RATE = 60;
	const Uint32 MILLISECS_PER_FRAME = 1000 / FRAME_RATE;
private:
	b2World* world_;
	std::unique_ptr<Manager> mngr_;
	Uint32 mLastUpdateTime;
};

