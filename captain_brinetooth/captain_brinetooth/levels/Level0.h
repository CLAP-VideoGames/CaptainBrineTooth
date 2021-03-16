#pragma once

#include "SDL.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

typedef int gid;

// Informacion sobre un tile individual
struct tile {
	SDL_Texture* sheet_;
	int x_, y_,	 //Coordenadas en el mundo
		tx_, ty_,	 //Coordenadas en la sprite sheet
		width_, height_;

	tile(SDL_Texture* tset, int x = 0, int y = 0, int tx = 0, int ty = 0, int w = 0, int h = 0);
	void draw(SDL_Renderer* ren);
};

class level {
public:
	level(const string& name);
	void load(const string& path, SDL_Renderer* ren);
	void draw(SDL_Renderer* ren);
private:
	string name_;
	int fils_, cols_,		// filas y columnas de 
		tile_width_, tile_height_;
	vector<tile> tiles_;			// tiles a dibujar en pantalla
	map<gid, SDL_Texture*> tilesets_;	// tilesets usados para dibujar el Tiled Map
};