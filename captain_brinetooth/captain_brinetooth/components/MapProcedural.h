#pragma once
#include "../ecs/Component.h"
#include "../levels/Level0.h"
#include "../ecs/Entity.h"
#include <filesystem>
#include <map>


namespace fs = std::filesystem;
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
	void TravelNextRoom(int dir) {
		//Nueva habitación a la que hemos ido
		actualRoom = actualRoom->conections[dir];

		//Cargamos nuevo mapa
		lvl->load(actualRoom->level);

		//Cogemos sus conexiones	
		getCons(actualRoom->getName(), actualRoom->cons);


		//Creamos habitaciones en función de las conexiones que tiene
		CreateConnections(actualRoom, actualRoom->cons);

	}

	void init() {
		//Leeemos los distintos directorios
		int roomsRead = 0;
		ReadDirectory("assets/maps/level_starts",roomsRead);
		fronteras[0] = roomsRead; //Asertamos la frontera entre inicios y habiaciones normales
		ReadDirectory("assets/maps/level_rooms", roomsRead);
		fronteras[1] = roomsRead; //Asertamos la frontera entre habitaciones y finales
		ReadDirectory("assets/maps/level_ends", roomsRead);

		//Cacheamos el componente Level
		lvl = entity_->getComponent<Level0>();


		int tile = sdlutils().rand().teCuoto(0, fronteras[0]);

		actualRoom = initializeNewRoom(roomNames[tile]);

		roomNames[tile].used = true;
	}


private:
	void ReadDirectory(const string& p, int& roomsRead) {
		std::string path = p;

		for (const auto& entry : fs::directory_iterator(path)) {
			//Cogemos toda la ruta
			string ruta = entry.path().string();

			roomNames[roomsRead].path = ruta; //Se la asignamos al path
			roomNames[roomsRead].used = false;	//No se ha usado la habitación
			roomNames[roomsRead].tipo = 1;		//Tipo start

			//Encontramos donde está la división con el nombre
			int puntoCorte = entry.path().string().find_last_of("\\");

			ruta[puntoCorte] = '/';
			//Nombre real del nivel
			roomNames[roomsRead].name = entry.path().filename().string();
			roomsRead++;
		}
	}


	struct Room {

		string getName() {
			int puntoCorte = level.find_last_of("\\");
			int length = level.length(); //Final del string

			string nombrecito = level.substr(puntoCorte + 1, length);

			return nombrecito;
		}

		string level;	//Nombre del tileMap
		std::array<Room*, 4> conections;	//Conexiones con otras habitaciones
		std::array<bool, 4> cons; //Array para crear las conexiones
	};

	//Opción con struct
	struct RoomNames {
		string name; //Nombre para comparar las conexiones
		string path; //Dirección desde la que cargarlo
		bool used;	 //Booleano que se usa para comprobar que la habitación ya ha salido

		int tipo; //0 inicio   1 intermedio   2 finales
	};


	//Primera sala
	Room* initializeNewRoom(const RoomNames& tag) {

		Room* r = new Room();

		r->level = tag.path;

		lvl->load(r->level);

		//Opcion 1
		getCons(tag.name, r->cons);

		
		//Creamos habitaciones en función de las conexiones que tiene
		/*if(rConnections[0] == true) r->conections[0] = initializeRoom(r, 0);

		if (rConnections[1] == true) r->conections[1] = initializeRoom(r, 1);

		if (rConnections[2] == true) r->conections[2] = initializeRoom(r, 2);

		if (rConnections[3] == true) r->conections[3] = initializeRoom(r, 3);*/
		CreateConnections(r, r->cons);

		return r;
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
		int tile = sdlutils().rand().teCuoto(fronteras[0], fronteras[1] + 1);

		
		//Buscamos hasta encontrar uno que no hayamos usado, quizás podamos hacer divide y vencerás p marcas, como en eda



		//Cogemos la posición opuesta
		int opositeDir = dir + 2;

		if (opositeDir >= 4) opositeDir = opositeDir - 4;

		bool concuerda = false;
		while (roomNames[tile].used && !concuerda) {
			tile = sdlutils().rand().teCuoto(fronteras[0], fronteras[1] + 1);

			if (roomNames[tile].name[opositeDir] == cardinals[opositeDir])concuerda = true;
		}
		//Si la habitación tiene una conexión, la del otro lado tiene que tener conexión opuesta
		//Bueno esto lo tengo mirar pero es esto basicamente, buscar una con esa dirección que pareces tonto


		r->level = roomNames[tile].path;

		/*if (partida->conections[0] != nullptr) r->level = "assets/maps/" + tile;
		
		if (partida->conections[1] != nullptr) r->level = "assets/maps/" + tile;

		if (partida->conections[2] != nullptr) r->level = "assets/maps" + tile;

		if (partida->conections[3] != nullptr) r->level = "assets/maps/" + tile;*/
		return r;
	}

	void getCons(const string& name, std::array<bool,4>& cons) {

		for (int i = 0; i < 4; i++) {
			if (name[i] == cardinals[i]) cons[i] = true;
		}

	}

protected:

	int nRooms, nRoomNames = 10;

	//Opcion con struct
	std::array<RoomNames, 20> roomNames;

	int roomsExplored = 0;

	Level0* lvl;

	Room* actualRoom;

	//División entre tipos de salas
	std::array<int, 2> fronteras;
};
