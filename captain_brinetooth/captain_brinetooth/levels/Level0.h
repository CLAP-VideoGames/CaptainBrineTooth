#pragma once

#include "SDL.h"
#include <string>
#include <vector>
#include <map>
#include "box2d.h"
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

struct object {
	float oX_, oY_, oWidth_, oHeight_;
	object(float oX, float oY, float oWidth, float oHeight);
};

class Level0 : public Component{
public:
	Level0(const string& name, b2World* b2World);
	void render() override;
	Vector2D getFilsCols() { return Vector2D(fils_, cols_); }
	Vector2D getTileSize() { return Vector2D(tile_height_, tile_width_); }
private:
	string name_;
	int fils_, cols_,		// filas y columnas de 
		tile_width_, tile_height_;
	vector<tile*> tiles_;			// tiles a dibujar en pantalla
	vector<object*> objects_;		// objetos (colisiones) en la capa de Tiled Map
	map<gid, Texture*> tilesets_;	// tilesets usados para dibujar el Tiled Map
	void load(const string& path);
	b2World* b2World_ = nullptr;
	b2Body* body_ = nullptr;
	b2Fixture* fixture_ = nullptr;
};