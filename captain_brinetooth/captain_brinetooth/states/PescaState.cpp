#include "PescaState.h"
using namespace ColLayers;

void PescaState::update() {
	manager_->getWorld()->Step(1.0f / 60.0f, 6, 2);
	GameState::update();
}

void PescaState::init() {
	//-----Map-----
#pragma region Map
	//---BG----
	//manager_->getWorld()->SetContactListener(&collisionListener);

	w = (int)sdlutils().width() * App::camera_Zoom_Out * 0.035;
	h = (int)w * 0.7;    //mantener aspect ratio
	int x = (int)((App::camera.w - w) * 0.5);


	//Fill array
	for (int i = 0; i < rowHeights.size(); i++)
	{
		int y = (int)((App::camera.h - h) * (0.40 + 0.8 * i));
		rowHeights[i] = y;
	}


	auto* bg = createBasicEntity(Vector2D(0, 1440), Vector2D(2560 * 2, 1440 * 2), 0.0f, Vector2D(0, 0));
	bg->addComponent<Animation>("1", &sdlutils().images().at("sky"), 1, 1, 1, 1, 0);
	//---------
	entitiesPerLine = 7;
	Config gancho{};
	gancho.pos = Vector2D(sdlutils().width(), 370);
	gancho.vel = Vector2D(0, 0);
	gancho.size = Vector2D(100.0f, 100.0f);
	gancho.friction = 0.0f;
	gancho.physicType = DYNAMIC;
	gancho.fixedRotation = true;
	gancho.rotation = 0.0f;
	gancho.col = DEFAULT;
	gancho.colMask = DEFAULT_MASK;
	createPesca(gancho);
	//Metodo para cambiar el zoom 

}
void PescaState::createPesca(const Config& entityConfig) {


	auto* floor = createBasicEntity(Vector2D(sdlutils().width(), sdlutils().height() * 2), Vector2D(sdlutils().width() + 1200, 500), 0.0f, Vector2D(0, 0));
	floor->addComponent<Animation>("debug", &sdlutils().images().at("arena"), 1, 1, 1, 1, 0);
	floor->addComponent<BoxCollider>(KINEMATIC, DEFAULT, DEFAULT_MASK);

	auto* muelle = createBasicEntity(Vector2D(sdlutils().width(), sdlutils().height() / 2), Vector2D(512 * 3, 128 * 3), 0, Vector2D(0, 0));
	muelle->addComponent<Animation>("muelle", &sdlutils().images().at("muelle"), 1, 1, 1, 1, 0);

	auto* topRod = createBasicEntity(entityConfig.pos + Vector2D(0, -entityConfig.size.getY() - 5), entityConfig.size, 0.0f, Vector2D(0, 0));
	topRod->addComponent<Animation>("debug", &sdlutils().images().at("debug_square"), 1, 1, 1, 1, 0);
	topRod->addComponent<BoxCollider>(DYNAMIC, DEFAULT, DEFAULT_MASK);
	topRod->addComponent<PescaController>();
	topRod->getMngr()->setHandler<Rod>(topRod); //Para deteccion de colision con el gancho 




	auto* player = createBasicEntity(entityConfig.pos + Vector2D(100, 20), Vector2D(256, 256), 0.0f, Vector2D(0, 0));
	player->addComponent<Animation>("player", &sdlutils().images().at("player_cana"), 1, 1, 1, 1, 0);
	BoxCollider* playercollider_ = player->addComponent<BoxCollider>(DYNAMIC, DEFAULT, DEFAULT_MASK);
	playercollider_->getFixture()->SetSensor(true);
	player->addComponent<PescaController>();

	auto* gancho = createBasicEntity(entityConfig.pos, entityConfig.size, entityConfig.rotation, entityConfig.vel);
	gancho->addComponent<Animation>("idle", &sdlutils().images().at("fullvida"), 1, 8, 8, 8, -1);
	gancho->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);
	gancho->addComponent<Gancho>(app);
	gancho->addComponent<PescaController>();



	//Rewards 


	createRandomReward(entityConfig);




	auto* cuerda = createBasicEntity(entityConfig.pos + Vector2D(15, -30), Vector2D(10.0f, 16.0f), 0.0f, Vector2D(0, 0));
	AnimBlendGraph* cuerda_anim_controller = cuerda->addComponent<AnimBlendGraph>();
	cuerda_anim_controller->addAnimation("cuerda", &sdlutils().images().at("cuerda"), 1, 1, 1, 1, 0);
	cuerda->addComponent<BoxCollider>(DYNAMIC, DEFAULT, DEFAULT_MASK);
	cuerda->addComponent<Cuerda>(gancho);
	cuerda->addComponent<PescaController>();



}
void PescaState::createRandomReward(const Config& entityConfig)
{
	//0 espada 1 martillo 2 sierra ---> se iran añadiendo segun vaya habiendo mas armas
	int random;
	Vector2D pos1 = Vector2D(sdlutils().width() / 2, (float)(App::camera.h - h) + h * 1.2);
	Vector2D pos2 = Vector2D(sdlutils().width() / 2, pos1.getY() + h * 1.2);
	Vector2D pos3 = Vector2D(sdlutils().width() / 2, pos2.getY() + h * 1.2);
	for (int i = 0; i < entitiesPerLine; i++)
	{
		random = sdlutils().rand().teCuoto(0, 7);

		auto* reward0 = createBasicEntity(Vector2D(pos1.getX() + (200 * i), pos1.getY()), Vector2D(w, h), 0.0f, Vector2D(0, 0));

		if (random == 0)
		{
			reward0->addComponent<Animation>("idle", &sdlutils().images().at("espada"), 1, 1, 1, 1, 0);
			reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);

		}
		else if (random == 1)
		{
			reward0->addComponent<Animation>("idle", &sdlutils().images().at("martillo"), 1, 1, 1, 1, 0);
			reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);

		}
		else if (random == 2)
		{
			reward0->addComponent<Animation>("idle", &sdlutils().images().at("sierra"), 1, 1, 1, 1, 0);
			reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);
		}
		else //No es ninguna recompensa activa 
		{
			int rnd = sdlutils().rand().teCuoto(0, 15);
			if (rnd >= 0 && rnd <= 5)
			{
				reward0->addComponent<Animation>("idle", &sdlutils().images().at("piedra"), 1, 1, 1, 1, 0);
				reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);

			}
			else if (rnd >= 6 && rnd < 10)
			{

				reward0->addComponent<Animation>("idle", &sdlutils().images().at("casco"), 1, 1, 1, 1, 0);
				reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);

			}
			else  if (rnd >= 10 && rnd <= 14)
			{

				reward0->addComponent<Animation>("idle", &sdlutils().images().at("lata"), 1, 1, 1, 1, 0);
				//Pones a pelo tanto el tipo como el colision mask-> acuerdate de poner la capa para que solo toque el gancho , es decir que es para cuando el gancho baja 
				reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);

			}
			random = 30;
		}

		reward0->addComponent<Reward>(random);


	}
	for (int i = 0; i < entitiesPerLine; i++)
	{
		random = sdlutils().rand().teCuoto(0, 7);

		auto* reward0 = createBasicEntity(Vector2D(pos2.getX() + (200 * i), pos2.getY()), Vector2D(w, h), 0.0f, Vector2D(0, 0));

		if (random == 0)
		{
			reward0->addComponent<Animation>("idle", &sdlutils().images().at("espada"), 1, 1, 1, 1, 0);
			reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);

		}
		else if (random == 1)
		{
			reward0->addComponent<Animation>("idle", &sdlutils().images().at("martillo"), 1, 1, 1, 1, 0);
			reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);

		}
		else if (random == 2)
		{
			reward0->addComponent<Animation>("idle", &sdlutils().images().at("sierra"), 1, 1, 1, 1, 0);
			reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);
		}
		else //No es ninguna recompensa activa 
		{
			int rnd = sdlutils().rand().teCuoto(0, 15);
			if (rnd >= 0 && rnd <= 5)
			{
				reward0->addComponent<Animation>("idle", &sdlutils().images().at("piedra"), 1, 1, 1, 1, 0);
				reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);

			}
			else if (rnd >= 6 && rnd < 10)
			{

				reward0->addComponent<Animation>("idle", &sdlutils().images().at("casco"), 1, 1, 1, 1, 0);
				reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);

			}
			else  if (rnd >= 10 && rnd <= 14)
			{

				reward0->addComponent<Animation>("idle", &sdlutils().images().at("lata"), 1, 1, 1, 1, 0);
				//Pones a pelo tanto el tipo como el colision mask-> acuerdate de poner la capa para que solo toque el gancho , es decir que es para cuando el gancho baja 
				reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);

			}
			random = 30;
		}

		reward0->addComponent<Reward>(random);


	}
	for (int i = 0; i < entitiesPerLine; i++)
	{
		random = sdlutils().rand().teCuoto(0, 7);

		auto* reward0 = createBasicEntity(Vector2D(pos3.getX() + (200 * i), pos3.getY()), Vector2D(w, h), 0.0f, Vector2D(0, 0));

		if (random == 0)
		{
			reward0->addComponent<Animation>("idle", &sdlutils().images().at("espada"), 1, 1, 1, 1, 0);
			reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);

		}
		else if (random == 1)
		{
			reward0->addComponent<Animation>("idle", &sdlutils().images().at("martillo"), 1, 1, 1, 1, 0);
			reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);

		}
		else if (random == 2)
		{
			reward0->addComponent<Animation>("idle", &sdlutils().images().at("sierra"), 1, 1, 1, 1, 0);
			reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);
		}
		else //No es ninguna recompensa activa 
		{
			int rnd = sdlutils().rand().teCuoto(0, 15);
			if (rnd >= 0 && rnd <= 5)
			{
				reward0->addComponent<Animation>("idle", &sdlutils().images().at("piedra"), 1, 1, 1, 1, 0);
				reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);

			}
			else if (rnd >= 6 && rnd < 10)
			{

				reward0->addComponent<Animation>("idle", &sdlutils().images().at("casco"), 1, 1, 1, 1, 0);
				reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);

			}
			else  if (rnd >= 10 && rnd <= 14)
			{

				reward0->addComponent<Animation>("idle", &sdlutils().images().at("lata"), 1, 1, 1, 1, 0);
				//Pones a pelo tanto el tipo como el colision mask-> acuerdate de poner la capa para que solo toque el gancho , es decir que es para cuando el gancho baja 
				reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);

			}
			random = 30;
		}

		reward0->addComponent<Reward>(random);


	}
}

