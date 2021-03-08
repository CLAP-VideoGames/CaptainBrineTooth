//#include <iostream>;
//#include "box2d.h"
//#include "../captain_brinetooth//ecs/ecs.h"
//#include "../captain_brinetooth/sdlutils/SDLUtils.h"
//#include "../captain_brinetooth/sdlutils/InputHandler.h"
//
//using namespace std;
//
//void InitGame()
//{
//	 SDLUtils::init("Ping Pong", 800, 600,
//		"../assets/config/base.resources.json");
//
//	// a boolean to exit the loop
//	bool exit = false;
//	SDL_Event event;
//
//	while (!exit) {
//		Uint32 startTime = sdlutils().currRealTime();
//
//		ih().clearState();
//		while (SDL_PollEvent(&event))
//			ih().update(event);
//
//		if (ih().isKeyDown(SDL_SCANCODE_ESCAPE)) {
//			exit = true;
//			continue;
//		}
//
//		sdlutils().clearRenderer();
//		sdlutils().presentRenderer();
//
//		Uint32 frameTime = sdlutils().currRealTime() - startTime;
//
//		if (frameTime < 20)
//			SDL_Delay(20 - frameTime);
//	}
//
//	//SDL_Init(SDL_INIT_EVERYTHING);
//	////Creacion de ventana provisional
//	//SDL_Window* window = SDL_CreateWindow("PAC-MAN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
//	//SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//
//	////Codigo Defensivo
//	//if (window == nullptr || renderer == nullptr) throw ("Error loading window o renderer.");
//
//	//Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
//	//Mix_Music* song;  
//	//
//	//song = Mix_LoadMUS("../../../../Assets/music/forest.wav");
//
//	//if (song == nullptr)
//	//	int n = 0;
//
//	//Mix_PlayMusic(song, -1);
//
//	////Bucle para probar que funciona la música
//	//bool run = true;
//	//SDL_Event event;
//	//while (SDL_PollEvent(&event) || run) {
//	//	if (event.type == SDL_KEYDOWN) {
//	//		run = false;
//	//		SDL_QUIT;
//	//	}
//	//}
//
//	//Mix_FreeMusic(song);
//	//song = nullptr; 
//}
//
//int main(int argc, char* argv[])
//{
//	InitGame();
//	return 0;
//}

// This file is part of the course TPV2@UCM - Samir Genaim

#include <iostream>
#include <memory>

#include "game/Game.h"

void start() {
	Game g;

	g.init();
	g.start();
}


int main(int, char**) {
	try {
		start();
	}
	catch (const std::string& e) { // catch exceptions thrown as strings
		std::cerr << e << std::endl;
	}
	catch (const char* e) { // catch exceptions thrown as char*
		std::cerr << e << std::endl;
	}
	catch (const std::exception& e) { // catch exceptions thrown as a sub-type of std::exception
		std::cerr << e.what();
	}
	catch (...) {
		std::cerr << "Caught and exception of unknown type ...";
	}

	return 0;
}