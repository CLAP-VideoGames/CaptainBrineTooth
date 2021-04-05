#pragma once
#include "../ecs/Component.h"
#include "../levels/Level0.h"
#include "../ecs/Entity.h"
#include <map>

//Le metearimos el componente al GM
class MapProcedural : public Component {
	const string ruta = "assets/maps/";
	const std::array<char, 4> cardinals = {'N','E','S','W'};
public:
	MapProcedural(int nR) {
		nRooms = nR;
		lvl = nullptr;
	}

	~MapProcedural() {

		delete actualRoom;
	}

	//0 = N, 1 = E, 2 = S, 3 = W
	void TravelNextRoom(int partida) {
		//Nueva habitación a la que hemos ido
		actualRoom = actualRoom->conections[partida];

		//Cargamos nuevo mapa
		lvl->load(actualRoom->level);

		//Cogemos sus conexiones
		std::array<bool, 4>rConnections = lvl->returnRoomCons();



		//Creamos habitaciones en función de las conexiones que tiene
		/*if (rConnections[0] == true) actualRoom->conections[0] = initializeRoom(actualRoom, 0);

		if (rConnections[1] == true) actualRoom->conections[1] = initializeRoom(actualRoom, 1);

		if (rConnections[2] == true) actualRoom->conections[1] = initializeRoom(actualRoom, 2);

		if (rConnections[3] == true) actualRoom->conections[1] = initializeRoom(actualRoom, 3);*/

		CreateConnections(actualRoom, rConnections);

	}

private:
	void init() {
		lvl = entity_->getComponent<Level0>();
		initRoomNames();
		int tile = sdlutils().rand().teCuoto(0, nRoomNames + 1);
		actualRoom = initializeNewRoom("assets/maps/inicial/" + roomNames[tile].name);

		roomNames[tile].used = true;
	}

	struct Room {

		string level;	//Nombre del tileMap
		std::array<Room*, 4> conections;	//Conexiones con otras habitaciones
	};

	//Opción con struct
	struct RoomNames {
		string name;
		bool used;
	};


	//Primera sala
	Room* initializeNewRoom(const string& path) {

		Room* r = new Room();

		r->level = path;

		lvl->load(r->level);

		//Opcion 1
		std::array<bool, 4>rConnections = lvl->returnRoomCons();

		
		//Creamos habitaciones en función de las conexiones que tiene
		/*if(rConnections[0] == true) r->conections[0] = initializeRoom(r, 0);

		if (rConnections[1] == true) r->conections[1] = initializeRoom(r, 1);

		if (rConnections[2] == true) r->conections[2] = initializeRoom(r, 2);

		if (rConnections[3] == true) r->conections[3] = initializeRoom(r, 3);*/
		CreateConnections(r, rConnections);

	}

	void CreateConnections(Room* r, const std::array<bool, 4>& rConnections) {
		for (int i = 0; i < 4; i++) {
			if (rConnections[i] == true) r->conections[i] = initializeRoom(r, i);
		}
	}

	Room* initializeRoom(Room* partida, int dir) {
		if (roomsExplored == nRooms) return nullptr;
		//Tenemos que reconocer donde están los extremos, para poder poner habitaciones limítrofes
		//Y también deberíamos crear los colliders desde level, btw
		if (roomsExplored == nRooms - 1) {
			//Habitación final
		}

		Room* r = new Room();

		//Seleccionaría aquí uno que tenga una entrada por el cardinal opuesto
		int tile = sdlutils().rand().teCuoto(0, nRoomNames + 1);

		
		//Buscamos hasta encontrar uno que no hayamos usado, quizás podamos hacer divide y vencerás p marcas, como en eda
		while (roomNames[tile].used) tile = sdlutils().rand().teCuoto(0, nRoomNames + 1);

		r->level = ruta + roomNames[tile].name;


		//Cogemos la posición opuesta
		int opositeDir = dir + 2;

		if (opositeDir >= 4) opositeDir = opositeDir - 4;

		//Si la habitación tiene una conexión, la del otro lado tiene que tener conexión opuesta
		//Bueno esto lo tengo mirar pero es esto basicamente, buscar una con esa dirección que pareces tonto
		for (int i = 0; i < 4 && roomNames[tile].used; i++) {
			for (int j = 0; j < 4 && roomNames[tile].used; j++) {
				if (roomNames[tile].name[opositeDir] == cardinals[j] && !roomNames[tile].used) roomNames[tile].used = true;
			}
		}

		/*if (partida->conections[0] != nullptr) r->level = "assets/maps/" + tile;
		
		if (partida->conections[1] != nullptr) r->level = "assets/maps/" + tile;

		if (partida->conections[2] != nullptr) r->level = "assets/maps" + tile;

		if (partida->conections[3] != nullptr) r->level = "assets/maps/" + tile;*/
		return r;
	}

	void initRoomNames() {
		for (int i = 0; i < nRoomNames; i++) {
			//Opción de Struct
			roomNames[i].name = "Tile" + i;
			roomNames[i].used = false;
		}
	}

protected:

	int nRooms, nRoomNames = 10;

	//Opcion con struct
	std::array<RoomNames, 10> roomNames;

	int roomsExplored = 0;

	Level0* lvl;

	Room* actualRoom;
};
