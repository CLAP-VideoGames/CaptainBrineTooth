#pragma once
#include "../ecs/Component.h"
#include "../levels/Level0.h"
#include "../ecs/Entity.h"
#include <map>

//Le metearimos el componente al GM
class Map : public Component {

public:
	Map(int nR) {
		nRooms = nR;
		lvl = nullptr;
	}

	~Map() {

		for (Room* r : rooms) {
			delete r;
		}

		rooms.clear();
	}


private:
	void init() {
		lvl = entity_->getComponent<Level0>();
		initRoomNames();
		initializeRoom(0);
	}

	struct Room {

		string level;	//Nombre del tileMap
		std::vector<Entity*> enemies;	//Enemigos
		std::array<Room*, 4> conections;	//Conexiones con otras habitaciones
	};

	//Opci�n con struct
	struct RoomNames {
		string name;
		bool used;
	};

	//Este es bastante mierda
	void initializeRooms() {
		for (int i = 0; i < nRooms; i++) {
			Room* r = new Room();

			r->level = "Tecuoto";
			//r->enemies = Hay que hacer un m�todo que inicialice los enemigos de una habitaci�n 
			//conections = Quiz� nos rente hacer recursi�n

			rooms.push_back(r);
		}
	}

	//Lo mismo pero en recursi�n(La i es el n�mero de habitaciones que llevamos inicializadas)
	Room* initializeRoom(int i) {
		
		if (i == nRooms) return nullptr;
		//Tenemos que reconocer donde est�n los extremos, para poder poner habitaciones lim�trofes
		//Y tambi�n deber�amos crear los colliders desde level, btw
		if (i == nRooms - 1) {

		}
		Room* r = new Room();

		//Creo que lo mejor es un mapa, para poder descartar los que no queramos, en su dfecto, podemos tener un array de un struct que hagamos
		int tile = sdlutils().rand().teCuoto(0, nRoomNames + 1);

		//Buscamos hasta encontrar uno que no hayamos usado, quiz�s podamos hacer divide y vencer�s p marcas, como en eda
		while (roomNames[tile].used) tile = sdlutils().rand().teCuoto(0, nRoomNames + 1);


		r->level = roomNames[tile].name;

		//r->enemies = Metodo que usemos en el level para cargarlos(los podemos hacer fijos o aleatorios);

		//Opcion 1
		//std::array<bool, 4>rConnections = Metodo que nos lo devuelve desde Level;

		//Tenemos que saber por donde tiene las salidas para poder incializar sus conexiones
		//Siempre hay que descartar la dirreci�n opuesta desde la que venimos(le podemos sumar 2 y ya), para que no pueda volver hacia atr�s

		//if(rConnections[0] == true) r->conections[0] = initializeRoom(i++);
		//Etc..

		
	}

	void initRoomNames() {
		for (int i = 0; i < 10; i++) {
			//Opci�n de Struct
			roomNames[i].name = "Tile" + i;
			roomNames[i].used = false;
		}
	}

protected:

	int nRooms, nRoomNames = 10;

	//Opcion con struct
	std::array<RoomNames, 10> roomNames;

	//Opcion con mapa
	std::map<string, bool> roomsNams;

	std::vector<Room*> rooms;

	Level0* lvl;
};