#include "PescaState.h"
#include "..//components/DamageCollisionCallbacks.h"
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


	auto* floor = createBasicEntity(Vector2D(sdlutils().width(), sdlutils().height() * 2), Vector2D(sdlutils().width()+1200, 500), 0.0f, Vector2D(0, 0));
	AnimBlendGraph* floor_anim_controller = floor->addComponent<AnimBlendGraph>();
	floor_anim_controller->addAnimation("debug", &sdlutils().images().at("arena"), 1, 1, 1, 1, 0);
	floor->addComponent<BoxCollider>(KINEMATIC, DEFAULT, DEFAULT_MASK);

	auto* muelle = createBasicEntity(Vector2D(sdlutils().width(), sdlutils().height() / 2), Vector2D(512 * 3, 128 * 3), 0, Vector2D(0, 0));
	AnimBlendGraph* muelle_anim_controller = muelle->addComponent<AnimBlendGraph>();
	muelle_anim_controller->addAnimation("muelle", &sdlutils().images().at("muelle"), 1, 1, 1, 1, 0);

	auto* topRod = createBasicEntity(entityConfig.pos + Vector2D(0, -entityConfig.size.getY() - 5), entityConfig.size, 0.0f, Vector2D(0, 0));
	AnimBlendGraph* topRod_anim_controller = topRod->addComponent<AnimBlendGraph>();
	topRod_anim_controller->addAnimation("debug", &sdlutils().images().at("debug_square"), 1, 1, 1, 1, 0);
	topRod->addComponent<BoxCollider>(DYNAMIC, DEFAULT, DEFAULT_MASK);
	topRod->addComponent<PescaController>();
	topRod->getMngr()->setHandler<Rod>(topRod); //Para deteccion de colision con el gancho 




	auto* player = createBasicEntity(entityConfig.pos + Vector2D(100, 50), Vector2D(256, 256), 0.0f, Vector2D(0, 0));
	AnimBlendGraph* player_anim_controller = player->addComponent<AnimBlendGraph>();
	player_anim_controller->addAnimation("player", &sdlutils().images().at("player_cana"), 1, 1, 1, 1, 0);
	BoxCollider* playercollider_ = player->addComponent<BoxCollider>(DYNAMIC, DEFAULT, DEFAULT_MASK);
	playercollider_->getFixture()->SetSensor(true);
	player->addComponent<PescaController>();

	auto* gancho = createBasicEntity(entityConfig.pos, entityConfig.size, entityConfig.rotation, entityConfig.vel);
	AnimBlendGraph* gancho_anim_controller = gancho->addComponent<AnimBlendGraph>();
	gancho_anim_controller->addAnimation("idle", &sdlutils().images().at("fullvida"), 1, 8, 8, 8, -1);
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
	Vector2D pos1 = Vector2D(sdlutils().width() / 2, 800);
	Vector2D pos2 = Vector2D(sdlutils().width() / 2, 1000);
	Vector2D pos3 = Vector2D(sdlutils().width() / 2, 1200);
	for (int i = 0; i < entitiesPerLine; i++)
	{
		random = sdlutils().rand().teCuoto(0, 7);
	
			auto* reward0 = createBasicEntity(Vector2D(pos1.getX()+(200*i), pos1.getY()), Vector2D(90, 90), 0.0f, Vector2D(0, 0));
			AnimBlendGraph* reward0_anim_controller = reward0->addComponent<AnimBlendGraph>();
			if (random == 0)
			{
				reward0_anim_controller->addAnimation("idle", &sdlutils().images().at("espada"), 1, 1, 1, 1, 0);
				
			}
			else if (random == 1)
			{
				reward0_anim_controller->addAnimation("idle", &sdlutils().images().at("martillo"), 1, 1, 1, 1, 0);
				
			}
			else if(random==2)
			{
				reward0_anim_controller->addAnimation("idle", &sdlutils().images().at("sierra"), 1, 1, 1, 1, 0);
			}
			else //No es ninguna recompensa activa 
			{
				reward0_anim_controller->addAnimation("idle", &sdlutils().images().at("piedra"), 1, 1, 1, 1, 0);
				random = 30;
			}
			reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);
			reward0->addComponent<Reward>(random);
			

	}
	for (int i = 0; i < entitiesPerLine; i++)
	{
		random = sdlutils().rand().teCuoto(0, 3);

		auto* reward0 = createBasicEntity(Vector2D(pos2.getX() + (200 * i), pos2.getY()), Vector2D(90, 90), 0.0f, Vector2D(0, 0));
		AnimBlendGraph* reward0_anim_controller = reward0->addComponent<AnimBlendGraph>();
		if (random == 0)
		{
			reward0_anim_controller->addAnimation("idle", &sdlutils().images().at("espada"), 1, 1, 1, 1, 0);

		}
		else if (random == 1)
		{
			reward0_anim_controller->addAnimation("idle", &sdlutils().images().at("martillo"), 1, 1, 1, 1, 0);

		}
		else
		{
			reward0_anim_controller->addAnimation("idle", &sdlutils().images().at("sierra"), 1, 1, 1, 1, 0);
		}
		reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);
		reward0->addComponent<Reward>(random);

	}
	for (int i = 0; i < entitiesPerLine; i++)
	{
		random = sdlutils().rand().teCuoto(0, 3);

		auto* reward0 = createBasicEntity(Vector2D(pos3.getX() + (200 * i), pos3.getY()), Vector2D(90, 90), 0.0f, Vector2D(0, 0));
		AnimBlendGraph* reward0_anim_controller = reward0->addComponent<AnimBlendGraph>();
		if (random == 0)
		{
			reward0_anim_controller->addAnimation("idle", &sdlutils().images().at("espada"), 1, 1, 1, 1, 0);

		}
		else if (random == 1)
		{
			reward0_anim_controller->addAnimation("idle", &sdlutils().images().at("martillo"), 1, 1, 1, 1, 0);

		}
		else
		{
			reward0_anim_controller->addAnimation("idle", &sdlutils().images().at("sierra"), 1, 1, 1, 1, 0);
		}
		reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);
		reward0->addComponent<Reward>(random);

	}
}
