#include <iostream>
#include <algorithm>


#include "Level0.h"
#include "../game/App.h"
#include "../assets/assets.h"
#include "..//components/AnimBlendGraph.h"

#include <tmxlite\Layer.hpp>


tile::tile(Texture* tset, int x, int y, int tx, int ty, int w, int h)
	: sheet_(tset), x_(x), y_(y), tx_(tx), ty_(ty), width_(w), height_(h) {}

void tile::draw() {
	if (!sheet_)
		return;

	SDL_Rect src;
	src.x = tx_; src.y = ty_;
	src.w = width_; src.h = height_;

	// Para que el tileset no siga a la cámara, hay que restarle la posición de la misma.
	// Para agrandar el tiledmap, hay que hacerlo manualmente en el propio TiledMapEditor, aumentando los píxeles por tile.
	SDL_Rect dest;
	dest.x = x_ - App::camera.x; dest.y = y_ - App::camera.y;
	dest.w = src.w; dest.h = src.h;

	sheet_->render(src, dest);
}

object::object(float oX, float oY, float oWidth, float oHeight)
	: oX_(oX), oY_(oY), oWidth_(oWidth), oHeight_(oHeight) {}

Level0::Level0(const string& name, std::shared_ptr<b2World> b2World)
	: name_(name), fils_(0), cols_(0), b2World_(b2World) {
	//load(name);
}

//Donde carguemos los enemigos hay que extraerlo en un método que nos devuelva la lista
//A las salas hay que meterles un atributo que sean las salidas, preferiblemente un bool
void Level0::load(const string& path) {
	if (tiles_.size() > 0) {
		for (tile* tile__ : tiles_) delete tile__;
		tiles_.clear();
	}
	//Limpiamos todos los vectores.
	if (tilesets_.size() > 0) tilesets_.clear();
	if (enemiePos.size() > 0)enemiePos.clear();
	if (points.size() > 0)points.clear();
	if (connectionPos.size() > 0)connectionPos.clear();
	if (connectionSize.size() > 0)connectionSize.clear();
	if (connectionsNames.size() > 0)connectionsNames.clear();


	//carga el mapa con TMXLite
	tmx::Map tiled_map;

	tiled_map.load(path);

	//dimensiones del mapa en tiles
	auto map_dimensions = tiled_map.getTileCount();
	fils_ = map_dimensions.y;
	cols_ = map_dimensions.x;

	//dimensiones de los tiles
	auto tilesize = tiled_map.getTileSize();
	tile_width_ = tilesize.x;
	tile_height_ = tilesize.y;

	//carga todo los tilesets y los guarda en un 'map'
	auto& map_tilesets = tiled_map.getTilesets();
	for (auto& tset : map_tilesets) {
		Texture* tex = &sdlutils().tilesets().find("level0")->second;
		tilesets_.insert(pair<gid, Texture*>(tset.getFirstGID(), tex));
	}

	//Lectura y guardado del Tiled Map de abajo a arriba.
	auto& map_layers = tiled_map.getLayers();
	for (auto& layer : map_layers) {
		//comprobamos si es una capa con Tiles en ella.
		if (layer->getType() != tmx::Layer::Type::Tile)
			continue;

		auto* tile_layer = dynamic_cast<const tmx::TileLayer*>(layer.get());
		//se obtienen todos los tiles de esta capa
		auto& layer_tiles = tile_layer->getTiles();

		//recorremos cada coordenada buscando d�nde hay tiles
		for (auto y = 0; y < fils_; y++) {
			for (auto x = 0; x < cols_; x++) {
				//como layer_tiles no es un array 2D, 
				//convertimos las coordenadas x, y en un �ndice para poder accederlo.
				auto tile_index = x + (y * cols_);

				//GID del tile actual (current GID -> cur_gid)
				auto cur_gid = layer_tiles[tile_index].ID;

				// si GID = 0, la tile esta vac�a.
				if (cur_gid == 0)
					continue;

				// si no es 0, buscamos el tileset al cual pertenece esta tile.
				auto tset_gid = -1;
				for (auto& ts : tilesets_) {
					if (ts.first <= cur_gid) {
						tset_gid = ts.first;
					}
					else
						break;
				}

				// si no se encuentra el tileset, no se dibuja nada
				if (tset_gid == -1)
					continue;

				cur_gid -= tset_gid; //normalizamos el GID

				//Dimensiones de la tile sheet.
				auto ts_width = 0, ts_height = 0;
				SDL_QueryTexture(tilesets_[tset_gid]->getTexture(), NULL, NULL, &ts_width, &ts_height);

				//area dentro de la tile sheet de la que dibujar
				auto region_x = (cur_gid % (ts_width / tile_width_)) * tile_width_,
					region_y = (cur_gid / (ts_width / tile_width_)) * tile_height_;

				//posicion en el mundo del tile.
				auto x_pos = x * tile_width_,
					y_pos = y * tile_height_;

				tiles_.push_back(new tile(tilesets_[tset_gid], x_pos, y_pos, region_x, region_y, tile_width_, tile_height_));
				//No podemos usar entity_ aquí porque todavía no se ha seteado
			}
		}
	}

	for (auto& layerObj : map_layers) {
		if (layerObj->getType() != tmx::Layer::Type::Object) continue;
		auto* object_layer = dynamic_cast<const tmx::ObjectGroup*>(layerObj.get());
		if (object_layer->getName() == "collision") {
			auto layer_objects = object_layer->getObjects();
			//las coordenadas de los puntos reales son layer_objects.back().getPosition() + layer_objects.back().getPoints()[i];
			int j = 0;
			for (auto& object : layer_objects) {
				points.push_back(object.getPoints());
				for (int i = 0; i < points[j].size(); i++) {
					points[j][i].x += object.getPosition().x;
					points[j][i].y += object.getPosition().y;

					points[j][i].x /= sdlutils().getPPM();
					points[j][i].y /= sdlutils().getPPM();
				}
				j++;
			}
		}

		if (object_layer->getName() == "spawns") {
			auto layer_objects = object_layer->getObjects();

			for (auto& spawn : layer_objects) {
				if (spawn.getName() == "player"){
					playerPos = spawn.getPosition();
				}
				else {
					tmx::Vector2f spa = spawn.getPosition();

					spa.x /= sdlutils().getPPM();
					spa.y /= sdlutils().getPPM();

					enemiePos.push_back(spa);

				}
			}
		}

		if (object_layer->getName() == "conx") {
			auto layer_objects = object_layer->getObjects();

			for (auto& c : layer_objects) {
				tmx::Vector2f size(c.getAABB().width, c.getAABB().height);
				//Obtenemos el tamaño del Trigger
				connectionSize.push_back(size); 
				//Al parecer el Trasform si que funciona apartir del punto central
				// Asi que hay que posicionar el objeto sumandole la mitad de sus ejes
				tmx::Vector2f con = c.getPosition();
				con.x += (size.x/2);
				con.y += (size.y/2);
				connectionPos.push_back(con);
				//Se obtiene el nombre de la capa
				connectionsNames.push_back(c.getName());
			}
		}
	}
}

void Level0::setPlayerPos()
{
	auto* p = entity_->getMngr()->getHandler<Player>();

	Vector2D plaPos(playerPos.x, playerPos.y);

	p->getComponent<Transform>()->getPos().set(plaPos);
}

void Level0::clearTileset()
{
	tilesets_.clear();
	tiles_.clear();
}

void Level0::render() {
	for (auto tile : tiles_)
		tile->draw();

}
