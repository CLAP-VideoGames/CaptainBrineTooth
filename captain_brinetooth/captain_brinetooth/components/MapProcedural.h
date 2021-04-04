#pragma once
#include "../ecs/Component.h"
#include "../levels/Level0.h"
#include "../ecs/Entity.h"
#include <map>

//Le metearimos el componente al GM
class MapProcedural : public Component {

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
		//Variable auxiliar
		Room* part = actualRoom;

		//Nueva habitación a la que hemos ido
		actualRoom = actualRoom->conections[partida];

		//Cargamos nuevo mapa
		lvl->load(part->conections[partida]->level);

		//Cogemos sus conexiones
		std::array<bool, 4>rConnections = lvl->returnRoomCons();



		//Creamos habitaciones en función de las conexiones que tiene
		if (rConnections[0] == true) actualRoom->conections[0] = initializeRoom(actualRoom);

		if (rConnections[1] == true) actualRoom->conections[1] = initializeRoom(actualRoom);

		if (rConnections[2] == true) actualRoom->conections[1] = initializeRoom(actualRoom);

		if (rConnections[3] == true) actualRoom->conections[1] = initializeRoom(actualRoom);

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


	//Lo mismo pero en recursión(La i es el número de habitaciones que llevamos inicializadas)
	Room* initializeNewRoom(const string& path) {

		Room* r = new Room();

		lvl->load(r->level);

		//Opcion 1
		std::array<bool, 4>rConnections = lvl->returnRoomCons();


		//Creamos habitaciones en función de las conexiones que tiene
		if(rConnections[0] == true) r->conections[0] = initializeRoom(r);

		if (rConnections[1] == true) r->conections[1] = initializeRoom(r);

		if (rConnections[2] == true) r->conections[2] = initializeRoom(r);

		if (rConnections[3] == true) r->conections[3] = initializeRoom(r);


	}

	Room* initializeRoom(Room* partida) {
		if (roomsExplored == nRooms) return nullptr;
		//Tenemos que reconocer donde están los extremos, para poder poner habitaciones limítrofes
		//Y también deberíamos crear los colliders desde level, btw
		if (roomsExplored == nRooms - 1) {
			//Habitación final
		}

		Room* r = new Room();

		int tile = sdlutils().rand().teCuoto(0, nRoomNames + 1);


		//Buscamos hasta encontrar uno que no hayamos usado, quizás podamos hacer divide y vencerás p marcas, como en eda
		while (roomNames[tile].used) tile = sdlutils().rand().teCuoto(0, nRoomNames + 1);

		//Si la habitación tiene una conexión, la del otro lado tiene que tener conexión opuesta
		if (partida->conections[0] != nullptr) r->level = "assets/maps/S/" + tile;
		
		if (partida->conections[1] != nullptr) r->level = "assets/maps/W/" + tile;

		if (partida->conections[2] != nullptr) r->level = "assets/maps/N/" + tile;

		if (partida->conections[3] != nullptr) r->level = "assets/maps/E/" + tile;
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
