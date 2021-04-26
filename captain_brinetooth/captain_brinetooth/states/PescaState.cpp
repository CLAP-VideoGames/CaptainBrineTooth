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
	auto* bg = createBasicEntity(Vector2D(0, 1440), Vector2D(2560 * 2, 1440 * 2), 0.0f, Vector2D(0, 0));
	bg->addComponent<Animation>("1", &sdlutils().images().at("sky"), 1, 1, 1, 1, 0);
	//---------

	Config gancho{};
	gancho.pos = Vector2D(sdlutils().width() * 0.8f, 400);
	gancho.vel = Vector2D(0, 0);
	gancho.size = Vector2D(100.0f, 100.0f);
	gancho.friction = 0.0f;
	gancho.physicType = DYNAMIC;
	gancho.fixedRotation = true;
	gancho.rotation = 0.0f;
	gancho.col = DEFAULT;
	gancho.colMask = DEFAULT_MASK;
	createPesca(gancho);


}
void PescaState::createPesca(const Config& entityConfig) {
	

	auto* floor = createBasicEntity(Vector2D(sdlutils().width(), sdlutils().height() * 2), Vector2D(sdlutils().width(), 400), 0.0f, Vector2D(0, 0));
	AnimBlendGraph* floor_anim_controller = floor->addComponent<AnimBlendGraph>();
	floor_anim_controller->addAnimation("debug", &sdlutils().images().at("debug_square"), 1, 1, 1, 1, 0);
	floor->addComponent<BoxCollider>(KINEMATIC, DEFAULT, DEFAULT_MASK);

	auto* floor2 = createBasicEntity(entityConfig.pos + Vector2D(0, -entityConfig.size.getY() - 5), entityConfig.size, 0.0f, Vector2D(0, 0));
	AnimBlendGraph* floor2_anim_controller = floor2->addComponent<AnimBlendGraph>();
	floor2_anim_controller->addAnimation("debug", &sdlutils().images().at("debug_square"), 1, 1, 1, 1, 0);
	floor2->addComponent<BoxCollider>(DYNAMIC, DEFAULT, DEFAULT_MASK);
	floor2->addComponent<PescaController>();
	floor2->getMngr()->setHandler<Rod>(floor2); //Para deteccion de colision con el gancho 
	



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
	gancho->addComponent<Gancho>();
	gancho->addComponent<PescaController>();

	auto* reward0 = createBasicEntity(Vector2D(sdlutils().width() / 2, 800), Vector2D(90,90), 0.0f, Vector2D(0,0));
	AnimBlendGraph* reward0_anim_controller = reward0->addComponent<AnimBlendGraph>();
	reward0_anim_controller->addAnimation("idle", &sdlutils().images().at("sierra"), 1, 1, 1, 1, 0);
	reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);
	reward0->addComponent<Reward>(26);

	auto* reward1 = createBasicEntity(Vector2D(sdlutils().width() / 2, 1000), Vector2D(90, 90), 0.0f, Vector2D(0, 0));
	AnimBlendGraph* reward1_anim_controller = reward1->addComponent<AnimBlendGraph>();
	reward1_anim_controller->addAnimation("idle", &sdlutils().images().at("espada"), 1, 1, 1, 1, 0);
	reward1->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);
	reward1->addComponent<Reward>(1);

	auto* reward2 = createBasicEntity(Vector2D(sdlutils().width() / 2, 1200), Vector2D(90, 90), 0.0f, Vector2D(0, 0));
	AnimBlendGraph* reward2_anim_controller = reward2->addComponent<AnimBlendGraph>();
	reward2_anim_controller->addAnimation("idle", &sdlutils().images().at("martillo"), 1, 1, 1, 1, 0);
	reward2->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);
	reward2->addComponent<Reward>(23);


	auto* cuerda = createBasicEntity(entityConfig.pos + Vector2D(15, -30), Vector2D(10.0f, 16.0f), 0.0f, Vector2D(0, 0));
	AnimBlendGraph* cuerda_anim_controller = cuerda->addComponent<AnimBlendGraph>();
	cuerda_anim_controller->addAnimation("cuerda", &sdlutils().images().at("cuerda"), 1, 1, 1, 1, 0);
	cuerda->addComponent<BoxCollider>(DYNAMIC, DEFAULT, DEFAULT_MASK);
	cuerda->addComponent<Cuerda>(gancho);
	cuerda->addComponent<PescaController>();
	


}

