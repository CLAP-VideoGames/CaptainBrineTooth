#pragma once
#include "../ecs/Component.h"
#include "../levels/Level0.h"
#include "../ecs/Entity.h"
#include "MapCollider.h"
#include <filesystem>
#include <map>
#include "../components/Connections.h"
//Los mapas, los vertices de colision tienen que ser en sentido horario

namespace fs = std::filesystem;
const int NUM_TILEMAPS = 25;
/// <summary>
/// Crea las habitaciones del mapa y controla sus conexiones. Aplica el componente MapCollider para el manejo de colisiones
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
		delete actualRoom;
	}

	//0 = N, 1 = E, 2 = S, 3 = W
	void init() override {
		//NECESITO SABER LA DIRECCION DE MEMORIA
		//Igual puedo meter los directorios en un array
		//Leeemos los distintos directorios
		

		if (fase == 0) {
			int roomsRead = 0;
			ReadDirectory("assets/maps/level_starts",roomsRead);
			fronteras[0] = roomsRead; //Asertamos la frontera entre inicios y habiaciones normales
			ReadDirectory("assets/maps/level_rooms", roomsRead);
			fronteras[1] = roomsRead; //Asertamos la frontera entre habitaciones y finales
			ReadDirectory("assets/maps/level_ends", roomsRead);

		}
		else if (fase == 1) {
			int roomsRead = 0;
			ReadDirectory("assets/maps/level_starts", roomsRead);
			fronteras[0] = roomsRead; //Asertamos la frontera entre inicios y habiaciones normales
			ReadDirectory("assets/maps/level_rooms", roomsRead);
			fronteras[1] = roomsRead; //Asertamos la frontera entre habitaciones y finales
			ReadDirectory("assets/maps/level_ends", roomsRead);
		}
		else {
			int roomsRead = 0;
			ReadDirectory("assets/maps/level_starts", roomsRead);
			fronteras[0] = roomsRead; //Asertamos la frontera entre inicios y habiaciones normales
			ReadDirectory("assets/maps/level_rooms", roomsRead);
			fronteras[1] = roomsRead; //Asertamos la frontera entre habitaciones y finales
			ReadDirectory("assets/maps/level_ends", roomsRead);
		}

		//Cacheamos el componente Level
		lvl = entity_->getComponent<Level0>();

		int tile = sdlutils().rand().teCuoto(0, fronteras[0]);
		//Testing
		//int tile = 0;

		actualRoom = initializeNewRoom(roomNames[tile]);

		roomNames[tile].used = true;
	}

	void setTravel(const bool travel, int dir){
		gonTotravel = travel;
		nextDir = dir;
	}

	Vector2D getPlayerPos() {
		tmx::Vector2f s = lvl->getPlayerPos();

		Vector2D spawn (s.x, s.y);

		return spawn;
	}

	void update() override{
		if (gonTotravel){
			TravelNextRoom(nextDir);
			nextDir = -1;
			gonTotravel = !gonTotravel;
		}

		if (zoneCompleted()) { 
			init();
			setFase(fase + 1);
			setNumRooms(10);
		}
	}

	void TravelNextRoom(int dir) {
		//Nueva habitaci�n a la que hemos ido
		actualRoom = actualRoom->conections[dir];
		//lvl->clearTileset();
		//Cargamos nuevo mapa
		lvl->load(actualRoom->level);
		//Setteamos los nuevos vertices para la creacion del cuerpo Collider

		entity_->removeComponent<MapCollider>();

		Entity* player = entity_->getMngr()->getHandler<Player>();

		player->getComponent<BoxCollider>()->actPhyscialPos(-300,-30);

		for (Entity* ent : triggers) ent->setActive(false);
		triggers.clear();

		chainCollider = entity_->addComponent<MapCollider>(lvl->getVerticesList());

		//cout << actualRoom->getName();
		//Cogemos sus conexiones	
		getConec(actualRoom->getName(), actualRoom->cons);
		//Creamos habitaciones en funci�n de las conexiones que tiene
		CreateConnections(actualRoom, actualRoom->cons, dir);

		roomsExplored++;

		std::cout << roomsExplored << "\n";
	}

	bool zoneCompleted() { return roomsExplored == nRooms; }

	int zone() { return fase; }
private:

	void setFase(int f) { fase = f; }

	void setNumRooms(int nR) { nRooms = nR; }

	static void travel(b2Contact* contact) {
		Entity* trigger = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
		
		if(trigger == trigger->getMngr()->getHandler<Player>()){
			trigger = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
		}

		auto* m = trigger->getMngr()->getHandler<Map>();

		std::string d= trigger->getComponent<Connections>()->getDir();
		int dir = -1;


		if (d == "N") dir = 0;
		else if (d == "E") dir = 1;
		else if (d == "S") dir = 2;
		else dir = 3;

		m->getComponent<MapProcedural>()->setTravel(true, dir);
	}


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
		std::string test = "assets/maps/level_starts/Stile10.tmx";
		r->level = tag.path;

		lvl->load(r->level);
		
		chainCollider = entity_->addComponent<MapCollider>(lvl->getVerticesList());
		
		//Opcion 1
		getConec(tag.name, r->cons);
		

		CreateConnections(r, r->cons, -1);

		return r;
	}

	void CreateConnections(Room* r, const std::array<bool, 4>& rConnections, int dir) {
		for (int i = 0; i < 4; i++) {
			if (rConnections[i] == true) r->conections[i] = initializeRoom(r, i);

		}
		createConnectionTriggers(dir);
	}

	Room* initializeRoom(Room* partida, int dir) {
		int tile;
		//Tenemos que reconocer donde est�n los extremos, para poder poner habitaciones lim�trofes
		//Y tambi�n deber�amos crear los colliders desde level, btw
		if (roomsExplored == nRooms - 1) {
			//Habitaci�n final
			tile = sdlutils().rand().teCuoto(fronteras[1], roomNames.size());
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


		//Para que no se repitan hay que añadir la condicion ( || roomNames[tile].used) al bucle
		while (!concuerda) {

			if (roomsExplored == nRooms - 1) tile = sdlutils().rand().teCuoto(fronteras[1], roomNames.size()); 
			else tile = sdlutils().rand().teCuoto(fronteras[0], fronteras[1] + 1);

			int i = 0;
			//Comprobamos que tiene conexión por el cardinal opuesto
			while (i < 4 && roomNames[tile].name[i] != cardinals[opositeDir]) i++;
			
			if (i < 4) concuerda = true;
		}
		//Si la habitaci�n tiene una conexi�n, la del otro lado tiene que tener conexi�n opuesta
		//Bueno esto lo tengo mirar pero es esto basicamente, buscar una con esa direcci�n que pareces tonto

		roomNames[tile].used = true;
		r->level = roomNames[tile].path;

		/*if (partida->conections[0] != nullptr) r->level = "assets/maps/" + tile;
		
		if (partida->conections[1] != nullptr) r->level = "assets/maps/" + tile;

		if (partida->conections[2] != nullptr) r->level = "assets/maps" + tile;

		if (partida->conections[3] != nullptr) r->level = "assets/maps/" + tile;*/
		return r;
	}

	void getConec(const string& name, std::array<bool,4>& cons) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) 
				if (name[i] == cardinals[j]) cons[j] = true;
		}
	}


	void createConnectionTriggers(int dir) {
		vector<tmx::Vector2f> positions = lvl->getConPos();	//Las posiciones de las conexiones
		vector<std::string> names = lvl->getConNames();
		std::string oppDir = "";

		if (dir != -1) {

			switch (dir)
		{
		case 0:
			oppDir = "S";
			break;
		case 1:
			oppDir = "W";
			break;
		case 2:
			oppDir = "N";
			break;
		case 3:
			oppDir = "E";
			break;
		default:
			break;
		}
		}

		for (int i = 0; i < positions.size(); i++) {	
			auto* t = entity_->getMngr()->addEntity();

			Vector2D size;
			Vector2D pos(positions[i].x, positions[i].y);

			//Tamaño en función de posición
			if (names[i] == "N" || names[i] == "S") size.set(400, 50);
			else size.set(50, 400);


			if (names[i] == oppDir) {
				entity_->getMngr()->getHandler<Player>()->getComponent<BoxCollider>()->setPhysicalTransform(pos.getX(), pos.getY(), 0);
			}
			else {
				t->addComponent<Transform>(pos, Vector2D(0, 0), size.getX(), size.getY(), 0);

				t->addComponent<BoxCollider>(STATIC, PLAYER_DETECTION, PLAYER_DETECTION_MASK, true, 0, true, 0.0);

				t->addComponent<Connections>(names[i]);

				t->setCollisionMethod(travel);

				triggers.push_back(t);
			}


			//entity_->addComponent<BoxCollider>(STATIC, PLAYER, PLAYER_MASK, true, 0, true, 0.0, positions[i], Vector2D(200,200));

		}
	}
	//Devuelve si se han explorado todas las habitaciones de la zona

protected:
	int nRooms, nRoomNames = 10;
	int fase;		//Número de la zona en la que está el player
	bool gonTotravel = false;
	int nextDir = -1;
	//Opcion con struct
	std::array<RoomNames, NUM_TILEMAPS> roomNames;

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
