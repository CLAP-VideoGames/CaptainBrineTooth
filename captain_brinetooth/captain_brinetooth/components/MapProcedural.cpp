#include "MapProcedural.h"

MapProcedural::MapProcedural(int nR, int f)
{
	nRooms = nR;
	lvl = nullptr;
	fase = f;
}

MapProcedural::~MapProcedural()
{
	delete actualRoom;
}

void MapProcedural::init() {
	//Leeemos los distintos directorios
	if (fase == 0) {
		int roomsRead = 0;
		ReadDirectory("assets/maps/level_starts0", roomsRead);
		fronteras[0] = roomsRead; //Asertamos la frontera entre inicios y habiaciones normales
		ReadDirectory("assets/maps/level_rooms0", roomsRead);
		fronteras[1] = roomsRead; //Asertamos la frontera entre habitaciones y finales
		ReadDirectory("assets/maps/level_ends0", roomsRead);
		lvl = entity_->getComponent<Level0>();

	}
	else if (fase == 1) {
		int roomsRead = 0;
		ReadDirectory("assets/maps/level_starts1", roomsRead);
		fronteras[0] = roomsRead; //Asertamos la frontera entre inicios y habiaciones normales
		ReadDirectory("assets/maps/level_rooms1", roomsRead);
		fronteras[1] = roomsRead; //Asertamos la frontera entre habitaciones y finales
		ReadDirectory("assets/maps/level_ends1", roomsRead);
	}
	else {
		int roomsRead = 0;
		ReadDirectory("assets/maps/level_starts2", roomsRead);
		fronteras[0] = roomsRead; //Asertamos la frontera entre inicios y habiaciones normales
		ReadDirectory("assets/maps/level_rooms2", roomsRead);
		fronteras[1] = roomsRead; //Asertamos la frontera entre habitaciones y finales
		ReadDirectory("assets/maps/level_ends2", roomsRead);
	}

	//Cacheamos el componente Level

	int tile = sdlutils().rand().teCuoto(0, fronteras[0]);
	//Testing
	//int tile = 0;

	actualRoom = initializeNewRoom(roomNames[tile]);

	roomNames[tile].used = true;
}

void MapProcedural::setTravel(const bool travel, int dir)
{
	gonTotravel = travel;
	nextDir = dir;
}

Vector2D MapProcedural::getPlayerPos()
{
	tmx::Vector2f s = lvl->getPlayerPos();

	Vector2D spawn(s.x, s.y);

	return spawn;
}

void MapProcedural::update()
{

	//Cambia de habitación
	if (gonTotravel) {
		TravelNextRoom(nextDir);
		nextDir = -1;
		gonTotravel = !gonTotravel;
	}

	//Cambia de zona
	if (travelZone) {
		lvl->traveled();

		entity_->removeComponent<MapCollider>();

		for (Entity* ent : triggers) ent->setActive(false);
		triggers.clear();

		chainCollider = entity_->addComponent<MapCollider>(lvl->getVerticesList(), STATIC, GROUND, GROUND_MASK);

		setFase(fase + 1);
		setNumRooms(10);
		init();


		Entity* player = entity_->getMngr()->getHandler<Player>();

		tmx::Vector2f pos = lvl->getPlayerPos();

		player->getComponent<BoxCollider>()->setPhysicalTransform(pos.x, pos.y, 0);

		roomsExplored = 0;

		travelZone = false;

	}
}

void MapProcedural::TravelNextRoom(int dir) {
	//Nueva habitaci�n a la que hemos ido
	actualRoom = actualRoom->conections[dir];
	//lvl->clearTileset();
	//Cargamos nuevo mapa
	lvl->load(actualRoom->level);
	//Setteamos los nuevos vertices para la creacion del cuerpo Collider

	entity_->removeComponent<MapCollider>();

	Entity* player = entity_->getMngr()->getHandler<Player>();

	//player->getComponent<BoxCollider>()->actPhyscialPos(-300,-30);

	for (Entity* ent : triggers) ent->setActive(false);
	triggers.clear();

	chainCollider = entity_->addComponent<MapCollider>(lvl->getVerticesList(), STATIC, GROUND, GROUND_MASK);

	//cout << actualRoom->getName();
	//Cogemos sus conexiones	
	getConec(actualRoom->getName(), actualRoom->cons);
	//Creamos habitaciones en funci�n de las conexiones que tiene
	CreateConnections(actualRoom, actualRoom->cons, dir);

	roomsExplored++;

	std::cout << roomsExplored << "\n";
}

bool MapProcedural::zoneCompleted()
{
	return roomsExplored == nRooms;
}

int MapProcedural::zone()
{
	return fase;
}

void MapProcedural::travelNextZone() {
	travelZone = true;
}

void MapProcedural::setFase(int f) { 
	fase = f; 
}

void MapProcedural::setNumRooms(int nR) { 
	nRooms = nR; 
}

void MapProcedural::travel(b2Contact* contact) {
	Entity* trigger = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;

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
}

void MapProcedural::travelNextZone(b2Contact* contact) {
	Entity* trigger = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;

	if (trigger == trigger->getMngr()->getHandler<Player>()) {
		trigger = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	}

	auto* m = trigger->getMngr()->getHandler<Map>();

	m->getComponent<MapProcedural>()->travelNextZone();
}

void MapProcedural::pescar(b2Contact* contact) {
	Entity* trigger = (Entity*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;

	if (trigger == trigger->getMngr()->getHandler<Player>()) {
		trigger = (Entity*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;
	}
}

void MapProcedural::ReadDirectory(const string& p, int& roomsRead) {
	std::string path = p;

	for (const auto& entry : fs::directory_iterator(path)) {
		//Cogemos toda la ruta
		string ruta = entry.path().u8string();
		//string ruta = entry.path();

		RoomNames rN;

		rN.path = ruta; //Se la asignamos al path
		rN.used = false;	//No se ha usado la habitaci�n
		rN.tipo = 1;		//Tipo start

		//Encontramos donde est� la divisi�n con el nombre
		int puntoCorte = entry.path().string().find_last_of("\\");

		ruta[puntoCorte] = '/';
		//Nombre real del nivel
		rN.name = entry.path().filename().string();

		roomNames.push_back(rN);
		roomsRead++;
	}
}

Room* MapProcedural::initializeNewRoom(const RoomNames& tag) {
	cout << tag.name;
	Room* r = new Room();
	/*std::string test = "assets/maps/level_rooms/NSWtile6.tmx";
	lvl->load(test);*/
	r->level = tag.path;
	lvl->load(r->level);

	chainCollider = entity_->addComponent<MapCollider>(lvl->getVerticesList(), GROUND, GROUND_MASK);

	//Opcion 1
	getConec(tag.name, r->cons);


	CreateConnections(r, r->cons, -1);

	return r;
}

void MapProcedural::CreateConnections(Room* r, const std::array<bool, 4>& rConnections, int dir) {
	for (int i = 0; i < 4; i++) {
		if (rConnections[i] == true) r->conections[i] = initializeRoom(r, i);

	}
	createConnectionTriggers(dir);
}

Room* MapProcedural::initializeRoom(Room* partida, int dir) {
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

		if (roomsExplored == nRooms - 1)
			tile = sdlutils().rand().teCuoto(fronteras[1], roomNames.size());
		else
			tile = sdlutils().rand().teCuoto(fronteras[0], fronteras[1]);

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

void MapProcedural::getConec(const string& name, std::array<bool, 4>& cons) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			if (name[i] == cardinals[j]) cons[j] = true;
	}
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
			//Dos sumas completamente arbitarias que me sirven
			if (oppDir == "S")
				entity_->getMngr()->getHandler<Player>()->getComponent<BoxCollider>()->setPhysicalTransform(pos.getX() + (size[i].x / 2), pos.getY() + (size[i].y / 2), 0);
			else if (oppDir == "W")
				entity_->getMngr()->getHandler<Player>()->getComponent<BoxCollider>()->setPhysicalTransform(pos.getX() + (size[i].x / 2), pos.getY() + (size[i].y / 2), 0);
			else entity_->getMngr()->getHandler<Player>()->getComponent<BoxCollider>()->setPhysicalTransform(pos.getX() + (size[i].x / 2), pos.getY() + (size[i].y / 2), 0);
		}
		else {
			t->addComponent<Transform>(pos, Vector2D(0, 0), size[i].x, size[i].y, 0);

			t->addComponent<BoxCollider>(STATIC, PLAYER_DETECTION, PLAYER_DETECTION_MASK, true, 0, true, 0.0);

			t->addComponent<Connections>(names[i]);

			t->setCollisionMethod(travel);

			triggers.push_back(t);
		}


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

	tmx::Vector2f posAux = lvl->getPescaPoint();


	Vector2D pescaPos(posAux.x, posAux.y);
	//Comprobamos que hay un punto de pesca
	if (pescaPos.getX() != 0) {
		auto* t = entity_->getMngr()->addEntity();

		t->addComponent<Transform>(pescaPos, Vector2D(0, 0), 200, 200, 0);

		t->addComponent<BoxCollider>(STATIC, PLAYER_DETECTION, PLAYER_DETECTION_MASK, true, 0, true, 0.0);

		t->setCollisionMethod(pescar);
	}
}
