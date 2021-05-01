#pragma once
#include <filesystem>
#include <map>
#include "../ecs/Component.h"
#include "../levels/Level0.h"
#include "../ecs/Entity.h"
#include "MapCollider.h"
#include "../components/Connections.h"

namespace fs = std::filesystem;
const int NUM_TILEMAPS = 40;
using namespace ColLayers;

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

//Opcion con struct
struct RoomNames {
	string name; //Nombre para comparar las conexiones
	string path; //Direcci�n desde la que cargarlo
	bool used;	 //Booleano que se usa para comprobar que la habitaci�n ya ha salido
	int tipo; //0 inicio   1 intermedio   2 finales
};

/// <summary>
/// Crea las habitaciones del mapa y controla sus conexiones. Aplica el componente MapCollider para el manejo de colisiones
/// </summary>
class MapProcedural : public Component {
	const string ruta = "assets/maps/";
	const std::array<char, 4> cardinals = {'N','E','S','W'};
public:

	MapProcedural(int nR, int f);

	~MapProcedural();

	//0 = N, 1 = E, 2 = S, 3 = W
	void init() override;

	void update() override;

	void setTravel(const bool travel, int dir);

	void TravelNextRoom(int dir);

	void travelNextZone();

	Vector2D getPlayerPos();

	bool zoneCompleted();

	int zone();

private:

	void setFase(int f);

	void setNumRooms(int nR);


	void ReadDirectory(const string& p, int& roomsRead);

	//Primera sala
	Room* initializeNewRoom(const RoomNames& tag);

	void CreateConnections(Room* r, const std::array<bool, 4>& rConnections, int dir);

	Room* initializeRoom(Room* partida, int dir);

	void getConec(const string& name, std::array<bool, 4>& cons);

	//Devuelve si se han explorado todas las habitaciones de la zona
	void createConnectionTriggers(int dir);

	static void travel(b2Contact* contact);

	static void travelNextZone(b2Contact* contact);

	static void pescar(b2Contact* contact);

protected:
	int nRooms, nRoomNames = 10;
	int fase;		//Número de la zona en la que está el player
	bool gonTotravel = false, travelZone = false;
	int nextDir = -1;
	//Opcion con struct
	vector<RoomNames> roomNames;

	//Numero de habitaciones exploradas
	int roomsExplored = 1;

	//Habitacion actual
	Room* actualRoom;

	//Divisi�n entre tipos de salas
	std::array<int, 2> fronteras;

	Level0* lvl;

	MapCollider* chainCollider;

	vector<Entity*> triggers;
};
