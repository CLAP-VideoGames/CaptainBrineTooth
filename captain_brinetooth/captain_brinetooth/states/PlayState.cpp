#include "PlayState.h"
#include "PasueState.h"
const auto MAP_PATH = "assets/maps/levelTest/levelTest - copia.tmx";

PlayState::PlayState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd): GameState(a, mundo, snd){
	//Testing floor
	//auto suelo = manager_->addEntity();
	//suelo->addComponent<Transform>(Vector2D(500, 600), Vector2D(), 500, 20, 0.0f);
	//auto anim = suelo->addComponent<AnimBlendGraph>();
	//anim->addAnimation("idle", &sdlutils().images().at("debug_square"), 1, 1, 1, 24, 0);

	//suelo->addComponent<BoxCollider>();
}

PlayState::~PlayState(){
}

void PlayState::init() {

	//SE TIENE QUE CREAR PRIMERO EL NIVEL Y LUEGO EL PLAYER
	createLevel0();


	/*float w = 252.63f;
	float h = 101.48f;
	Config floor{};
	floor.pos = Vector2D(1064 + (w/2), 1086 + (h + (h/2)));
	floor.vel = Vector2D(0, 0);
	floor.size = Vector2D(w, h);
	floor.friction = 0.0f;
	floor.physicType = STATIC;
	floor.fixedRotation = true;
	floor.rotation = 0.0f;
	floor.col = DEFAULT;
	floor.colMask = DEFAULT_MASK;

	auto* box_Floor = createBasicEntity(floor.pos, floor.size, floor.rotation, floor.vel);
	box_Floor->addComponent<BoxCollider>(floor.physicType, floor.col, floor.colMask, floor.isTrigger, floor.friction, floor.fixedRotation, floor.rotation);*/



	Config playerConfig{};
	playerConfig.pos = getMngr()->getHandler<Map>()->getComponent<MapProcedural>()->getPlayerPos();
	playerConfig.vel = Vector2D(0, 0);
	playerConfig.size = Vector2D(100.0f, 100.0f);
	playerConfig.friction = 0.2f;
	playerConfig.physicType = DYNAMIC;
	playerConfig.fixedRotation = true;
	playerConfig.rotation = 0.0f;
	createPlayer(playerConfig);

	Config swordGiverConfig{};
	swordGiverConfig.pos = Vector2D(sdlutils().width() / 2.5f, sdlutils().height());
	swordGiverConfig.vel = Vector2D();
	swordGiverConfig.size = Vector2D(50, 50);
	swordGiverConfig.friction = 0.2f;
	swordGiverConfig.physicType = STATIC;
	swordGiverConfig.fixedRotation = true;
	swordGiverConfig.rotation = 0.0f;
	swordGiverConfig.spriteId = "espada";
	int swordNumber = 0;
	createWeaponGiver(swordGiverConfig, swordNumber);

	Config hammerGiverConfig{};
	hammerGiverConfig.pos = Vector2D(sdlutils().width() / 2.0f, sdlutils().height());
	hammerGiverConfig.vel = Vector2D();
	hammerGiverConfig.size = Vector2D(50, 50);
	hammerGiverConfig.friction = 0.2f;
	hammerGiverConfig.physicType = STATIC;
	hammerGiverConfig.fixedRotation = true;
	hammerGiverConfig.rotation = 0.0f;
	hammerGiverConfig.spriteId = "martillo";
	int hammerNumber = 1;
	createWeaponGiver(hammerGiverConfig, hammerNumber);

	Config chainsawGiverConfig{};
	chainsawGiverConfig.pos = Vector2D(sdlutils().width() / 2.5f, sdlutils().height() - 100);
	chainsawGiverConfig.vel = Vector2D();
	chainsawGiverConfig.size = Vector2D(50, 50);
	chainsawGiverConfig.friction = 0.2f;
	chainsawGiverConfig.physicType = STATIC;
	chainsawGiverConfig.fixedRotation = true;
	chainsawGiverConfig.rotation = 0.0f;
	chainsawGiverConfig.spriteId = "sierra";
	int chainsawNumber = 2;
	createWeaponGiver(chainsawGiverConfig, chainsawNumber);

	Config crabGiverConfig{};
	crabGiverConfig.pos = Vector2D(sdlutils().width() / 2.0f, sdlutils().height() - 100);
	crabGiverConfig.vel = Vector2D();
	crabGiverConfig.size = Vector2D(50, 50);
	crabGiverConfig.friction = 0.2f;
	crabGiverConfig.physicType = STATIC;
	crabGiverConfig.fixedRotation = true;
	crabGiverConfig.rotation = 0.0f;
	crabGiverConfig.spriteId = "crab";
	int crabNumber = 3;
	createWeaponGiver(crabGiverConfig, crabNumber);

	Config elfShark{};
	elfShark.pos = Vector2D(sdlutils().width() * 1.6f, sdlutils().height() * 0.3f);
	elfShark.vel = Vector2D(0, 0);
	elfShark.size = Vector2D(180.0f, 180.0f);
	elfShark.friction = 0.2f;
	elfShark.physicType = KINEMATIC;
	elfShark.fixedRotation = true;
	elfShark.rotation = 0.0f;
	elfShark.col = ENEMY;
	elfShark.colMask = ENEMY_MASK;
	createElfShark(elfShark);

	//Config flowerJellyHat{};
	//flowerJellyHat.pos = Vector2D(sdlutils().width() * 0.8f, sdlutils().height() * 0.8f);
	//flowerJellyHat.vel = Vector2D(0, 0);
	//flowerJellyHat.size = Vector2D(100.0f, 100.0f);
	//flowerJellyHat.friction = 0.2f;
	//flowerJellyHat.physicType = DYNAMIC;
	//flowerJellyHat.fixedRotation = true;
	//flowerJellyHat.rotation = 0.0f;
	//flowerJellyHat.col = ENEMY;
	//flowerJellyHat.colMask = ENEMY_MASK;
	//createFlowerJellyHat(flowerJellyHat);
}

void PlayState::update(){
	//Queremos que, en el estado que hayan objetos físicos, se actualicen las físicas
	manager_->getWorld()->Step(1.0f / 60.0f, 6, 2);
	if (ih().keyDownEvent()) {
		if (ih().isKeyDown(SDLK_p)) {
			manager_->getSoundMngr()->playPauseMusic();
			StateMachine* sM = app->getStateMachine();
			sM->pushState(new PauseState(this, app, sM->currentState()->getMngr()->getWorld(), sM->currentState()->getMngr()->getSoundMngr()));
		}
	}
	GameState::update();

}

/// <summary>
/// Crea el tile nivel 0 con f�sicas
/// </summary>
void PlayState::createLevel0() {
	auto* nivel = manager_->addEntity();
	Level0* levelTile = nivel->addComponent<Level0>(MAP_PATH, manager_->getWorld());
	map = nivel->addComponent<MapProcedural>(2, 0);
	getMngr()->setHandler<Map>(nivel);
	camLimits = levelTile->getMaxCoordenates();
}

void PlayState::createPlayer(const Config& playerConfig){
	auto* player = createBasicEntity(playerConfig.pos, playerConfig.size, playerConfig.rotation, playerConfig.vel);
	//AVISO: Cada spriteSheet tiene su punto de anclaje
	//En cada animacion hay que colocar un punto de anclaje. Si no se determina, dicho punto de anclaje sera el centro del Sprite
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
	anim_controller->addAnimation("chainsaw_attack1", &sdlutils().images().at("chainsaw_combo"), 6, 8, 47, 24, 0, 1, 8, Vector2D(0.75, 0.72));
	anim_controller->addAnimation("chainsaw_attack2", &sdlutils().images().at("chainsaw_combo"), 6, 8, 47, 24, 0, 9, 18, Vector2D(0.75, 0.72));
	anim_controller->addAnimation("chainsaw_attack3", &sdlutils().images().at("chainsaw_combo"), 6, 8, 47, 24, -1, 19, 46, Vector2D(0.75, 0.72)); // provisional, habria que hacer una de mantener

	anim_controller->addTransition("run", "chainsaw_attack1", "chainsaw_att", 1, false);
	anim_controller->addTransition("jump", "chainsaw_attack1", "chainsaw_att", 1, false);
	anim_controller->addTransition("idle", "chainsaw_attack1", "chainsaw_att", 1, false);
	anim_controller->addTransition("chainsaw_attack1", "run", "chainsaw_att", 0, true);
	anim_controller->addTransition("chainsaw_attack1", "chainsaw_attack2", "chainsaw_att", 2, true);
	anim_controller->addTransition("run", "chainsaw_attack2", "chainsaw_att", 2, false);
	anim_controller->addTransition("jump", "chainsaw_attack2", "chainsaw_att", 2, false);
	anim_controller->addTransition("idle", "chainsaw_attack2", "chainsaw_att", 2, false);
	anim_controller->addTransition("chainsaw_attack2", "run", "chainsaw_att", 0, true);
	anim_controller->addTransition("chainsaw_attack2", "chainsaw_attack3", "chainsaw_att", 3, true);
	anim_controller->addTransition("run", "chainsaw_attack3", "chainsaw_att", 3, false);
	anim_controller->addTransition("jump", "chainsaw_attack3", "chainsaw_att", 3, false);
	anim_controller->addTransition("idle", "chainsaw_attack3", "chainsaw_att", 3, false);
	anim_controller->addTransition("chainsaw_attack3", "run", "chainsaw_att", 0, false);
	anim_controller->addTransition("chainsaw_attack3", "chainsaw_attack1", "chainsaw_att", 4, true);

	anim_controller->setParamValue("chainsaw_att", 0);
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion

#pragma region Sword
	//---SWORD------------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("sword_attack1", &sdlutils().images().at("sword_combo"), 6, 8, 48, 24, 0, 1, 9);
	anim_controller->addAnimation("sword_attack2", &sdlutils().images().at("sword_combo"), 6, 8, 48, 24, 0, 12, 20);
	anim_controller->addAnimation("sword_attack3", &sdlutils().images().at("sword_combo"), 6, 8, 48, 24, -1, 27, 44);

	anim_controller->addTransition("run", "sword_attack1", "sword_att", 1, false);
	anim_controller->addTransition("jump", "sword_attack1", "sword_att", 1, false);
	anim_controller->addTransition("idle", "sword_attack1", "sword_att", 1, false);
	anim_controller->addTransition("sword_attack1", "run", "sword_att", 0, true);
	anim_controller->addTransition("sword_attack1", "sword_attack2", "sword_att", 2, true);
	anim_controller->addTransition("run", "sword_attack2", "sword_att", 2, false);
	anim_controller->addTransition("jump", "sword_attack2", "sword_att", 2, false);
	anim_controller->addTransition("idle", "sword_attack2", "sword_att", 2, false);
	anim_controller->addTransition("sword_attack2", "run", "sword_att", 0, true);
	anim_controller->addTransition("sword_attack2", "sword_attack3", "sword_att", 3, true);
	anim_controller->addTransition("run", "sword_attack3", "sword_att", 3, false);
	anim_controller->addTransition("jump", "sword_attack3", "sword_att", 3, false);
	anim_controller->addTransition("idle", "sword_attack3", "sword_att", 3, false);
	anim_controller->addTransition("sword_attack3", "run", "sword_att", 0, false);
	anim_controller->addTransition("sword_attack3", "sword_attack1", "sword_att", 4, true);

	anim_controller->setParamValue("sword_att", 0);
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion

#pragma region Hammer
//---HAMMER---------------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("hammer_attack1", &sdlutils().images().at("hammer_combo"), 5, 7, 31, 24, 0, 1, 16);
	anim_controller->addAnimation("hammer_attack2", &sdlutils().images().at("hammer_combo"), 5, 7, 31, 24, 0, 17, 30);

	anim_controller->addTransition("run", "hammer_attack1", "hammer_att", 1, false);
	anim_controller->addTransition("jump", "hammer_attack1", "hammer_att", 1, false);
	anim_controller->addTransition("idle", "hammer_attack1", "hammer_att", 1, false);
	anim_controller->addTransition("hammer_attack1", "run", "hammer_att", 0, false);
	anim_controller->addTransition("hammer_attack1", "hammer_attack2", "hammer_att", 2, true);
	anim_controller->addTransition("run", "hammer_attack2", "hammer_att", 2, false);
	anim_controller->addTransition("jump", "hammer_attack2", "hammer_att", 2, false);
	anim_controller->addTransition("idle", "hammer_attack2", "hammer_att", 2, false);
	anim_controller->addTransition("hammer_attack2", "run", "hammer_att", 0, false);
	anim_controller->addTransition("hammer_attack2", "hammer_attack1", "hammer_att", 1, false);

	anim_controller->setParamValue("hammer_att", 0);
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion

#pragma region Crab
//---CRAB---------------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("crab_attack1", &sdlutils().images().at("crab_combo"), 6, 7, 39, 24, 0, 1, 8);
	anim_controller->addAnimation("crab_attack2", &sdlutils().images().at("crab_combo"), 6, 7, 39, 24, -1, 8, 25);
	anim_controller->addAnimation("crab_attack3", &sdlutils().images().at("crab_combo"), 6, 7, 39, 24, 0, 27, 38);

	anim_controller->addTransition("run", "crab_attack1", "crab_att", 1, false);
	anim_controller->addTransition("jump", "crab_attack1", "crab_att", 1, false);
	anim_controller->addTransition("idle", "crab_attack1", "crab_att", 1, false);
	anim_controller->addTransition("crab_attack1", "run", "crab_att", 0, true);
	anim_controller->addTransition("crab_attack1", "crab_attack2", "crab_att", 2, true);
	anim_controller->addTransition("run", "crab_attack2", "crab_att", 2, false);
	anim_controller->addTransition("jump", "crab_attack2", "crab_att", 2, false);
	anim_controller->addTransition("idle", "crab_attack2", "crab_att", 2, false);
	anim_controller->addTransition("crab_attack2", "run", "crab_att", 0, false);
	anim_controller->addTransition("crab_attack2", "crab_attack3", "crab_att", 3, false);
	anim_controller->addTransition("run", "crab_attack3", "crab_att", 3, false);
	anim_controller->addTransition("jump", "crab_attack3", "crab_att", 3, false);
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
	//player->addComponent<SoundManager>(75, "FinalBoss");

	if(playerConfig.physicType != KINEMATIC) player->addComponent<PlayerController>();
	else player->addComponent<KeyBoardCtrl>(map);

	player->addComponent<CameraFollow>(player->getComponent<Transform>(), &(manager_->getHandler<Map>()->getComponent<Level0>()->getMaxCoordenates()), Vector2D(250.0f, -300.0f), 0.06f, app->getCameraZooOut(), false, false); //Vector2D offset y porcentaje de la velocidad de la camara, mas bajo mas lento sigue
	player->addComponent<Inventory>();

	player->addComponent<LoseLife>();

	//Seteamos al Player como MainHandler
	manager_->setHandler<Player>(player);
}

void PlayState::createWeaponGiver(const Config& weaponGiverConfig, const int& weaponType) {
	auto* weaponGiver = createBasicEntity(weaponGiverConfig.pos, weaponGiverConfig.size, weaponGiverConfig.rotation, weaponGiverConfig.vel);

	auto* anim_controller = weaponGiver->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("idle", &sdlutils().images().at(weaponGiverConfig.spriteId), 1, 1, 1, 24, 1);
	weaponGiver->addComponent<BoxCollider>(weaponGiverConfig.physicType, PLAYER_DETECTION, PLAYER_DETECTION_MASK, true);
	weaponGiver->addComponent<GetAWeaponOnCollision>(weaponType);
}

/// <summary>
/// Crea una caja roja que posee f�sicas y controlador de teclado
/// </summary>
/// <param name="pos">Posici�n</param>
/// <param name="vel">Velocidad (por defecto es cero)</param>
/// <param name="height">Altura en pixeles</param>
/// <param name="width">Anchura en pixeles</param>
/// <param name="rotation">Rotacion (por defecto es cero)</param>
/// <param name="physicType">Determina el tipo f�sico del objeto (STATIC, DYNAMIC, KINEMATIC)</param>
void PlayState::createBoxTest(const Config& entityConfig){
	auto* box = createBasicEntity(entityConfig.pos, entityConfig.size, entityConfig.rotation, entityConfig.vel);

	auto* anim_controller = box->addComponent<AnimBlendGraph>();

	box->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask, entityConfig.isTrigger, entityConfig.friction, entityConfig.fixedRotation, entityConfig.rotation);
	//box->addComponent<CameraFollow>(box->getComponent<Transform>());

	if (entityConfig.physicType == 1 || entityConfig.physicType == 2)
		box->addComponent<KeyBoardCtrl>(map);
}

void PlayState::createElfShark(const Config& entityConfig){
#pragma region ElfShark
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
#pragma endregion
}

void PlayState::createFlowerJellyHat(const Config& entityConfig) {
	auto* fjh1 = createBasicEntity(entityConfig.pos, entityConfig.size, entityConfig.rotation, entityConfig.vel);
	AnimBlendGraph* fjh1_anim_controller = fjh1->addComponent<AnimBlendGraph>();
	fjh1_anim_controller->addAnimation("idle", &sdlutils().images().at("Medusa"), 7, 6, 38, 8, -1);
	fjh1->addComponent<Enemy_Health>(300, Vector2D(300, 20), build_sdlcolor(255, 0, 0, 255), 50);
	fjh1->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask);
	fjh1->addComponent<JellyHatBehavior>(fjh1);
}

