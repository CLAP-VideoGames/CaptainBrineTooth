#include "MapProcedural.h"

#define _CRTDBG_MAP_ALLOC
#include<iostream>
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

MapProcedural::MapProcedural(int nR, int f, App* s){
	nRooms = nR;
	lvl = nullptr;
	fase = f;
	states = s;
	lobby = true;
}

MapProcedural::~MapProcedural() {
	//Borramos los triggers
	deleteTriggers();

	delete currentRoom;
}

void MapProcedural::init() {
	loadTileFiles();

	//En funcion de la fase, obtenemos de manera aletoria el nombre de una de las salas  
	//Testing
	//int tile = 0;

	int tile = getRandomTileFromArea(Starts);
	if (!lobby) currentRoom = initializeNewRoom(roomNames[tile]);
	else loadLobby();

	roomNames[tile].used = true;
}

CurrentRoom* MapProcedural::initializeNewRoom(const RoomNames& tag) {
	cout << tag.name;
	CurrentRoom* r = new CurrentRoom();
	//Test
	//std::string test = "assets/maps/level_rooms0\\NEtile3.tmx";
	//r->level = test;
	//lvl->load(test);
	
	//Real
	r->level = tag.path;
	lvl->load(r->level);
	

	r->nameLevel = sdlutils().getNameFilePath(r->level);

	if(!entity_->hasComponent<MapCollider>())chainCollider = entity_->addComponent<MapCollider>(lvl->getVerticesList(), GROUND, GROUND_MASK);

	////Setteamos las conexiones en funcion del nombre del archivo

	getConec(r->nameLevel, r->cons);

	CreateConnections(r, r->cons, -1);

	return r;
}

/// <summary>
/// Dado el nombre del archivo de un Tile, selecciona sus conexiones
/// </summary>
/// <param name="name"></param>
/// <param name="cons"></param>
void MapProcedural::getConec(const string& name, std::array<bool, 4>& cons) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++){

			if (name[i] == cardinals[j]) 
				cons[j] = true;
		}
	}
}

void MapProcedural::CreateConnections(CurrentRoom* r, const std::array<bool, 4> & rConnections, int dir) {
	for (int i = 0; i < 4; i++) {
		if (rConnections[i] == true) 
			r->conections[i] = initializeRoom(i);
	}
	createConnectionTriggers(dir);
}

void MapProcedural::createConnectionTriggers(int dir) {
	vector<tmx::Vector2f> positions = lvl->getConPos();	//Las posiciones de las conexiones
	vector<tmx::Vector2f> size = lvl->getConSize();	//Los tamaños de las conexiones
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

		//Vector2D size;
		Vector2D pos(positions[i].x, positions[i].y);

		
		if (names[i] == oppDir) {
			entity_->getMngr()->getHandler<Player>()->getComponent<BoxCollider>()->setPhysicalTransform(pos.getX() + (size[i].x / 2), pos.getY() + (size[i].y / 2), 0);
		}
		else {
			t->addComponent<Transform>(pos, Vector2D(0, 0), size[i].x, size[i].y, 0);

			t->addComponent<BoxCollider>(STATIC, PLAYER_DETECTION, PLAYER_DETECTION_MASK, true, 0, true, 0.0);

			t->addComponent<Connections>(names[i]);

			t->setCollisionMethod(travel);

			triggers.push_back(t);
		}
		tmx::Vector2f playerpos = lvl->getPlayerPos();

		auto* player = entity_->getMngr()->getHandler<Player>();
		if (player != NULL && roomsExplored <= 1 && playerpos.x != 0)
			player->getComponent<BoxCollider>()->setPhysicalTransform(playerpos.x, playerpos.y, 0);
		//entity_->addComponent<BoxCollider>(STATIC, PLAYER, PLAYER_MASK, true, 0, true, 0.0, positions[i], Vector2D(200,200));

	}

	if (lvl->finalRoom()) {
		auto end = lvl->getEnd();

		auto* t = entity_->getMngr()->addEntity();

		Vector2D pos(end.x, end.y);

		t->addComponent<Transform>(pos, Vector2D(0, 0), 200, 200, 0);

		t->addComponent<BoxCollider>(STATIC, PLAYER_DETECTION, PLAYER_DETECTION_MASK, true, 0, true, 0.0);

		t->setCollisionMethod(travelNextZone);

		triggers.push_back(t);

	}

	vector<tmx::Vector2f> posAux = lvl->getPescaPoints();


	//Comprobamos que hay un punto de pesca
	if (posAux.size() != 0) {
		for (int i = 0; i < posAux.size(); i++) {
			Vector2D pescaPos(posAux[i].x, posAux[i].y);

			auto* t = entity_->getMngr()->addEntity();

			t->addComponent<Transform>(pescaPos, Vector2D(0, 0), 200, 200, 0);

			t->addComponent<BoxCollider>(STATIC, PLAYER_DETECTION, PLAYER_DETECTION_MASK, true, 0, true, 0.0);

			t->setCollisionMethod(pescar);

			pesca.push_back(t);
		}
	}
}

Room MapProcedural::initializeRoom(int dir) {
	int tile;
	bool concuerda = false;
	if (roomsExplored <= 1){
		concuerda = true;
		entity_->getMngr()->getSoundMngr()->ChangeMainMusic("Nivel1");
		tile = getRandomTileFromArea(Starts);
		//tile = sdlutils().rand().teCuoto(0, areaLimits[0]);
	}
	//Habitacion final
	else if (roomsExplored == nRooms - 1) {
		tile = getRandomTileFromArea(Final);
		//tile = sdlutils().rand().teCuoto(areaLimits[1], roomNames.size());
	}
	//Habitación intermedia
	else {
		tile = sdlutils().rand().teCuoto(areaLimits[0], areaLimits[1] + 1);
	}

	Room r;

	//Cogemos la posici�n opuesta
	int opositeDir = dir + 2;

	//Si es 2 = "S", luego es 4, y el opuesto de S es N, luego es 0
	//Si es 3 = "W", luego es 5, y el opuesto de W es E, luego es 1
	if (opositeDir >= 4) opositeDir = opositeDir - 4;

	if (!concuerda){
		int i = 0;
		while (i < 4 && roomNames[tile].name[i] != cardinals[opositeDir]) i++;
		if (i < 4) concuerda = true;
	}


	//Para que no se repitan hay que añadir la condicion ( || roomNames[tile].used) al bucle
	while (!concuerda) {

	/*	if(roomsExplored <= 1)
			tile = sdlutils().rand().teCuoto(0, fronteras[0]);*/
		if (roomsExplored == nRooms - 1)
			tile = getRandomTileFromArea(Final);
			//tile =  sdlutils().rand().teCuoto(areaLimits[1], roomNames.size());
		else
			tile = getRandomTileFromArea(Mid);

			//tile = sdlutils().rand().teCuoto(areaLimits[0], areaLimits[1]);

		int i = 0;
		//Comprobamos que tiene conexión por el cardinal opuesto
		//Quite good this
		while (i < 4 && roomNames[tile].name[i] != cardinals[opositeDir]) i++;

		if (i < 4) concuerda = true;
	}

	roomNames[tile].used = true;
	r.level = roomNames[tile].path;
	r.nameLevel = sdlutils().getNameFilePath(roomNames[tile].path);

	return r;
}



void MapProcedural::update() {

	//Cambia de habitación
	if (gonTotravel) {
		TravelNextRoom(nextDir);
		nextDir = -1;
		gonTotravel = !gonTotravel;
	}

	//Cambia de zona
	if (travelZone) {
		lvl->traveled();
		roomsExplored = 0;

		entity_->removeComponent<MapCollider>();

		for (Entity* ent : triggers) ent->setActive(false);
		triggers.clear();

		chainCollider = entity_->addComponent<MapCollider>(lvl->getVerticesList(), GROUND, GROUND_MASK);

		setPhase(fase + 1);
		setNumRooms(10);
		init();


		Entity* player = entity_->getMngr()->getHandler<Player>();

		tmx::Vector2f pos = lvl->getPlayerPos();

		player->getComponent<BoxCollider>()->setPhysicalTransform(pos.x, pos.y, 0);


		travelZone = false;

	}

	if (stopFishing) {
		for (Entity* e : pesca) {
			e->setActive(false);
		}

		pesca.clear();
		stopFishing = false;
	}

}

void MapProcedural::TravelNextRoom(int dir) {
	//Nueva habitacion a la que hemos ido

	//actualRoom->level = "assets/maps/level_rooms0\\NSWtile6.tmx";
	//actualRoom->nameLevel = "NSWtile6.tmx";

	currentRoom->level = currentRoom->conections[dir].level;
	currentRoom->nameLevel = currentRoom->conections[dir].nameLevel;

	//Reseteamos las conexiones para la nueva habitacion
	for (bool& con : currentRoom->cons) con = false;

	//Hay que resetear tambien los datos de las conexiones
	for (Room& room : currentRoom->conections) room = {};


	//lvl->clearTileset();
	//Cargamos nuevo mapa
	lvl->load(currentRoom->level);

	//Setteamos los nuevos vertices para la creacion del cuerpo Collider
	entity_->removeComponent<MapCollider>();

	Entity* player = entity_->getMngr()->getHandler<Player>();

	//player->getComponent<BoxCollider>()->actPhyscialPos(-300,-30);

	for (Entity* ent : triggers) {
		ent->setActive(false);
		
	}
	for (Entity* ent : pesca) {
		ent->setActive(false);
	}
	triggers.clear();
	pesca.clear();

	chainCollider = entity_->addComponent<MapCollider>(lvl->getVerticesList(), GROUND, GROUND_MASK);

	//cout << actualRoom->getName();
	//Cogemos sus conexiones	
	getConec(currentRoom->nameLevel, currentRoom->cons);

	//Creamos habitaciones en funci�n de las conexiones que tiene
	CreateConnections(currentRoom, currentRoom->cons, dir);

	roomsExplored++;

	std::cout << roomsExplored << "\n";
}

void MapProcedural::setPlayer2spawn(){
	tmx::Vector2f playerpos = lvl->getPlayerPos();
	entity_->getMngr()->getHandler<Player>()->getComponent<BoxCollider>()->setPhysicalTransform(playerpos.x, playerpos.y, 0.0f);
}

void MapProcedural::loadLobby() {

	RoomNames lob;
	lob.name = "Etile0";
	lob.path = LOBBY;

	currentRoom = initializeNewRoom(lob);
	//No es más el lobby
	lobby = !lobby;
	//Cambiamos la musica
	entity_->getMngr()->getSoundMngr()->ChangeMainMusic("Lobby");
}

void MapProcedural::loadTileFiles(){
	//Leeemos los distintos directorios
	if (fase == 0) {
		int roomsRead = 0;
		ReadDirectory("assets/maps/level_starts0", roomsRead);
		areaLimits[0] = roomsRead; //Asertamos la frontera entre inicios y habiaciones normales
		ReadDirectory("assets/maps/level_rooms0", roomsRead);
		areaLimits[1] = roomsRead; //Asertamos la frontera entre habitaciones y finales
		ReadDirectory("assets/maps/level_ends0", roomsRead);
		lvl = entity_->getComponent<Level0>();

	}
	else if (fase == 1) {
		int roomsRead = 0;
		ReadDirectory("assets/maps/level_starts1", roomsRead);
		areaLimits[0] = roomsRead; //Asertamos la frontera entre inicios y habiaciones normales
		ReadDirectory("assets/maps/level_rooms1", roomsRead);
		areaLimits[1] = roomsRead; //Asertamos la frontera entre habitaciones y finales
		ReadDirectory("assets/maps/level_ends1", roomsRead);
	}
	else {
		int roomsRead = 0;
		ReadDirectory("assets/maps/level_starts2", roomsRead);
		areaLimits[0] = roomsRead; //Asertamos la frontera entre inicios y habiaciones normales
		ReadDirectory("assets/maps/level_rooms2", roomsRead);
		areaLimits[1] = roomsRead; //Asertamos la frontera entre habitaciones y finales
		ReadDirectory("assets/maps/level_ends2", roomsRead);
	}
}

void MapProcedural::travel(b2Contact* contact) {
 	Entity* trigger = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	//Si el contacto es el player, obtenemos el otro contacto que nos interesa, es decir, el trigger
	if (trigger == trigger->getMngr()->getHandler<Player>()) {
		trigger = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	}

	auto* m = trigger->getMngr()->getHandler<Map>();
	std::string d;
	//d= trigger->getComponent<Connections>()->getDir();
	auto* con = trigger->getComponent<Connections>();

	if (con != nullptr) d = con->getDir();

	int dir = -1;


	if (d == "N") dir = 0;
	else if (d == "E") dir = 1;
	else if (d == "S") dir = 2;
	else dir = 3;

	m->getComponent<MapProcedural>()->setTravel(true, dir);

	//trigger->getMngr()->getHandler<Map>()->getComponent<MapProcedural>()->stoppedFishing();
}

void MapProcedural::travelNextZone(b2Contact* contact) {
	Entity* trigger = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;

	if (trigger == trigger->getMngr()->getHandler<Player>()) {
		trigger = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	}

	int aux = trigger->getMngr()->getHandler<Map>()->getComponent<MapProcedural>()->getPhase();

	// Cambiamos de cancion cuando cambiamos de fase
	if (aux == 2){
		trigger->getMngr()->getSoundMngr()->ChangeMainMusic("Nivel2");
	}
	else if (aux == 3){
		trigger->getMngr()->getSoundMngr()->ChangeMainMusic("Nivel3");
	}

	auto* m = trigger->getMngr()->getHandler<Map>();

	m->getComponent<MapProcedural>()->travelNextZone();
}

void MapProcedural::pescar(b2Contact* contact) {
	Entity* trigger = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;

	if (trigger == trigger->getMngr()->getHandler<Player>()) {
		trigger = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	}


	trigger->getMngr()->getHandler<Map>()->getComponent<MapProcedural>()->getStates()->changeToPesca();
	trigger->getMngr()->getHandler<Map>()->getComponent<MapProcedural>()->stoppedFishing();
}

void MapProcedural::ReadDirectory(const string& p, int& roomsRead) {
	std::string path = p;

	for (const auto& entry : fs::directory_iterator(path)) {
		//Cogemos toda la ruta
		string ruta = entry.path().u8string();
		//string ruta = entry.path();

							//No usada //TipoStart
		RoomNames rN{"", ruta, false, 1};

		//Encontramos donde est� la divisi�n con el nombre
		int puntoCorte = entry.path().string().find_last_of("\\");

		ruta[puntoCorte] = '/';
		//Nombre real del nivel
		rN.name = entry.path().filename().string();

		roomNames.push_back(rN);
		roomsRead++;
	}
}

void MapProcedural::setTravel(const bool travel, int dir) {
	gonTotravel = travel;
	nextDir = dir;
}

Vector2D MapProcedural::getPlayerPos() {
	tmx::Vector2f s = lvl->getPlayerPos();

	Vector2D spawn(s.x, s.y);

	return spawn;
}

void MapProcedural::deleteTriggers(){
	if (!triggers.empty()) {
		for (Entity* ent : triggers) ent->setActive(false);
		triggers.clear();
	}
}

int MapProcedural::getRandomTileFromArea(Area a){
	if (a == Starts) return sdlutils().rand().teCuoto(0, areaLimits[(int)a]);
	else if (a == Mid) return sdlutils().rand().teCuoto(areaLimits[0], areaLimits[a]);
	else  return sdlutils().rand().teCuoto(areaLimits[a], roomNames.size());
}

bool MapProcedural::isZoneCompleted(){ return roomsExplored == nRooms; }

int MapProcedural::getPhase(){ return fase; }

void MapProcedural::travelNextZone() { travelZone = true; }

void MapProcedural::setPhase(int f) { fase = f; }

void MapProcedural::setNumRooms(int nR) { nRooms = nR; }



