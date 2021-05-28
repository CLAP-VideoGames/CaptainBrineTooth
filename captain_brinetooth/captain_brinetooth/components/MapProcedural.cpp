#include "MapProcedural.h"
#include "CameraFollow.h"
#include "Inventory.h"
#include "../states/StateMachine.h"
#include "../states/SkillTreeState.h"
#include "../states/PlayState.h"
#include "../states/GameState.h"
#include "../states/StateMachine.h"

#define _CRTDBG_MAP_ALLOC
#include<iostream>
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

MapProcedural::MapProcedural(int nR, int f, App* s) {
	nRooms = nR;
	lvl = nullptr;
	fase = f;
	app = s;
	roomsExplored = 0;
	backgroundLayer = nullptr;

	accessButton = nullptr;
	scarceBait = nullptr;
	accessB_Size = {};
	player = nullptr;
	timeBaitError = 1500;
	lastTime = 0;

	std::cout << roomsExplored << std::endl;
	gonTotravel = travelZone = stopFishing = startRun_ = canAccess = scarceBaitError = false;
	currentRoom = nullptr;
}

MapProcedural::~MapProcedural() {
	//Borramos los triggers
	deleteTriggers();

	delete currentRoom;
}

void MapProcedural::init() {
	//FeedBack Initialization
	accessButton = &sdlutils().images().at("player_controls");
	scarceBait = &sdlutils().msgs().at("scarceBaits");
	lastTime = sdlutils().currRealTime();
	accessB_Size = Vector2D(accessButton->width(), accessButton->height());

	//Run Initialization
	PlayState* playState = dynamic_cast<PlayState*>(app->getStateMachine()->currentState());
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

	isLobby = true;

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

void MapProcedural::onEnterAccessTrigger(b2Contact* contact) {
	Entity* trigger = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	//Si el contacto es el player, obtenemos el otro contacto que nos interesa, es decir, el trigger
	if (trigger == trigger->getMngr()->getHandler<Player>()) {
		trigger = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	}

	if (trigger->isActive()) {
		Entity* m = nullptr;
		m = trigger->getMngr()->getHandler<Map>();
		if (m != nullptr && m->componentsSize() > 0) {
			MapProcedural* map = nullptr; 
			if (m->hasComponent<MapProcedural>()) {
				map = m->getComponent<MapProcedural>();
				if (map != nullptr)
					map->pressToAccess(true);
			}
		}
	}
}

void MapProcedural::onExitAccessTrigger(b2Contact* contact) {
	Entity* trigger = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
	//Si el contacto es el player, obtenemos el otro contacto que nos interesa, es decir, el trigger
	if (trigger == trigger->getMngr()->getHandler<Player>()) {
		trigger = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	}

	if (trigger->isActive()){
		if (trigger->componentsSize()>0){
			Entity* m = nullptr;
			m = trigger->getMngr()->getHandler<Map>();
			if (m != nullptr && m->componentsSize() > 0) {
				MapProcedural* map = nullptr;
				if (m->hasComponent<MapProcedural>()) {
					map = m->getComponent<MapProcedural>();
					if (map != nullptr)
						map->pressToAccess(false);
				}
			}
		}
	}
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

bool MapProcedural::matchConnections(int tileNum, Cardinals oppositeDir) {
	int i = 0;
	while (i < 4 && roomNames[tileNum].name[i] != cardinals[(int)oppositeDir]) i++;
	return (i < 4);
}

void MapProcedural::initConnections(CurrentRoom* r) {
	getConec(r->nameLevel, r->cons);
	//Creamos habitaciones en funci�n de las conexiones que tiene
	setConnections(r, r->cons);
}

void MapProcedural::setConnections(CurrentRoom* r, const std::array<bool, 4>& rConnections) {
	for (int i = 0; i < 4; i++)
		if (rConnections[i])  r->conections[i] = initializeRoom(i);
}

Room MapProcedural::initializeRoom(int dir) {
	int tile = -1;
	bool concuerda = false;
	if (roomsExplored < 1) {
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
	tmx::Vector2f storePos;
	if (isLobby) {
		storePos = lvl->getStorePos();
		//Generar trigger para que se pueda entrar en el arbol de habilidades
		auto* t = entity_->getMngr()->addEntity();

		Vector2D pos = Vector2D(storePos.x, storePos.y);

		t->addComponent<Transform>(pos, Vector2D(0, 0), 112, 120, 0);

		t->addComponent<BoxCollider>(STATIC, PLAYER_DETECTION, PLAYER_DETECTION_MASK, true, 0, true, 0.0);
		t->addComponent<Animation>("skill", &sdlutils().images().at("skillTableExterior"),1,1,1,1,0, Vector2D(0.5, 0.5));

		t->setCollisionMethod(onEnterAccessTrigger);

		t->setEndCollisionMethod(onExitAccessTrigger);

		triggers.push_back(t);
	}

	Cardinals oppositeDir = getOppositeDir((Cardinals)dir);

	for (int i = 0; i < positions.size(); i++) {
		auto* t = entity_->getMngr()->addEntity();

		//Vector2D size;
		Vector2D pos(positions[i].x, positions[i].y);


		t->addComponent<Transform>(pos, Vector2D(0, 0), size[i].x, size[i].y, 0);
		//Posicionamos al jugador en la salida opuesta a la que ha entrado
		if (dir != -1 && names[i] == cardinals[(int)oppositeDir]) {
			int x = pos.getX();
			int y = pos.getY();

			player = entity_->getMngr()->getHandler<Player>();
			playerCollider_ = player->getComponent<BoxCollider>();
			playerCollider_->setPhysicalTransform(x, y, 0);
			playerCollider_->setSpeed(Vector2D(0.0f, 0.0f));

			player->getComponent<CameraFollow>()->setCamToEntity();


			if(dir == (int)Cardinals::N || dir == (int)Cardinals::W) t->addComponent<Animation>("Grid", &sdlutils().images().at("grid"), 1, 1, 1, 1, 0);
			else t->addComponent<Animation>("Grid", &sdlutils().images().at("verticalGrid"), 1, 1, 1, 1, 0);

		}
		else { //Creamos los colliders de los triggers la sala actual
			//t->addComponent<Transform>(pos, Vector2D(0, 0), size[i].x, size[i].y, 0);

			t->addComponent<BoxCollider>(STATIC, PLAYER_DETECTION, PLAYER_DETECTION_MASK, true, 0, true, 0.0);

			t->addComponent<Animation>("mist", &sdlutils().images().at("mist"), 4, 5, 20, 12, -1, Vector2D(0.5, 0.5));

			t->addComponent<Connections>(names[i]);

			t->setCollisionMethod(method);
		}
			triggers.push_back(t);
	}
	tmx::Vector2f playerpos = lvl->getPlayerPos();

	player = entity_->getMngr()->getHandler<Player>();
	//Si solo ha explorado la habitacion inicial (cuando se crea la habitacion inicial, roomExplored = 1)
	//Si es la sala inicial, posicionamos al player en el Spawn Player
	if (player != nullptr && roomsExplored < 2 && playerpos.x != 0)
		player->getComponent<BoxCollider>()->setPhysicalTransform(playerpos.x, playerpos.y, 0);

	//Si es un final room, creamos el trigger que hace pasar de zona
	if (lvl->finalRoom()) {
		auto end = lvl->getEnd();

		auto* t = entity_->getMngr()->addEntity();

		Vector2D pos(end.x, end.y);

		t->addComponent<Transform>(Vector2D(pos.getX()+35, pos.getY()-75), Vector2D(0, 0), 100, 150, 0);

		if (fase == 0) {
			t->addComponent<Animation>("portal", &sdlutils().images().at("aztec_portal"), 2, 2, 4, 8, -1);
			t->getComponent<Animation>()->setAlpha(208);
		}
		else if (fase == 1) {
			t->addComponent<Animation>("portal", &sdlutils().images().at("ice_portal"), 2, 2, 4, 8, -1);
			t->getComponent<Animation>()->setAlpha(208);
		}

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

			t->addComponent<Transform>(Vector2D(pescaPos.getX(), pescaPos.getY()-75), Vector2D(0, 0), 75, 75, 0);

			t->addComponent<Animation>("rod", &sdlutils().images().at("fishing_rod"), 2, 3, 6, 4, -1);

			t->addComponent<BoxCollider>(STATIC, PLAYER_DETECTION, PLAYER_DETECTION_MASK, true, 0, true, 0.0);

			t->setCollisionMethod(onEnterAccessTrigger);

			t->setEndCollisionMethod(onExitAccessTrigger);

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

		pesca.clear();
		stopFishing = false;
		//Se cancela a veces la llamada de collisionExit de la pesca, y necesita resetearse
		scarceBaitError = false;
		canAccess = false;
	}

	if (ih().keyDownEvent()) {
		if (canAccess && ih().isKeyDown(SDL_SCANCODE_E)) {
			if (isLobby){
				app->getStateMachine()->pushState(new SkillTreeState(app->getStateMachine()->currentState(), app, entity_->getWorld(), entity_->getMngr()->getSoundMngr(), player));
			}
			else{
				playerCollider_ = player->getComponent<BoxCollider>();
				playerCollider_->setSpeed(Vector2D(0.0f, 0.0f));

				int baits_ = player->getComponent<Inventory>()->getBaits();

				if (baits_ > 0) {
					app->changeToPesca();
					stoppedFishing();
				}
				else{
					scarceBaitError = true;
					lastTime = sdlutils().currRealTime();
				}
			}
		}
	}

	if (scarceBaitError){
		if (sdlutils().currRealTime() - lastTime > timeBaitError){
			scarceBaitError = false;
		}
	}
}

void MapProcedural::render() {
	if (canAccess) {
		player = entity_->getMngr()->getHandler<Player>();
		//Renderizamos el botton para que acceder a acciones
		//c -> 4 | r -> 2 
		Vector2D playerPos;
		Transform* tr_ = nullptr;
		if (player != nullptr) {
			tr_ = player->getComponent<Transform>();
			playerPos = tr_->getPos();
			playerPos.setX(playerPos.getX());
			playerPos.setY(playerPos.getY() + (tr_->getH()/2));
		}

		float w = accessB_Size.getX() / (4 * 3.5);
		float h = accessB_Size.getY() / (3 * 3.5);
		//Vector2D destPos(App::camera.w / 2 - (w/2), App::camera.h / 2/*playerPos.getY()*//* - (h / 2)*/);
		Vector2D destPos(playerPos.getX() - (w / 2) - (App::camera.x), playerPos.getY() - (App::camera.y) -(tr_->getH() * 1.25 + h));
		SDL_Rect dest = build_sdlrect(destPos, w , h);

		Vector2D pos(0, 2 * (float)(accessButton->height() / 3));
		SDL_Rect src = build_sdlrect(pos, ((float)accessButton->width() / 4), ((float)accessButton->height() / 3));
		accessButton->render(src, dest, 0.0f);

		//En caso de no haber suficientes anzuelos, sale un mensaje de advertencia al jugador durante x segundos
		if (scarceBaitError){
			float w = scarceBait->width()*0.5;
			float h = scarceBait->height()*0.5;
			//Se pone debajo del boton de la tecla E
			destPos.setY(destPos.getY() - dest.h);

			destPos.setX(playerPos.getX() - (w / 2) - (App::camera.x));
			dest = build_sdlrect(destPos, w, h);
			scarceBait->render(dest);
		}
	}
}

void MapProcedural::refreshCollider() {
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

void MapProcedural::setPlayer2spawn() {
	tmx::Vector2f playerpos = lvl->getPlayerPos();
	entity_->getMngr()->getHandler<Player>()->getComponent<BoxCollider>()->setPhysicalTransform(playerpos.x, playerpos.y, 0.0f);

	//Guardado
	app->getStateMachine()->currentState()->saveGame();
}

void MapProcedural::loadTileFiles() {

	if (!roomNames.empty()) roomNames.clear();
	//Leeemos los distintos directorios
	int roomsRead = 0;

	if (fase < 3) {
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
	else {
		std::string boss = "assets/maps/level_boss";
		ReadDirectory(boss, roomsRead);
	}
}

void MapProcedural::pressToAccess(bool entered) {
	if (this != nullptr){
		canAccess = entered;
		if (scarceBaitError) scarceBaitError = false;
	}
}

void MapProcedural::initBoss() {
	auto* p = entity_->getMngr()->getHandler<Player>();
	SkillTree* sT = nullptr;
	if (p) sT = p->getComponent<SkillTree>();
	if(sT) sT->setCounterAttackPercentage(0.03f);
	// Se inicia la musica del nivel correspondiente
	entity_->getMngr()->getSoundMngr()->ChangeMainMusic("FinalBoss");
	roomsExplored = 0;
	int tile = getRandomTileFromArea(Boss);
	roomNames[tile].used = true;
	roomsExplored++;
	currentRoom = initilizeCurrentRoom(roomNames[tile]);
}

void MapProcedural::initRun() {
	entity_->getMngr()->getSoundMngr()->ChangeMainMusic("Nivel" + std::to_string(fase));
	backgroundLayer->setLevelBackground(fase);
	int tile = getRandomTileFromArea(Starts);
	roomNames[tile].used = true;
	roomsExplored++;
	//std::cout << roomsExplored << std::endl;
	currentRoom = initilizeCurrentRoom(roomNames[tile]);
	//Tiene que estar aqui debajo para que el createTrigger verifique que se ha pasado a la run
	startRun_ = false;
}

CurrentRoom* MapProcedural::initilizeCurrentRoom(const RoomNames& tag) {
	CurrentRoom* r = new CurrentRoom();
	r->level = tag.path;
	r->nameLevel = sdlutils().getNameFilePath(r->level);

	lvl->load(r->level);

	Entity* player = entity_->getMngr()->getHandler<Player>();

	if (player != nullptr) {
		player->getComponent<CameraFollow>()->setCamMaxLimits(lvl->getMaxCoordenates());
		player->getComponent<CameraFollow>()->setCamToEntity(1);
	}

	refreshCollider();

	//No habrá más conexiones cuando haya llegado al nivel de boss
	if (fase < 3)
		initConnections(r);

	//Queremos que los triggers hagan viajar al player a otras habitaciones. Como se va a cagar la primera hab, dir es -1. No hay dir opuesta
	createConnectionTriggers(-1, travel);

	//Guardado automatico
	app->getStateMachine()->currentState()->saveGame();

	return r;
}

void MapProcedural::travelNextZone(b2Contact* contact) {
	Entity* trigger = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;

	if (trigger == trigger->getMngr()->getHandler<Player>()) {
		trigger = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	}

	int aux = trigger->getMngr()->getHandler<Map>()->getComponent<MapProcedural>()->getPhase();

	auto* m = trigger->getMngr()->getHandler<Map>();

	MapProcedural* mapP = m->getComponent<MapProcedural>();
	m->getComponent<MapProcedural>()->setTravelZone();
	//Guardado automatico
	mapP->app->getStateMachine()->currentState()->saveGame();
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

	//Guardado automatico
	mapP->app->getStateMachine()->currentState()->saveGame();
}

void MapProcedural::ReadDirectory(const string& p, int& roomsRead) {
	std::string path = p;

	for (const auto& entry : fs::directory_iterator(path)) {
		string ruta = entry.path().u8string();//Cogemos toda la ruta

							//No usada //TipoStart
		RoomNames rN{ "", ruta, false, 1 };

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

void MapProcedural::startRun(bool start) {
	startRun_ = start;
	isLobby = false;
}

void MapProcedural::deleteTriggers() {
	if (!triggers.empty()) {
		for (Entity* ent : triggers) ent->setActive(false);
		triggers.clear();
	}
}

int MapProcedural::getRandomTileFromArea(Area a) {
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

bool MapProcedural::isZoneCompleted() { return roomsExplored == nRooms; }

int MapProcedural::getPhase() { return fase; }

void MapProcedural::setTravelZone() { travelZone = true; }

void MapProcedural::setPhase(int f) { fase = f; }

void MapProcedural::setNumRooms(int nR) { nRooms = nR; }