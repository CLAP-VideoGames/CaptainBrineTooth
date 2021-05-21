#include "PescaState.h"
#include "../components/Inventory.h"
#include "../sdlutils/InputHandler.h"
using namespace ColLayers;

PescaState::PescaState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd, Entity* p, std::shared_ptr <b2World> maux) : GameState(a, mundo, snd)
{
	playerRef = p;   //Referencia al jugador par obtener su inventario
	playWorld = maux; //Mundo auxiliar creado para la pesca 
	//Para que el jugador despues de este estado aparezca parado
	p->getComponent<PlayerController>()->setMoveRight(false); 
	p->getComponent<PlayerController>()->setMoveLeft(false);
}
PescaState::~PescaState() {
	app->setCameraZoomOut(main_zoom);
};


void PescaState::update() {
	manager_->getWorld()->Step(1.0f / 60.0f, 6, 2);
	if (ih().keyDownEvent()) {
		if (ih().isKeyDown(SDL_SCANCODE_SPACE) && !space_pressed_) {
			space_pressed_ = true;
			s->setActive(false);
			s = nullptr;
			m3->setActive(false);
			m3 = nullptr;
		}
	}
	GameState::update();
}

void PescaState::init() {
	//-----Map-----
#pragma region Map
	//Metodo para cambiar el zoom 
	main_zoom = app->getCameraZoomOut();
	app->setCameraZoomOut(2.0f);
	space_pressed_ = false;


	//Ancho y alto de la ventana para situar mejor los sprites
	screen_width = sdlutils().width() * App::camera_Zoom_Out;
	screen_heigth = sdlutils().height() * App::camera_Zoom_Out;

	w_reward = (int)60 * App::camera_Zoom_Out;
	h_reward = (int)40 * App::camera_Zoom_Out;    //mantener aspect ratio
	int y = (int)(screen_heigth * 0.5 - h_reward);	//Offset hacia abajo con respecto al jugador

	//Fill array
	for (int i = 0; i < rows_; i++)
	{
		y = (int)(y + h_reward * 2);	//establece la posicion y de la columna, solo hay que cambiar el numero
		rowHeights.push_back(y);
	}

	auto* bg = createBasicEntity(Vector2D(screen_width / 2, screen_heigth / 2), Vector2D(screen_width, screen_heigth), 0.0f, Vector2D(0, 0));
	bg->addComponent<Animation>("1", &sdlutils().images().at("fondoPesca"), 1, 10, 10, 3, -1);
	//---------
	entitiesPerLine = 1;
	totalBasura = sdlutils().rand().teCuoto(3, 9); //AHora la basura tiene un rango en cuanto a cantidad de aparicion 

	Config gancho{};
	gancho.pos = Vector2D(screen_width / 2.0, screen_heigth * 0.22);
	gancho.vel = Vector2D(0, 0);
	gancho.size = Vector2D(50.0 * App::camera_Zoom_Out, 50.0 * App::camera_Zoom_Out);
	gancho.friction = 0.0f;
	gancho.physicType = DYNAMIC;
	gancho.fixedRotation = true;
	gancho.rotation = 0.0f;
	gancho.col = DEFAULT;
	gancho.colMask = DEFAULT_MASK;
	createPesca(gancho);
	//Imagenes
	auto* a = manager_->addEntity();
	Vector2D aPos = Vector2D(sdlutils().width() * 0.1f * App::camera_Zoom_Out, sdlutils().height() * 0.1f * App::camera_Zoom_Out);
	a->addComponent<Transform>(aPos, Vector2D(0, 0), 50.0f * App::camera_Zoom_Out, 50.0f * App::camera_Zoom_Out, 0.0f);
	a->addComponent<Animation>("p", &sdlutils().images().at("player_controls"), 2, 4, 8, 1, 0, 0, 0);

	auto* d = manager_->addEntity();
	Vector2D dPos = Vector2D(sdlutils().width() * 0.9f * App::camera_Zoom_Out, sdlutils().height() * 0.1f * App::camera_Zoom_Out);
	d->addComponent<Transform>(dPos, Vector2D(0, 0), 50.0f * App::camera_Zoom_Out, 50.0f * App::camera_Zoom_Out, 0.0f);
	d->addComponent<Animation>("p", &sdlutils().images().at("player_controls"), 2, 4, 8, 1, 0, 1, 1);

	s = manager_->addEntity();
	Vector2D sPos = Vector2D(sdlutils().width() * 0.5f * App::camera_Zoom_Out, sdlutils().height() * 0.4f * App::camera_Zoom_Out);
	s->addComponent<Transform>(sPos, Vector2D(0, 0), 180.0f * App::camera_Zoom_Out, 120.0f * App::camera_Zoom_Out, 0.0f);
	s->addComponent<Animation>("p", &sdlutils().images().at("player_controls"), 2, 4, 8, 1, 0, 5, 5);
	//Messages
	int x = (int)(aPos.getX());
	y = (int)(aPos.getY() + a->getComponent<Transform>()->getH());
	auto* m = manager_->addEntity();
	m->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), 75.0f * App::camera_Zoom_Out, 10.0f * App::camera_Zoom_Out, 0.0f);
	m->addComponent<Animation>("a", &sdlutils().msgs().at("izquierda"), 1, 1, 1, 1, 0);

	x = (int)(dPos.getX());
	y = (int)(dPos.getY() + d->getComponent<Transform>()->getH());
	auto* m2 = manager_->addEntity();
	m2->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), 75.0f * App::camera_Zoom_Out, 10.0f * App::camera_Zoom_Out, 0.0f);
	m2->addComponent<Animation>("d", &sdlutils().msgs().at("derecha"), 1, 1, 1, 1, 0);

	x = (int)(sPos.getX());
	y = (int)(sPos.getY() + s->getComponent<Transform>()->getH() * 0.5);
	m3 = manager_->addEntity();
	m3->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), 180.0f * App::camera_Zoom_Out, 30.0f * App::camera_Zoom_Out, 0.0f);
	m3->addComponent<Animation>("a", &sdlutils().msgs().at("fishing"), 1, 1, 1, 1, 0);
}
void PescaState::createPesca(const Config& entityConfig) {

	float z_factor = App::camera_Zoom_Out;	//Si quereis que el tamano sea con un numero multiplicadlo por esto

	//Suelo *arreglado
	auto* floor = createBasicEntity(Vector2D(entityConfig.pos.getX(), screen_heigth * 0.95), Vector2D(screen_width * 0.8, screen_heigth * 0.1), 0.0f, Vector2D(0, 0));
	//floor->addComponent<Animation>("debug", &sdlutils().images().at("arena"), 1, 1, 1, 1, 0);
	floor->addComponent<BoxCollider>(KINEMATIC, DEFAULT, DEFAULT_MASK);
	//cuerda *arreglada
	auto* topRod = createBasicEntity(entityConfig.pos + Vector2D(0, -entityConfig.size.getY() - 5 * z_factor), Vector2D(entityConfig.size.getX() * 4, entityConfig.size.getX()), 0.0f, Vector2D(0, 0));
	//topRod->addComponent<Animation>("debug", &sdlutils().images().at("debug_square"), 1, 1, 1, 1, 0);
	topRod->addComponent<BoxCollider>(DYNAMIC, DEFAULT, DEFAULT_MASK);
	topRod->addComponent<PescaController>(screen_width);
	topRod->getMngr()->setHandler<Rod>(topRod); //Para deteccion de colision con el gancho 

	//player *arreglado
	auto* player = createBasicEntity(entityConfig.pos + Vector2D(60 * z_factor, 5 * z_factor), Vector2D(128.0 * z_factor, 140.0 * z_factor), 0.0f, Vector2D(0, 0));
	player->addComponent<Animation>("player", &sdlutils().images().at("player_cana"), 1, 1, 1, 1, 0);
	BoxCollider* playercollider_ = player->addComponent<BoxCollider>(DYNAMIC, DEFAULT, DEFAULT_MASK);
	playercollider_->getFixture()->SetSensor(true);
	player->addComponent<PescaController>(screen_width);
	//gancho *si arreglado
	auto* gancho = createBasicEntity(Vector2D(entityConfig.pos.getX() + 5, entityConfig.pos.getY()), entityConfig.size, entityConfig.rotation, entityConfig.vel);
	gancho->addComponent<Animation>("idle", &sdlutils().images().at("fullvida"), 1, 8, 8, 8, -1);
	gancho->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask, false, 0.7f, true, 0.0f, Vector2D(entityConfig.size.getX() * 0.5, entityConfig.size.getY() * 0.5));
	gancho->addComponent<Gancho>(app);
	gancho->addComponent<PescaController>(screen_width);

	//Rewards 
	createRandomReward(entityConfig);
	crearBasura();
	//cuerda *no arreglada
	auto* cuerda = createBasicEntity(entityConfig.pos + Vector2D(15 * z_factor, -30 * z_factor), Vector2D(7.0f * z_factor, 16.0f * z_factor), 0.0f, Vector2D(0, 0));
	AnimBlendGraph* cuerda_anim_controller = cuerda->addComponent<AnimBlendGraph>();
	cuerda_anim_controller->addAnimation("cuerda", &sdlutils().images().at("cuerda"), 1, 1, 1, 1, 0);
	cuerda->addComponent<BoxCollider>(DYNAMIC, DEFAULT, DEFAULT_MASK);
	cuerda->addComponent<Cuerda>(gancho);
	cuerda->addComponent<PescaController>(screen_width);

}
void PescaState::createRandomReward(const Config& entityConfig)
{
	//0 espada 1 martillo 2 sierra ---> se iran a�adiendo segun vaya habiendo mas armas



	//MIRAR PARA HACER LAS PROBABILIDADES 
	//Filas: solo pueden salir las que estan puestas
	//Niveles: salen con mas probabilidad esas armas 

	//En cada fila solo se pone lo que tienes aqui
	// fila  1  Espada, Escupetintas
	// fila  2  Sierra, Puños Cangrejo
	// fila  3  Ametralladora, Martillo

	//En los nieveles puede salir en cada uno todas las armas pero en funcion del nivel se ve cuales armas son mas probable
	//Nivel 1  Espada, Escupetintas
	//Nivel 2  Sierra, Puños Cangrejo
	//Nivel 3  Ametralladora, Martillo

	//1 fila 
	//2 fila todas menos el martillo


	float offset = 10;
	float speedperlevel = 2;
	int x = sdlutils().width() * App::camera_Zoom_Out;
	int random;
	int probabInLevel;

	int entitiesaux = entitiesPerLine;
	Inventory* playerInv = playerRef->getComponent<Inventory>();
	int lastRandom = 90; //Inicialmente no hay ultimo arma generada asi que ponemos un numero grande para que el bucle no se confunda haciendo comprobaciones 

	for (int i = 0; i < rows_; i++) {
		for (int j = 0; j < entitiesaux; j++)
		{
			random = sdlutils().rand().teCuoto(0, 6);

			//Si vuelve a salir el mismo arma que antes volvemos a generar otra 
			while (random == lastRandom)
			{
				random = sdlutils().rand().teCuoto(0, 6);
			}
			auto* reward0 = createBasicEntity(Vector2D(x + (400 * App::camera_Zoom_Out * j) + (75 * App::camera_Zoom_Out * i), rowHeights[i]), Vector2D(w_reward, h_reward), 0.0f, Vector2D(0, 0));


			//If player has not the weapon we try adding another weapon that player hasn�t
			while (playerInv->hasWeapon(random))
			{
				random = sdlutils().rand().teCuoto(0, 6);
			}
			if (random == 0)
			{
				reward0->addComponent<Animation>("idle", &sdlutils().images().at("espada"), 1, 1, 1, 1, 0);
			}
			else if (random == 1) {
				reward0->addComponent<Animation>("idle", &sdlutils().images().at("martillo"), 1, 1, 1, 1, 0);
			}
			else if (random == 2) {
				reward0->addComponent<Animation>("idle", &sdlutils().images().at("sierra"), 1, 1, 1, 1, 0);
			}
			else if (random == 3) {
				reward0->addComponent<Animation>("idle", &sdlutils().images().at("crab"), 1, 1, 1, 1, 0);
			}
			else if (random == 4) {
				reward0->addComponent<Animation>("idle", &sdlutils().images().at("machine_gun"), 1, 1, 1, 1, 0);
			}
			else if (random == 5) {
				reward0->addComponent<Animation>("idle", &sdlutils().images().at("escupetintas"), 1, 1, 1, 1, 0);
			}


			reward0->addComponent<BoxCollider>(DYNAMIC, CEBO_GANCHO, CEBO_GANCHO_MASK);
			reward0->addComponent<Reward>(random, playerRef, app, speedperlevel);
			lastRandom = random; //Guardamos el ultimo random para saber que recompensa se ha generado la ultima 

			speedperlevel += 0.12;
		}
		entitiesaux++;
		speedperlevel += 1.5;
	}
}

void PescaState::crearBasura() {
	int rndx, rndy;
	for (int i = 0; i < totalBasura; i++) {
		do {
			rndx = sdlutils().rand().teCuoto(screen_width * 0.15, screen_width * 0.75);
			rndy = sdlutils().rand().teCuoto(screen_heigth * 0.6f, screen_heigth * 0.9f);
		} while (!checkBasura(rndx, rndy));
		auto* basura = createBasicEntity(Vector2D(rndx, rndy), Vector2D(w_reward, h_reward), 0.0f, Vector2D(0, 0));
		int random = sdlutils().rand().teCuoto(30, 32);
		if (random == 30) {
			basura->addComponent<Animation>("idle", &sdlutils().images().at("lata"), 1, 1, 1, 1, 0);
		}
		else if (random == 31) {
			basura->addComponent<Animation>("idle", &sdlutils().images().at("casco"), 1, 1, 1, 1, 0);
		}
		else if (random == 32) {
			basura->addComponent<Animation>("idle", &sdlutils().images().at("piedra"), 1, 1, 1, 1, 0);
		}
		basura->addComponent<BoxCollider>(STATIC, CEBO_GANCHO, CEBO_GANCHO_MASK);
		basura->addComponent<Reward>(random, playerRef, app, 0);
		basuras.push_back(basura);
	}
}

bool PescaState::checkBasura(int x, int y)
{
	if (basuras.empty()) {
		return true;
	}
	else {
		int i = 0;
		while ((i < basuras.size()) && ((x + 50 < basuras[i]->getComponent<Transform>()->getPos().getX() - 50 || x - 50 > basuras[i]->getComponent<Transform>()->getPos().getX() + 50) || (y + 50 < basuras[i]->getComponent<Transform>()->getPos().getY() - 50 || y - 50 > basuras[i]->getComponent<Transform>()->getPos().getY() + 50))) {
			i++;
		}
		if (i == basuras.size())
			return true;
	}
	return false;
}

