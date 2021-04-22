#include "TestZoneState.h"

using namespace ColLayers;

void TestZoneState::update() {
	manager_->getWorld()->Step(1.0f / 60.0f, 6, 2);
	GameState::update();
}

void TestZoneState::init() {
	//-----Map-----
#pragma region Map
	//---BG----
	auto* bg = createBasicEntity(Vector2D(0,1440),Vector2D(2560*2,1440*2),0.0f,Vector2D(0,0));
	bg->addComponent<Animation>("1", &sdlutils().images().at("sky"), 1, 1, 1, 1, 0);
	//---------

	/*Config gancho{};
	gancho.pos = Vector2D(sdlutils().width() * 0.8f, 400);
	gancho.vel = Vector2D(0, 0);
	gancho.size = Vector2D(100.0f, 100.0f);
	gancho.friction = 0.0f;
	gancho.physicType = DYNAMIC;
	gancho.fixedRotation = true;
	gancho.rotation = 0.0f;
	gancho.col = DEFAULT;
	gancho.colMask = DEFAULT_MASK;
	createPesca(gancho);*/
	
	Config floor{};
	floor.pos = Vector2D(200, sdlutils().height() * 2.0f);
	floor.vel = Vector2D(0, 0);
	floor.size = Vector2D(sdlutils().width() * 4.0f, 100.0f);
	floor.friction = 0.0f;
	floor.physicType = STATIC;
	floor.fixedRotation = true;
	floor.rotation = 0.0f;
	floor.col = GROUND;
	floor.colMask = GROUND_MASK;
	createBoxFloor(floor);
#pragma endregion
//-----Player-----
#pragma region Player
	Config playerConfig{};
	playerConfig.pos = Vector2D(100, sdlutils().height() * 1.5f);
	playerConfig.vel = Vector2D(0, 0);
	playerConfig.size = Vector2D(100.0f, 100.0f);
	playerConfig.friction = 0.2f;
	playerConfig.physicType = DYNAMIC;
	playerConfig.fixedRotation = true;
	playerConfig.rotation = 0.0f;
	playerConfig.col = PLAYER;
	playerConfig.colMask = PLAYER_MASK;
	createPlayer(playerConfig);
#pragma endregion
//-----Enemies-----
#pragma region Enemies
	/*#pragma region PompeyWorm
	Config pompeyWorm{};
	pompeyWorm.pos = Vector2D(700, sdlutils().height() * 2.0f - 200);
	pompeyWorm.vel = Vector2D(0, 0);
	pompeyWorm.size = Vector2D(100.0f, 100.0f);
	pompeyWorm.friction = 0.2f;
	pompeyWorm.physicType = DYNAMIC;
	pompeyWorm.fixedRotation = true;
	pompeyWorm.rotation = 0.0f;
	pompeyWorm.col = ENEMY;
	pompeyWorm.colMask = ENEMY_MASK;
	createPompeyWorm(pompeyWorm);
	#pragma endregion
	#pragma region ElfShark
	/*Config elfShark{};
	elfShark.pos = Vector2D(sdlutils().width() * 1.6f, sdlutils().height() * 0.3f);
	elfShark.vel = Vector2D(0, 0);
	elfShark.size = Vector2D(180.0f, 180.0f);
	elfShark.friction = 0.2f;
	elfShark.physicType = DYNAMIC;
	elfShark.fixedRotation = true;
	elfShark.rotation = 0.0f;
	elfShark.col = ENEMY;
	elfShark.colMask = ENEMY_MASK;
	createElfShark(elfShark);*/
	#pragma endregion
	#pragma region FringeHead
	/*auto* enemy = manager_->addEntity();
	Transform* t= enemy->addComponent<Transform>(Vector2D(sdlutils().width() / 1.7f, sdlutils().height() / 1.65f), Vector2D(0, 0), 70.0f, 70.0f, 0.0f);
	enemy->addComponent<BoxCollider>(STATIC, ENEMY, ENEMY_MASK);
	AnimBlendGraph* anim_controller = enemy->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("idle", &sdlutils().images().at("Medusa"), 7, 6, 38, 40, -1, 0, 37);
	enemy->addComponent<FringeHeadAtack>();
	enemy->addComponent<Enemy_Health>(200);*/
	#pragma endregion
#pragma endregion
}

void TestZoneState::createPlayer(const Config& playerConfig) {
	auto* player = createBasicEntity(playerConfig.pos, playerConfig.size, playerConfig.rotation, playerConfig.vel);

#pragma region Animations
	//Plantilla de uso de ANIMATION CONTROLLER
	auto* anim_controller = player->addComponent<AnimBlendGraph>();
#pragma region idle, run & jump
	//-idle, run & jump---------------------------------------------------------------------------------------------------
	//Animations
	anim_controller->addAnimation("idle", &sdlutils().images().at("player_idle"), 4, 6, 24, 24, -1);
	anim_controller->addAnimation("run", &sdlutils().images().at("player_run"), 4, 5, 20, 24, -1);
	anim_controller->addAnimation("jump", &sdlutils().images().at("player_jump"), 4, 6, 24, 24, 0);
	//Proportion?
	anim_controller->keepProportion("idle", Vector2D(player->getComponent<Transform>()->getW(), player->getComponent<Transform>()->getH()));
	//Transitions
	anim_controller->addTransition("idle", "run", "Speed", 1, false);
	anim_controller->addTransition("run", "idle", "Speed", 0, false);
	anim_controller->addTransition("run", "jump", "NotOnFloor", 1, false);	//Anim fuente, anim destino, parametro, valor de parametro, esperar a que termine la animacion
	anim_controller->addTransition("jump", "run", "NotOnFloor", 0, true);
	anim_controller->addTransition("idle", "jump", "NotOnFloor", 1, false);
	anim_controller->addTransition("jump", "idle", "NotOnFloor", 0, true);
	anim_controller->setParamValue("NotOnFloor", 0);	//AVISO: Si no existe el parametro, no hara nada
	anim_controller->setParamValue("Speed", 0);
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion
#pragma region Weapons
	//-WEAPONS------------------------------------------------------------------------------------------------------
#pragma region Chainsaw
	//---CHAINSAW---------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("chainsaw_attack1", &sdlutils().images().at("chainsaw_combo"), 6, 8, 48, 24, 0, 1, 8, Vector2D(0.75,0.72));
	anim_controller->addAnimation("chainsaw_attack2", &sdlutils().images().at("chainsaw_combo"), 6, 8, 48, 24, 0, 9, 18, Vector2D(0.75, 0.72));
	anim_controller->addAnimation("chainsaw_attack3", &sdlutils().images().at("chainsaw_combo"), 6, 8, 48, 24, 0, 19, 47, Vector2D(0.75, 0.72)); // provisional, habria que hacer una de mantener

	anim_controller->addTransition("run", "chainsaw_attack1", "chainsaw_att", 1, false);
	anim_controller->addTransition("idle", "chainsaw_attack1", "chainsaw_att", 1, false);
	anim_controller->addTransition("chainsaw_attack1", "run", "chainsaw_att", 0, true);
	anim_controller->addTransition("chainsaw_attack1", "chainsaw_attack2", "chainsaw_att", 2, true);
	anim_controller->addTransition("run", "chainsaw_attack2", "chainsaw_att", 2, false);
	anim_controller->addTransition("idle", "chainsaw_attack2", "chainsaw_att", 2, false);
	anim_controller->addTransition("chainsaw_attack2", "run", "chainsaw_att", 0, true);
	anim_controller->addTransition("chainsaw_attack2", "chainsaw_attack3", "chainsaw_att", 3, true);
	anim_controller->addTransition("run", "chainsaw_attack3", "chainsaw_att", 3, false);
	anim_controller->addTransition("idle", "chainsaw_attack3", "chainsaw_att", 3, false);
	anim_controller->addTransition("chainsaw_attack3", "run", "chainsaw_att", 0, false);
	anim_controller->addTransition("chainsaw_attack3", "chainsaw_attack1", "chainsaw_att", 4, true);

	anim_controller->setParamValue("chainsaw_att", 0);
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion

#pragma region Sword
	//---SWORD------------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("sword_attack1", &sdlutils().images().at("sword_combo"), 6, 8, 48, 48, 0, 1, 9, Vector2D(0.71,0.69));
	anim_controller->addAnimation("sword_attack2", &sdlutils().images().at("sword_combo"), 6, 8, 48, 48, 0, 12, 20, Vector2D(0.71, 0.69));
	anim_controller->addAnimation("sword_attack3", &sdlutils().images().at("sword_combo"), 6, 8, 48, 48, -1, 27, 44, Vector2D(0.71, 0.69));

	anim_controller->addTransition("run", "sword_attack1", "sword_att", 1, false);
	anim_controller->addTransition("idle", "sword_attack1", "sword_att", 1, false);
	anim_controller->addTransition("sword_attack1", "run", "sword_att", 0, true);
	anim_controller->addTransition("sword_attack1", "sword_attack2", "sword_att", 2, true);
	anim_controller->addTransition("run", "sword_attack2", "sword_att", 2, false);
	anim_controller->addTransition("idle", "sword_attack2", "sword_att", 2, false);
	anim_controller->addTransition("sword_attack2", "run", "sword_att", 0, true);
	anim_controller->addTransition("sword_attack2", "sword_attack3", "sword_att", 3, true);
	anim_controller->addTransition("run", "sword_attack3", "sword_att", 3, false);
	anim_controller->addTransition("idle", "sword_attack3", "sword_att", 3, false);
	anim_controller->addTransition("sword_attack3", "run", "sword_att", 0, false);
	anim_controller->addTransition("sword_attack3", "sword_attack1", "sword_att", 4, true);

	anim_controller->setParamValue("sword_att", 0);
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion

#pragma region Hammer
//---HAMMER---------------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("hammer_attack1", &sdlutils().images().at("hammer_combo"), 5, 7, 31, 48, 0, 1, 16, Vector2D(0.75, 0.69));
	anim_controller->addAnimation("hammer_attack2", &sdlutils().images().at("hammer_combo"), 5, 7, 31, 48, 0, 17, 30, Vector2D(0.75, 0.69));

	anim_controller->addTransition("run", "hammer_attack1", "hammer_att", 1, false);
	anim_controller->addTransition("idle", "hammer_attack1", "hammer_att", 1, false);
	anim_controller->addTransition("hammer_attack1", "run", "hammer_att", 0, false);
	anim_controller->addTransition("hammer_attack1", "hammer_attack2", "hammer_att", 2, true);
	anim_controller->addTransition("run", "hammer_attack2", "hammer_att", 2, false);
	anim_controller->addTransition("idle", "hammer_attack2", "hammer_att", 2, false);
	anim_controller->addTransition("hammer_attack2", "run", "hammer_att", 0, false);
	anim_controller->addTransition("hammer_attack2", "hammer_attack1", "hammer_att", 1, false);

	anim_controller->setParamValue("hammer_att", 0);
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion

#pragma region Crab
//---CRAB---------------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("crab_attack1", &sdlutils().images().at("crab_combo"), 6, 7, 39, 48, 0, 1, 8, Vector2D(0.71,0.5));
	anim_controller->addAnimation("crab_attack2", &sdlutils().images().at("crab_combo"), 6, 7, 39, 48, 0, 8, 25, Vector2D(0.71, 0.5));
	anim_controller->addAnimation("crab_attack3", &sdlutils().images().at("crab_combo"), 6, 7, 39, 48, 0, 27, 38, Vector2D(0.71, 0.5));

	anim_controller->addTransition("run", "crab_attack1", "crab_att", 1, false);
	anim_controller->addTransition("idle", "crab_attack1", "crab_att", 1, false);
	anim_controller->addTransition("crab_attack1", "run", "crab_att", 0, true);
	anim_controller->addTransition("crab_attack1", "crab_attack2", "crab_att", 2, true);
	anim_controller->addTransition("run", "crab_attack2", "crab_att", 2, false);
	anim_controller->addTransition("idle", "crab_attack2", "crab_att", 2, false);
	anim_controller->addTransition("crab_attack2", "run", "crab_att", 0, false);
	anim_controller->addTransition("crab_attack2", "crab_attack3", "crab_att", 3, false);
	anim_controller->addTransition("run", "crab_attack3", "crab_att", 3, false);
	anim_controller->addTransition("idle", "crab_attack3", "crab_att", 3, false);
	anim_controller->addTransition("crab_attack3", "run", "crab_att", 0, false);
	anim_controller->addTransition("crab_attack3", "crab_attack1", "crab_att", 4, true);

	anim_controller->setParamValue("crab_att", 0);
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion
#pragma endregion

	player->addComponent<BoxCollider>(playerConfig.physicType, PLAYER, PLAYER_MASK, false, playerConfig.friction, playerConfig.fixedRotation, playerConfig.rotation);
	player->addComponent<Player_Health>(&sdlutils().images().at("fullvida"), &sdlutils().images().at("mediavida"), &sdlutils().images().at("vacio"), 300.0f, app);
	player->addComponent<Armas_HUD>(&sdlutils().images().at("sierra"), &sdlutils().images().at("espada"), app);
	//player->addComponent<SoundManager>(0, "FinalBoss");

	/*if (playerConfig.physicType != KINEMATIC) */player->addComponent<PlayerController>();
	//else player->addComponent<KeyBoardCtrl>(map);

	//SE NECESITA LA VARIABLE DE CAMLIMITS DE PLAYSTATE; POR ESO ESTA COMENTADO
	Vector2D mapLimit = Vector2D(2000, 2000);
	player->addComponent<CameraFollow>(player->getComponent<Transform>(), &mapLimit, Vector2D(250.0f, -300.0f), 0.06f, app->getCameraZooOut(), true, true); //Vector2D offset y porcentaje de la velocidad de la camara, mas bajo mas lento sigue
	player->addComponent<Sword>();

	player->addComponent<LoseLife>();

	//Seteamos al Player como MainHandler
	manager_->setHandler<Player>(player);
}
/// <summary>
/// Crea un suelo Statico
void TestZoneState::createBoxFloor(const Config& entityConfig) {

	auto* box_Floor = createBasicEntity(entityConfig.pos, entityConfig.size, entityConfig.rotation, entityConfig.vel);
	box_Floor->addComponent<Animation>("Floor", &sdlutils().images().at("red_square"), 1, 1, 1, 1, 0);
	box_Floor->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask, false, entityConfig.friction, entityConfig.fixedRotation, entityConfig.rotation);
}

void TestZoneState::createPompeyWorm(const Config& enemy1Config)
{
	auto* gusano = createBasicEntity(enemy1Config.pos, enemy1Config.size, enemy1Config.rotation, enemy1Config.vel);
	gusano->addComponent<BoxCollider>(enemy1Config.physicType, enemy1Config.col, enemy1Config.colMask);
	AnimBlendGraph* gusano_anim_controller = gusano->addComponent<AnimBlendGraph>();
	gusano_anim_controller->addAnimation("idle", &sdlutils().images().at("Elf_Shark"), 1, 3, 1, 1, -1);
	gusano_anim_controller->addAnimation("attack", &sdlutils().images().at("Elf_Shark"), 1, 3, 3, 8, 0);
	gusano_anim_controller->addTransition("idle", "attack", "Attack", 1, false);
	gusano_anim_controller->addTransition("attack", "idle", "Attack", 0, true);
	gusano_anim_controller->setParamValue("Attack", 0);
	auto* trigger_gusano = gusano->addComponent<EnemyTrigger>(Vector2D(800.0f, 500.0f));
	trigger_gusano->addTriggerComponent<PompeyWormAttack>(gusano);
	gusano->addComponent<Enemy_Health>(300, Vector2D(300, 20), build_sdlcolor(255, 0, 0, 255), 50);
}
void TestZoneState::createElfShark(const Config& entityConfig) {
	auto* elf1 = createBasicEntity(entityConfig.pos, entityConfig.size, entityConfig.rotation, entityConfig.vel);
	//auto* elf1 = manager_->addEntity();
	//Transform* t = elf1->addComponent<Transform>(Vector2D(sdlutils().width() * 1.6f, sdlutils().height() * 0.3f), Vector2D(0, 0), 180.0f, 180.0f, 0.0f);
	//elf1->addComponent<BoxCollider>(KINEMATIC, ENEMY, ENEMY_MASK);
	elf1->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);
	AnimBlendGraph* elf1_anim_controller = elf1->addComponent<AnimBlendGraph>();
	elf1_anim_controller->addAnimation("idle", &sdlutils().images().at("Elf_Shark"), 1, 3, 1, 1, -1);
	elf1_anim_controller->addAnimation("attack", &sdlutils().images().at("Elf_Shark"), 1, 3, 3, 8, 0);
	elf1_anim_controller->addTransition("idle", "attack", "Attack", 1, false);
	elf1_anim_controller->addTransition("attack", "idle", "Attack", 0, true);
	auto* trigger_elf1 = elf1->addComponent<EnemyTrigger>(Vector2D(1000.0f, 600.0f));
	trigger_elf1->addTriggerComponent<ElfSharkAttack>(elf1);
	elf1->addComponent<Enemy_Health>(300, Vector2D(300, 20), build_sdlcolor(255, 0, 0, 255), 50);
}

void TestZoneState::createPesca(const Config& entityConfig) {
	

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
	



	/*auto* player = createBasicEntity(entityConfig.pos + Vector2D(100, 50), Vector2D(256, 256), 0.0f, Vector2D(0, 0));
	AnimBlendGraph* player_anim_controller = player->addComponent<AnimBlendGraph>();
	player_anim_controller->addAnimation("player", &sdlutils().images().at("player_cana"), 1, 1, 1, 1, 0);
	BoxCollider* playercollider_ = player->addComponent<BoxCollider>(DYNAMIC, DEFAULT, DEFAULT_MASK);
	playercollider_->getFixture()->SetSensor(true);
	player->addComponent<PescaController>();*/

	auto* gancho = createBasicEntity(entityConfig.pos, entityConfig.size, entityConfig.rotation, entityConfig.vel);
	AnimBlendGraph* gancho_anim_controller = gancho->addComponent<AnimBlendGraph>();
	gancho_anim_controller->addAnimation("idle", &sdlutils().images().at("fullvida"), 1, 8, 8, 8, -1);
	gancho->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);
	gancho->addComponent<Gancho>();
	gancho->addComponent<PescaController>();

	auto* reward0 = createBasicEntity(Vector2D(sdlutils().width() / 2, 800), Vector2D(128,128), 0.0f, Vector2D(0,0));
	AnimBlendGraph* reward0_anim_controller = reward0->addComponent<AnimBlendGraph>();
	reward0_anim_controller->addAnimation("idle", &sdlutils().images().at("sierra"), 1, 1, 1, 1, 0);
	reward0->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);
	reward0->addComponent<Reward>();


	auto* cuerda = createBasicEntity(entityConfig.pos + Vector2D(15, -30), Vector2D(16.0f, 16.0f), 0.0f, Vector2D(0, 0));
	AnimBlendGraph* cuerda_anim_controller = cuerda->addComponent<AnimBlendGraph>();
	cuerda_anim_controller->addAnimation("cuerda", &sdlutils().images().at("cuerda"), 1, 1, 1, 1, 0);
	cuerda->addComponent<BoxCollider>(DYNAMIC, DEFAULT, DEFAULT_MASK);
	cuerda->addComponent<Cuerda>(gancho);
	cuerda->addComponent<PescaController>();
	


}

