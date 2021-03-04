#include <iostream>;
#include "SDL.h"
#include "box2d.h"
#include "SDL_mixer.h"

using namespace std;

int InitGame()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	//Creacion de ventana provisional
	SDL_Window* window = SDL_CreateWindow("PAC-MAN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//Codigo Defensivo
	if (window == nullptr || renderer == nullptr) throw ("Error loading window o renderer.");

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	Mix_Music* song;  
	
	song = Mix_LoadMUS("../../../../Assets/music/forest.wav");

	if (song == nullptr)
		int n = 0;

	Mix_PlayMusic(song, -1);

	//Bucle para probar que funciona la música
	bool run = true;
	SDL_Event event;
	while (SDL_PollEvent(&event) || run) {
		if (event.type == SDL_KEYDOWN) {
			run = false;
			SDL_QUIT;
		}
	}

	Mix_FreeMusic(song);
	song = nullptr; 
}

int main(int argc, char* argv[])
{
	InitGame();
	return 0;
}