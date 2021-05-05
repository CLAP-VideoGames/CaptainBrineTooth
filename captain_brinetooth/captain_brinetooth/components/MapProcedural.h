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
const string LOBBY = "assets/maps\\Etile0.tmx";	
using namespace ColLayers;

enum class Cardinals { N, E, S, W, None};

struct Room {
	Room() {};

	~Room() {
		
	};

	string nameLevel;
	string level;	//Nombre del tileMap
};



struct CurrentRoom {

	CurrentRoom(){
		for (bool& con : cons)
			con = false;

	}

	~CurrentRoom() {

	};

	string nameLevel;
	string level;	//Nombre del tileMap
	std::array<Room, 4> conections;	//Conexiones con otras habitaciones
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

	MapProcedural(int nR, int f, App* s);

	~MapProcedural();

	//0 = N, 1 = E, 2 = S, 3 = W
	void init() override;

	void update() override;

	void setTravel(const bool travel, int dir);

	void TravelNextRoom(int dir);

	void travelNextZone();

	bool isZoneCompleted();

	int getPhase();

	Vector2D getPlayerPos();

	App* getStates() { return states; }

private:

	void setPhase(int f);

	void setNumRooms(int nR);

	void ReadDirectory(const string& p, int& roomsRead);

	/// <summary>
	/// Inicializa la primera sala
	/// </summary>
	/// <param name="tag"></param>
	/// <returns></returns>
	CurrentRoom* initializeNewRoom(const RoomNames& tag);

	/// <summary>
	/// Inicializa cada habitacion
	/// </summary>
	/// <param name="dir"></param>
	/// <returns></returns>
	Room initializeRoom(int dir);

	/// <summary>
	/// Crea las conexiones de cada habitacion
	/// </summary>
	/// <param name="r"></param>
	/// <param name="rConnections"></param>
	/// <param name="dir">direccion a la que se dirige el player</param>
	void CreateConnections(CurrentRoom* r, const std::array<bool, 4>& rConnections, int dir);

	/// <summary>
	/// Obtiene las conexiones de un habitacion en concreto
	/// </summary>
	/// <param name="name"></param>
	/// <param name="cons"></param>
	void getConec(const string& name, std::array<bool, 4>& cons);

	/// <summary>
	/// Crea los triggers de una habitacion en funcion de sus salidas
	/// </summary>
	/// <param name="dir"></param>
	void createConnectionTriggers(int dir);

	void stoppedFishing(){stopFishing = true;}

	//Callbacks
	static void travel(b2Contact* contact);

	static void travelNextZone(b2Contact* contact);

	static void pescar(b2Contact* contact);

protected:
	int nRooms, nRoomNames = 10;
	int fase;		//Número de la zona en la que está el player
	bool gonTotravel = false, travelZone = false, stopFishing = false;

	Cardinals nextDirection = Cardinals::None;
	int nextDir = -1;
	//Opcion con struct
	vector<RoomNames> roomNames;

	//Numero de habitaciones exploradas
	int roomsExplored = 1;

	//Habitacion actual
	//Room* actualRoom;
	CurrentRoom* actualRoom;

	//Divisi�n entre tipos de salas
	std::array<int, 2> fronteras;

	Level0* lvl;

	MapCollider* chainCollider;

	vector<Entity*> triggers;
	vector<Entity*> pesca;

	App* states;

	bool lobby;
};
