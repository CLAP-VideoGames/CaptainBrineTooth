#include "MapProcedural.h"
#include "CameraFollow.h"
#include "../states/StateMachine.h"
#include "../states/PlayState.h"

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
	roomsExplored = 0;
	backgroundLayer = nullptr;

	std::cout << roomsExplored << std::endl;
	gonTotravel = travelZone = stopFishing = startRun_ = false;
	currentRoom = nullptr;
}

MapProcedural::~MapProcedural() {
	//Borramos los triggers
	deleteTriggers();

	delete currentRoom;
}

void MapProcedural::init() {

	PlayState* playState = dynamic_cast<PlayState*>(states->getStateMachine()->currentState());
	//Obtenemos el playStat para poder cambiar el fondo en cada nivel
	if (playState != nullptr)
		backgroundLayer = playState->getBackgroundLevel()->getComponent<ParallaxScroll>();

	//Setteamos el lvl
	lvl = entity_->getComponent<Level0>();
	//Cargamos todos los archivos
	loadTileFiles();
	//Cargamos e inicializamos los valores para el lobby
	loadLobby();
}

void MapProcedural::loadLobby() {
	//Cambiamos la musica
	entity_->getMngr()->getSoundMngr()->ChangeMainMusic("Lobby");

	lvl->load(LOBBY);

	if (!entity_->hasComponent<MapCollider>())
		chainCollider = entity_->addComponent<MapCollider>(lvl->getVerticesList(), GROUND, GROUND_MASK);

	createConnectionTriggers(-1, leaveLobby);
}

void MapProcedural::leaveLobby(b2Contact* contact) {
	Entity* trigger = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	//Si el contacto es el player, obtenemos el otro contacto que nos interesa, es decir, el trigger
	if (trigger == trigger->getMngr()->getHandler<Player>()) {
		trigger = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	}

	auto* m = trigger->getMngr()->getHandler<Map>();

	m->getComponent<MapProcedural>()->startRun(true);


}

void MapProcedural::getConec(const string& name, std::array<bool, 4>& cons) {
	for (int i = 0; i < 4; i++) {
		int card = checkMatch(name[i]);
		if (card != -1) cons[card] = true;
		else return;
	}
}

int MapProcedural::checkMatch(const char& ch) {
	if (ch == cardinals[(int)Cardinals::N]) return (int)Cardinals::N;
	else if (ch == cardinals[(int)Cardinals::E]) return (int)Cardinals::E;
	else if (ch == cardinals[(int)Cardinals::S]) return (int)Cardinals::S;
	else if (ch == cardinals[(int)Cardinals::W]) return (int)Cardinals::W;
	else return -1;
}

bool MapProcedural::matchConnections(int tileNum, Cardinals oppositeDir){
	int i = 0;
	while (i < 4 && roomNames[tileNum].name[i] != cardinals[(int)oppositeDir]) i++;
	return (i < 4);
}

void MapProcedural::initConnections(CurrentRoom* r) {
	getConec(r->nameLevel, r->cons);
	//Creamos habitaciones en funci�n de las conexiones que tiene
	setConnections(r, r->cons);
}

void MapProcedural::setConnections(CurrentRoom* r, const std::array<bool, 4> & rConnections) {
	for (int i = 0; i < 4; i++)
		if (rConnections[i])  r->conections[i] = initializeRoom(i);
}

Room MapProcedural::initializeRoom(int dir) {
	int tile = -1;
	bool concuerda = false;
	if (roomsExplored < 1){
		concuerda = true;
		tile = getRandomTileFromArea(Starts);
	}
	else if (roomsExplored == (nRooms - 1))
		tile = getRandomTileFromArea(Final);
	else 
		tile = sdlutils().rand().teCuoto(areaLimits[0], areaLimits[1] + 1); //Habitación intermedia
	
	Cardinals opositeDir = getOppositeDir((Cardinals)dir);

	if (tile != -1 && !concuerda)
		concuerda = matchConnections(tile, opositeDir);

	//Para que no se repitan hay que añadir la condicion ( || roomNames[tile].used) al bucle
	while (!concuerda) {
		if (roomsExplored == (nRooms - 1)) tile = getRandomTileFromArea(Final);
		else tile = getRandomTileFromArea(Mid);

		//Quite good this
		concuerda = matchConnections(tile, opositeDir);
	}

	roomNames[tile].used = true;

	return { sdlutils().getNameFilePath(roomNames[tile].path) , roomNames[tile].path };
}

void MapProcedural::createConnectionTriggers(int dir, CallBackCollision* method) {
	vector<tmx::Vector2f> positions = lvl->getConPos();	//Las posiciones de las conexiones
	vector<tmx::Vector2f> size = lvl->getConSize();	//Los tamaños de las conexiones
	vector<char> names = lvl->getConNames();

	Cardinals oppositeDir = getOppositeDir((Cardinals)dir);

	for (int i = 0; i < positions.size(); i++) {
		auto* t = entity_->getMngr()->addEntity();

		//Vector2D size;
		Vector2D pos(positions[i].x, positions[i].y);

		//Posicionamos al jugador en la salida opuesta a la que ha entrado
		if (dir != -1 && names[i] == cardinals[(int)oppositeDir]) {
			int x = pos.getX();
			int y = pos.getY();

			Entity* player = entity_->getMngr()->getHandler<Player>();
			playerCollider_ = player->getComponent<BoxCollider>();
			playerCollider_->setPhysicalTransform(x, y, 0);
			playerCollider_->setSpeed(Vector2D(0.0f, 0.0f));

			player->getComponent<CameraFollow>()->setCamToEntity();
		}
		else { //Creamos los colliders de los triggers la sala actual
			t->addComponent<Transform>(pos, Vector2D(0, 0), size[i].x, size[i].y, 0);

			t->addComponent<BoxCollider>(STATIC, PLAYER_DETECTION, PLAYER_DETECTION_MASK, true, 0, true, 0.0);

			t->addComponent<Connections>(names[i]);

			t->setCollisionMethod(method);

			triggers.push_back(t);
		}
	}
	tmx::Vector2f playerpos = lvl->getPlayerPos();

	auto* player = entity_->getMngr()->getHandler<Player>();
	//Si solo ha explorado la habitacion inicial (cuando se crea la habitacion inicial, roomExplored = 1)
	//Si es la sala inicial, posicionamos al player en el Spawn Player
	if (player != NULL && roomsExplored < 2 && playerpos.x != 0)
		player->getComponent<BoxCollider>()->setPhysicalTransform(playerpos.x, playerpos.y, 0);

	//Si es un final room, creamos el trigger que hace pasar de zona
	if (lvl->finalRoom()) {
		auto end = lvl->getEnd();

		auto* t = entity_->getMngr()->addEntity();

		Vector2D pos(end.x, end.y);

		t->addComponent<Transform>(pos, Vector2D(0, 0), 100, 100, 0);

		t->addComponent<BoxCollider>(STATIC, PLAYER_DETECTION, PLAYER_DETECTION_MASK, true, 0, true, 0.0);

		t->setCollisionMethod(travelNextZone);

		triggers.push_back(t);

	}

	vector<tmx::Vector2f> posAux = lvl->getPescaPoints();


	//Comprobamos que hay un punto de pesca
	if (posAux.size() != 0) {

		entity_->getMngr()->getSoundMngr()->ChangeMainMusic("CofreTesoro");
		for (int i = 0; i < posAux.size(); i++) {
			Vector2D pescaPos(posAux[i].x, posAux[i].y);

			auto* t = entity_->getMngr()->addEntity();

			t->addComponent<Transform>(pescaPos, Vector2D(0, 0), 50, 50, 0);

			t->addComponent<BoxCollider>(STATIC, PLAYER_DETECTION, PLAYER_DETECTION_MASK, true, 0, true, 0.0);

			t->setCollisionMethod(pescar);

			pesca.push_back(t);
		}
	}
	else if (entity_->getMngr()->getSoundMngr()->currentSong() == "CofreTesoro")
	{
		entity_->getMngr()->getSoundMngr()->ChangeMainMusic("Nivel" + std::to_string(fase));
	}
}

void MapProcedural::update() {

	if (startRun_) initRun();

	//Cambia de habitación
	if (gonTotravel) {
		TravelNextRoom(nextDir);
		nextDir = -1;
		gonTotravel = !gonTotravel;
	}

	//Cambia de zona
	if (travelZone) {
		//Se ha acabado el viaje de zona
		lvl->traveled();
		travelZone = false;
		//Nueva zona, habitaciones exploradas son cero
		roomsExplored = 0;
		//Nueva zona, fase + 1
		setPhase(fase + 1);
		//Numero de salas que habrá en el nivel
		setNumRooms(nRooms);
		//Borramos los triggers de la ultima sala
		deleteTriggers();
		//Borramos el currentRoom
		delete currentRoom;
		//Cargamos los nuevos archivos de la nueva zona
		loadTileFiles();
		//Cambiamos el fondo del nivel
		backgroundLayer->setLevelBackground(fase);
		//Empezamos la run
		if (fase < 3) initRun();
		else initBoss();
	}

	if (stopFishing) {
		for (Entity* e : pesca) {
			e->setActive(false);
		}

		playerCollider_->setSpeed(Vector2D(0, 0));
		pesca.clear(); 
		stopFishing = false;
	}
}

void MapProcedural::refreshCollider(){
	if (entity_->hasComponent<MapCollider>()) {
		entity_->removeComponent<MapCollider>();

		deleteTriggers();

		chainCollider = entity_->addComponent<MapCollider>(lvl->getVerticesList(), GROUND, GROUND_MASK);
	}
}

void MapProcedural::TravelNextRoom(int dir) {
	currentRoom->level = currentRoom->conections[dir].level;
	currentRoom->nameLevel = currentRoom->conections[dir].nameLevel;

	//Reseteamos las conexiones para la nueva habitacion
	for (bool& con : currentRoom->cons) con = false;
	//Hay que resetear tambien los datos de las conexiones
	for (Room& room : currentRoom->conections) room = {};

	//Cargamos nuevo mapa y renovamos el vector de vertices
	lvl->load(currentRoom->level);

	Entity* player = entity_->getMngr()->getHandler<Player>();

	if (player != nullptr)
		player->getComponent<CameraFollow>()->setCamMaxLimits(lvl->getMaxCoordenates());

	refreshCollider();

	for (Entity* ent : pesca) ent->setActive(false);
	pesca.clear();

	roomsExplored++;

	initConnections(currentRoom);

	createConnectionTriggers(dir, travel);
}

void MapProcedural::setPlayer2spawn(){
	tmx::Vector2f playerpos = lvl->getPlayerPos();
	entity_->getMngr()->getHandler<Player>()->getComponent<BoxCollider>()->setPhysicalTransform(playerpos.x, playerpos.y, 0.0f);
}

void MapProcedural::loadTileFiles(){

	if(!roomNames.empty()) roomNames.clear();
	//Leeemos los distintos directorios
	int roomsRead = 0;

	if(fase<3){
		string fase_ = std::to_string(fase);
		std::string starts = "assets/maps/level_starts" + fase_;
		std::string rooms = "assets/maps/level_rooms" + fase_;
		std::string ends = "assets/maps/level_ends" + fase_;
		ReadDirectory(starts, roomsRead);
		areaLimits[0] = roomsRead; //Asertamos la frontera entre inicios y habiaciones normales
		ReadDirectory(rooms, roomsRead);
		areaLimits[1] = roomsRead; //Asertamos la frontera entre habitaciones y finales
		ReadDirectory(ends, roomsRead);
	}
	else{
		std::string boss = "assets/maps/level_boss";
		ReadDirectory(boss, roomsRead);
	}
}

void MapProcedural::initBoss() {
	// Se inicia la musica del nivel correspondiente
	entity_->getMngr()->getSoundMngr()->ChangeMainMusic("FinalBoss");
	roomsExplored = 0;
	int tile = getRandomTileFromArea(Boss);
	roomNames[tile].used = true;
	roomsExplored++;
	currentRoom = initilizeCurrentRoom(roomNames[tile]);
}

void MapProcedural::initRun(){
	entity_->getMngr()->getSoundMngr()->ChangeMainMusic("Nivel" + std::to_string(fase));
	backgroundLayer->setLevelBackground(fase);
	startRun_ = false;
	int tile = getRandomTileFromArea(Starts);
	roomNames[tile].used = true;
	roomsExplored++;
	//std::cout << roomsExplored << std::endl;
	currentRoom = initilizeCurrentRoom(roomNames[tile]);
}

CurrentRoom* MapProcedural::initilizeCurrentRoom(const RoomNames& tag) {
	CurrentRoom* r = new CurrentRoom();
	r->level = tag.path;
	r->nameLevel = sdlutils().getNameFilePath(r->level);

	lvl->load(r->level);

	Entity* player = entity_->getMngr()->getHandler<Player>();

	if (player != nullptr){
		player->getComponent<CameraFollow>()->setCamMaxLimits(lvl->getMaxCoordenates());
		player->getComponent<CameraFollow>()->setCamToEntity(1);
	}

	refreshCollider();

	//No habrá más conexiones cuando haya llegado al nivel de boss
	if(fase < 3)
		initConnections(r);
	
	//Queremos que los triggers hagan viajar al player a otras habitaciones. Como se va a cagar la primera hab, dir es -1. No hay dir opuesta
	createConnectionTriggers(-1, travel);

	return r;
}

void MapProcedural::travelNextZone(b2Contact* contact) {
	Entity* trigger = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;

	if (trigger == trigger->getMngr()->getHandler<Player>()) {
		trigger = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	}

	int aux = trigger->getMngr()->getHandler<Map>()->getComponent<MapProcedural>()->getPhase();

	auto* m = trigger->getMngr()->getHandler<Map>();

	m->getComponent<MapProcedural>()->setTravelZone();
}

void MapProcedural::travel(b2Contact* contact) {
	Entity* trigger = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	//Si el contacto es el player, obtenemos el otro contacto que nos interesa, es decir, el trigger
	if (trigger == trigger->getMngr()->getHandler<Player>()) {
		trigger = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	}

	auto* map = trigger->getMngr()->getHandler<Map>();
	auto* con = trigger->getComponent<Connections>();

	char d;
	if (con != nullptr) d = (char)con->getDir();

	MapProcedural* mapP = map->getComponent<MapProcedural>();
	int dir = -1;
	dir = mapP->checkMatch(d);

	map->getComponent<MapProcedural>()->setTravel(true, dir);
}

void MapProcedural::pescar(b2Contact* contact) {
	Entity* trigger = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;

	if (trigger == trigger->getMngr()->getHandler<Player>()) {
		trigger = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	}


	playerCollider_ = trigger->getMngr()->getHandler<Player>()->getComponent<BoxCollider>();
	playerCollider_->setSpeed(Vector2D(0.0f, 0.0f));
	trigger->getMngr()->getHandler<Map>()->getComponent<MapProcedural>()->getStates()->changeToPesca();
	trigger->getMngr()->getHandler<Map>()->getComponent<MapProcedural>()->stoppedFishing();
}

void MapProcedural::ReadDirectory(const string& p, int& roomsRead) {
	std::string path = p;

	for (const auto& entry : fs::directory_iterator(path)) {
		string ruta = entry.path().u8string();//Cogemos toda la ruta

							//No usada //TipoStart
		RoomNames rN{"", ruta, false, 1};

		int puntoCorte = entry.path().string().find_last_of("\\"); //Encontramos donde est� la divisi�n con el nombre
		ruta[puntoCorte] = '/';
		
		rN.name = entry.path().filename().string(); //Nombre real del nivel

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

void MapProcedural::startRun(bool start){
	startRun_ = start;
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
	else if (a == Boss) return sdlutils().rand().teCuoto(0, roomNames.size());
	//else if (a == Boss) return sdlutils().rand().teCuoto(areaLimits[0], areaLimits[a]);
	else  return sdlutils().rand().teCuoto(areaLimits[1], roomNames.size());
}

Cardinals MapProcedural::getOppositeDir(Cardinals dir) {
	if (dir != Cardinals::None) {
		//Cogemos la posici�n opuesta
		int opositeDir = (int)dir + 2;

		//Si es 2 = "S", luego es 4, y el opuesto de S es N, luego es 0
		//Si es 3 = "W", luego es 5, y el opuesto de W es E, luego es 1
		if (opositeDir >= 4) opositeDir = opositeDir - 4;

		Cardinals opDr = (Cardinals)opositeDir;

		return  opDr;
	}
	else return Cardinals::None;
}

bool MapProcedural::isZoneCompleted(){ return roomsExplored == nRooms; }

int MapProcedural::getPhase(){ return fase; }

void MapProcedural::setTravelZone() { travelZone = true; }

void MapProcedural::setPhase(int f) { fase = f; }

void MapProcedural::setNumRooms(int nR) { nRooms = nR; }