#include <iostream>;
#include "SDL.h"
#include "box2d.h"
#include <SDL_mixer.h>

using namespace std;

void CrearJuego()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_Music* song;
	song = Mix_LoadMUS("../../../../Assets/music/song1.mp3");

	//Creacion de ventana provisional
	SDL_Window* window = SDL_CreateWindow("Captain Brinetooth", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);

	Mix_PlayMusic(song, 0);

	//Bucle para probar que funciona la música
	bool run = true;
	SDL_Event event;
	while (SDL_PollEvent(&event) || run) {
		if (event.type == SDL_KEYDOWN) {
			run = false;
			SDL_QUIT;
		}
	}
}

int main(int argc, char* argv[])
{
	CrearJuego();
	return 0;
}