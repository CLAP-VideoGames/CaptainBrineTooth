#pragma once

#include "SDL.h"
#include <string>
#include <vector>
#include <map>
#include "../sdlutils/Texture.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Component.h"

using namespace std;

typedef int gid;

// Informacion sobre un tile individual
struct tile {
	Texture* sheet_;
	int x_, y_,	 //Coordenadas en el mundo
		tx_, ty_,	 //Coordenadas en la sprite sheet
		width_, height_;

	tile(Texture* tset, int x = 0, int y = 0, int tx = 0, int ty = 0, int w = 0, int h = 0);
	void draw();
};

class Level0 : public Component{
public:
	Level0(const string& name);
	void render() override;
private:
	string name_;
	int fils_, cols_,		// filas y columnas de 
		tile_width_, tile_height_;
	vector<tile*> tiles_;			// tiles a dibujar en pantalla
	map<gid, Texture*> tilesets_;	// tilesets usados para dibujar el Tiled Map
	void load(const string& path);
};