#pragma once
#include "../ecs/Component.h"
#include "../levels/Level0.h"
#include "../ecs/Entity.h"
#include "ChainCollider.h"
#include <filesystem>
#include <map>
//Los mapas, los vertices de colision tienen que ser en sentido horario

namespace fs = std::filesystem;
/// <summary>
/// Crea las habitaciones del mapa y controla sus conexiones. Aplica el componente ChainCollider para el manejo de colisiones
/// </summary>
class MapProcedural : public Component {
	const string ruta = "assets/maps/";
	const std::array<char, 4> cardinals = {'N','E','S','W'};
public:
	MapProcedural(int nR, int f) {
		nRooms = nR;
		lvl = nullptr;
		fase = f;
	}

	~MapProcedural() {
		if (chainCollider != nullptr) entity_->removeComponent<ChainCollider>();
		delete actualRoom;
	}

	//0 = N, 1 = E, 2 = S, 3 = W
	void init() {
		//Cuando estén las distintas zonas, podemos ordenar con un array y así puedo hacer zona[fase]
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
		//Testing
		//int tile = 0;

		actualRoom = initializeNewRoom(roomNames[tile]);

		roomNames[tile].used = true;
	}

	void TravelNextRoom(int dir) {
		//Nueva habitaci�n a la que hemos ido
		actualRoom = actualRoom->conections[dir];
		//lvl->clearTileset();
		//Cargamos nuevo mapa
		lvl->load(actualRoom->level);
		//Setteamos los nuevos vertices para la creacion del cuerpo Collider
		chainCollider->setVertices(lvl->getVerticesList());
		chainCollider->deleteChains();
		chainCollider->createChainFixture();

		cout << actualRoom->getName();
		//Cogemos sus conexiones	
		getConec(actualRoom->getName(), actualRoom->cons);
		//Creamos habitaciones en funci�n de las conexiones que tiene
		CreateConnections(actualRoom, actualRoom->cons);
	}

private:
	void ReadDirectory(const string& p, int& roomsRead) {
		std::string path = p;

		for (const auto& entry : fs::directory_iterator(path)) {
			//Cogemos toda la ruta
			string ruta = entry.path().u8string();
			//string ruta = entry.path();

			roomNames[roomsRead].path = ruta; //Se la asignamos al path
			roomNames[roomsRead].used = false;	//No se ha usado la habitaci�n
			roomNames[roomsRead].tipo = 1;		//Tipo start

			//Encontramos donde est� la divisi�n con el nombre
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
			string namePath = level.substr(puntoCorte + 1, length);
			return namePath;
		}

		string level;	//Nombre del tileMap
		std::array<Room*, 4> conections;	//Conexiones con otras habitaciones
		std::array<bool, 4> cons; //Array para crear las conexiones
	};

	//Opci�n con struct
	struct RoomNames {
		string name; //Nombre para comparar las conexiones
		string path; //Direcci�n desde la que cargarlo
		bool used;	 //Booleano que se usa para comprobar que la habitaci�n ya ha salido
		int tipo; //0 inicio   1 intermedio   2 finales
	};


	//Primera sala
	Room* initializeNewRoom(const RoomNames& tag) {
		cout << tag.name;
		Room* r = new Room();

		r->level = tag.path;

		lvl->load(r->level);
		
		chainCollider = entity_->addComponent<ChainCollider>(lvl->getVerticesList());
		
		//Opcion 1
		getConec(tag.name, r->cons);
		
		//Creamos habitaciones en funci�n de las conexiones que tiene
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
		int tile;
		if (roomsExplored == nRooms) return nullptr;
		//Tenemos que reconocer donde est�n los extremos, para poder poner habitaciones lim�trofes
		//Y tambi�n deber�amos crear los colliders desde level, btw
		if (roomsExplored == nRooms - 1) {
			//Habitaci�n final
			tile = sdlutils().rand().teCuoto(fronteras[1], roomNames.size()+1);
		}
		else {
			//Habitación intermedia
			tile = sdlutils().rand().teCuoto(fronteras[0], fronteras[1] + 1);
		}

		Room* r = new Room();

		//Cogemos la posici�n opuesta
		int opositeDir = dir + 2;

		if (opositeDir >= 4) opositeDir = opositeDir - 4;

		bool concuerda = (roomNames[tile].name[opositeDir] == cardinals[opositeDir]);
		while (roomNames[tile].used || !concuerda) {
			tile = sdlutils().rand().teCuoto(fronteras[0], fronteras[1] + 1);

			int i = 0;
			//Comprobamos que tiene conexión por el cardinal opuesto
			while (i < 4 && roomNames[tile].name[i] != cardinals[opositeDir]) i++;
			
			if (i < 4) concuerda = true;
		}
		//Si la habitaci�n tiene una conexi�n, la del otro lado tiene que tener conexi�n opuesta
		//Bueno esto lo tengo mirar pero es esto basicamente, buscar una con esa direcci�n que pareces tonto


		r->level = roomNames[tile].path;

		/*if (partida->conections[0] != nullptr) r->level = "assets/maps/" + tile;
		
		if (partida->conections[1] != nullptr) r->level = "assets/maps/" + tile;

		if (partida->conections[2] != nullptr) r->level = "assets/maps" + tile;

		if (partida->conections[3] != nullptr) r->level = "assets/maps/" + tile;*/
		return r;
	}

	void getConec(const string& name, std::array<bool,4>& cons) {
		for (int i = 0; i < 4; i++) 
			for (int j = 0; j < 4; j++) 
				if (name[i] == cardinals[j]) cons[j] = true;
	}

	

protected:
	int nRooms, nRoomNames = 10;

	//Opcion con struct
	std::array<RoomNames, 20> roomNames;

	//Numero de habitaciones exploradas
	int roomsExplored = 0;

	//Habitacion actual
	Room* actualRoom;

	//Divisi�n entre tipos de salas
	std::array<int, 2> fronteras;

	Level0* lvl;

	int fase;
	MapProcedural* nextMap;
	ChainCollider* chainCollider;
};
