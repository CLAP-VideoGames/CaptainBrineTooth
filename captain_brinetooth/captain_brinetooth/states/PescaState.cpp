#include "PescaState.h"
using namespace ColLayers;

PescaState::~PescaState() {
	app->setCameraZoomOut(main_zoom);
	//Falta poner la musica bien de nuevo 
	playerRef->getComponent<PlayerController>()->playerReceiveInput(true);

	//Falta cambiar la musica con el sounmanager a la de playstate 
};


void PescaState::update() {
	manager_->getWorld()->Step(1.0f / 60.0f, 6, 2);
	GameState::update();
}

void PescaState::init() {
	//-----Map-----
#pragma region Map
	//Metodo para cambiar el zoom 
	main_zoom = app->getCameraZoomOut();
	app->setCameraZoomOut(2.0f);

	playerRef->getComponent<PlayerController>()->playerReceiveInput(false);


	//---BG----
	//manager_->getWorld()->SetContactListener(&collisionListener);

	//Ancho y alto de la ventana para situar mejor los sprites
	screen_width = sdlutils().width() * App::camera_Zoom_Out;
	screen_heigth = sdlutils().height() * App::camera_Zoom_Out;

	w_reward = (int)40 * App::camera_Zoom_Out;
	h_reward = (int)30 * App::camera_Zoom_Out;    //mantener aspect ratio
	int y = (int)(screen_heigth * 0.5 - h_reward);	//Offset hacia abajo con respecto al jugador

	//Fill array
	for (int i = 0; i < rows_; i++)
	{
		y = (int)(y + h_reward * 3);	//establece la posicion y de la columna, solo hay que cambiar el numero
		rowHeights.push_back(y);
	}

	auto* bg = createBasicEntity(Vector2D(screen_width / 2, screen_heigth / 2), Vector2D(screen_width, screen_heigth), 0.0f, Vector2D(0, 0));
	bg->addComponent<Animation>("1", &sdlutils().images().at("fondoPesca"), 1, 1, 1, 1, 0);
	//---------
	entitiesPerLine = 3;
	totalBasura = 8;

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

}
void PescaState::createPesca(const Config& entityConfig) {

	float z_factor = App::camera_Zoom_Out;	//Si quereis que el tamano sea con un numero multiplicadlo por esto

	//Suelo *arreglado
	auto* floor = createBasicEntity(Vector2D(entityConfig.pos.getX(), screen_heigth * 0.95), Vector2D(screen_width * 0.8, screen_heigth * 0.1), 0.0f, Vector2D(0, 0));
	//floor->addComponent<Animation>("debug", &sdlutils().images().at("arena"), 1, 1, 1, 1, 0);
	floor->addComponent<BoxCollider>(KINEMATIC, DEFAULT, DEFAULT_MASK);
	//cuerda *arreglada
	auto* topRod = createBasicEntity(entityConfig.pos + Vector2D(0, -entityConfig.size.getY() - 5 * z_factor), entityConfig.size, 0.0f, Vector2D(0, 0));
	topRod->addComponent<Animation>("debug", &sdlutils().images().at("debug_square"), 1, 1, 1, 1, 0);
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
	auto* gancho = createBasicEntity(entityConfig.pos, entityConfig.size, entityConfig.rotation, entityConfig.vel);
	gancho->addComponent<Animation>("idle", &sdlutils().images().at("fullvida"), 1, 8, 8, 8, -1);
	gancho->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);
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
	//0 espada 1 martillo 2 sierra ---> se iran añadiendo segun vaya habiendo mas armas
	int x = sdlutils().width() * App::camera_Zoom_Out;
	int random;
	for (int i = 0; i < rows_; i++) {
		for (int j = 0; j < entitiesPerLine; j++)
		{
			random = sdlutils().rand().teCuoto(0, 2);

			auto* reward0 = createBasicEntity(Vector2D(x + (100 * App::camera_Zoom_Out * j), rowHeights[i]), Vector2D(w_reward, h_reward), 0.0f, Vector2D(0, 0));

			if (random == 0)
				reward0->addComponent<Animation>("idle", &sdlutils().images().at("espada"), 1, 1, 1, 1, 0);

			else if (random == 1)
				reward0->addComponent<Animation>("idle", &sdlutils().images().at("martillo"), 1, 1, 1, 1, 0);

			else if (random == 2)
				reward0->addComponent<Animation>("idle", &sdlutils().images().at("sierra"), 1, 1, 1, 1, 0);



			reward0->addComponent<BoxCollider>(DYNAMIC, CEBO_GANCHO, CEBO_GANCHO_MASK);
			reward0->addComponent<Reward>(random, playerRef, app);

		}
	}
}

void PescaState::crearBasura() {
	for (int i = 0; i < totalBasura; i++) {
		int rndx = sdlutils().rand().teCuoto(screen_width * 0.15, screen_width * 0.75);
		int rndy = sdlutils().rand().teCuoto(screen_heigth * 0.6f, screen_heigth * 0.9f);
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
		basura->addComponent<Reward>(random, playerRef, app);
	}
}

