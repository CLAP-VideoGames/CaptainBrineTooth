#pragma once
#include "../ecs/Component.h"
#include "../levels/Level0.h"
#include "../ecs/Entity.h"

//Le metearimos el componente al GM
class Map : public Component {

public:
	Map(int nR) {
		nRooms = nR;
	}

	~Map() {

		for (Room* r : rooms) {
			delete r;
		}

		rooms.clear();
	}


private:
	struct Room {

		string level;	//Nombre del tileMap
		std::vector<Entity*> enemies;	//Enemigos
		std::array<Room*, 4> conections;	//Conexiones con otras habitaciones
	};

	void initializeRooms() {
		for (int i = 0; i < nRooms; i++) {
			Room* r = new Room();

			r->level = "Tecuoto";
			//r->enemies = Hay que hacer un método que inicialice los enemigos de una habitación 
			//conections = Quizá nos rente hacer recursión

			rooms.push_back(r);
		}
	}

	void initRoomNames() {
		for (int i = 0; i < 10; i++) {
			roomNames[i] = "tile" + i;
		}
	}

protected:

	int nRooms, nRoomNames = 10;

	std::array<string, 10> roomNames;

	std::vector<Room*> rooms;
};