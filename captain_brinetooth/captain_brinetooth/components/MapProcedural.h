#pragma once
#include <filesystem>
#include <map>
#include "../ecs/Component.h"
#include "../levels/Level0.h"
#include "../ecs/Entity.h"
#include "MapCollider.h"
#include "../components/Connections.h"
#include "ParallaxScroll.h"

static BoxCollider* playerCollider_;
namespace fs = std::filesystem;
const int NUM_TILEMAPS = 40;
//r->level = "assets/maps/level_rooms0\\NEtile3.tmx";
const string LOBBY = "assets/maps\\Etile0.tmx";	
using namespace ColLayers;

					//0 = N, 1 = E, 2 = S, 3 = W
enum class Cardinals { N , E , S, W, None };

enum Area {Starts, Mid, Final, Boss};

struct Room {
	string nameLevel;
	string level;	//Nombre del tileMap
};

struct CurrentRoom {
	CurrentRoom(){
		for (bool& con : cons) con = false;
		nameLevel = level = "";
	}

	string nameLevel;
	string level;	//Nombre del tileMap
	std::array<Room, 4> conections;	//Conexiones con otras habitaciones
	std::array<bool, 4> cons; //Array para crear las conexiones
};

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
	std::array<char, 4> cardinals = {'N','E','S','W'};
public:

	MapProcedural(int nR, int f, App* s);

	~MapProcedural();

	void init() override;

	void update() override;

	void deleteTriggers();

	//-------SET RUN------------------
	void loadTileFiles();

	void TravelNextRoom(int dir);

	void setTravelZone();

	void loadLobby();
	
	void setPlayer2spawn();

	void initRun();

	void initBoss();

	void startRun(bool start);

	void setTravel(const bool travel, int dir);

	void refreshCollider();

	//---------- GETTERS--------------
	bool isZoneCompleted();

	int getPhase();

	int getRandomTileFromArea(Area a);

	App* getStates() { return states; }
	
	Vector2D getPlayerPos();
private:

	//---------- CALLBACKS --------------
	static void travel(b2Contact* contact);

	static void leaveLobby(b2Contact* contact);

	static void travelNextZone(b2Contact* contact);

	static void pescar(b2Contact* contact);

	//---------CONNECTIONS---------------
	int checkMatch(const char& ch);
	
	Cardinals getOppositeDir(Cardinals dir);
	
	/// <summary>
	/// Inicializa la primera sala
	/// </summary>
	/// <param name="tag"></param>
	/// <returns></returns>
	CurrentRoom* initilizeCurrentRoom(const RoomNames& tag);

	/// <summary>
	/// Inicializa cada habitacion
	/// </summary>
	/// <param name="dir"></param>
	/// <returns></returns>
	Room initializeRoom(int dir);

	bool matchConnections(int tileNum, Cardinals oppositeDir);

	/// <summary>
	/// Inicializa las conexiones de la habitacion actual
	/// </summary>
	/// <param name="dir"></param>
	/// <returns></returns>
	void initConnections(CurrentRoom* r);

	void setConnections(CurrentRoom* r, const std::array<bool, 4>& rConnections);

	/// <summary>
	/// Dado el nombre del archivo de un Tile, selecciona sus conexiones
	/// </summary>
	/// <param name="name"></param>
	/// <param name="cons"></param>
	void getConec(const string& name, std::array<bool, 4>& cons);

	/// <summary>
	/// Crea los triggers de una habitacion en funcion de sus salidas
	/// </summary>
	/// <param name="dir"></param>
	void createConnectionTriggers(int dir, CallBackCollision* method);
	//-----------------------------------

	void setPhase(int f);

	void setNumRooms(int nR);

	void ReadDirectory(const string& p, int& roomsRead);

	void stoppedFishing() {stopFishing = true;}

protected:
	bool gonTotravel, travelZone, stopFishing, startRun_;
	int nRooms, nRoomNames = 10, nextDir = -1;
	int roomsExplored;	//Numero de habitaciones exploradas
	int fase;				//Número de la zona en la que está el player

	App* states;
	ParallaxScroll* backgroundLayer;
	Level0* lvl;
	MapCollider* chainCollider;

	//Habitacion actual
	CurrentRoom* currentRoom;

	Cardinals nextDirection = Cardinals::None;

	//numero de limites de areas starts|midRooms|finalRooms 
	std::array<int, 2> areaLimits;

	vector<RoomNames> roomNames;//vector de todos los nombres de las habitaciones
	vector<Entity*> triggers;	//Vector de triggers cardinales
	vector<Entity*> pesca;		//Vector de triggers de pesca	
};
