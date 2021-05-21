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
	auto* bg = createBasicEntity(Vector2D(400, sdlutils().height() * 1.45f),Vector2D(3000,1080),0.0f,Vector2D(0,0));
	//bg->addComponent<Animation>("1", &sdlutils().images().at("sky"), 1, 1, 1, 1, 0);
	auto* bgParallax = bg->addComponent<ParallaxScroll>();

	//--EJEMPLO PARA USAR GRUPOS DE CAPAS---------
	//std::vector<std::pair<Texture*, float>> groupLayer;
	//groupLayer.push_back({ &sdlutils().images().at("bg_layer1"), 0.2 });
	//groupLayer.push_back({ &sdlutils().images().at("bg_layer2"), 0.25});
	//groupLayer.push_back({ &sdlutils().images().at("bg_layer3"), 0.35});
	//groupLayer.push_back({ &sdlutils().images().at("bg_layer4"), 0.40});
	//bgParallax->addGroupLayer(groupLayer);

	//-----Player-----
	#pragma region Player
	Config playerConfig{};
	playerConfig.pos = Vector2D(100, sdlutils().height() * 1.5f);
	playerConfig.vel = Vector2D(0, 0);
	playerConfig.size = Vector2D(100.0f, 100.0f);
	playerConfig.friction = 0.0f;
	playerConfig.physicType = DYNAMIC;
	playerConfig.fixedRotation = true;
	playerConfig.rotation = 0.0f;
	playerConfig.col = PLAYER;
	playerConfig.colMask = PLAYER_MASK;
	createPlayer(playerConfig);
#pragma endregion

	//---SUELO---
	Config floor{};
	floor.pos = Vector2D(200, sdlutils().height() * 2.0f);
	floor.vel = Vector2D(0, 0);
	floor.size = Vector2D(sdlutils().width() * 4.0f, 100.0f);
	floor.friction = 0.2f;
	floor.physicType = STATIC;
	floor.fixedRotation = true;
	floor.rotation = 0.0f;
	floor.col = GROUND;
	floor.colMask = GROUND_MASK;
	createBoxFloor(floor);

	Config floor2{};
	floor2.pos = Vector2D(0, sdlutils().height() * 2.0f - 350);
	floor2.vel = Vector2D(0, 0);
	floor2.size = Vector2D(400, 100.0f);
	floor2.friction = 0.2f;
	floor2.physicType = STATIC;
	floor2.fixedRotation = true;
	floor2.rotation = 0.0f;
	floor2.col = GROUND;
	floor2.colMask = GROUND_MASK;
	createBoxFloor(floor2);

#pragma region TestJoint

	//Config floor3{};
	//floor3.pos = Vector2D(-900, sdlutils().height() * 2.0f - 550);
	//floor3.vel = Vector2D(0, 0);
	//floor3.size = Vector2D(400, 100.0f);
	//floor3.friction = 0.2f;
	//floor3.physicType = STATIC;
	//floor3.fixedRotation = true;
	//floor3.rotation = 0.0f;
	//floor3.col = GROUND;
	//floor3.colMask = GROUND_MASK;
	//auto* father = createBoxFloor(floor3);
	//
	//Config floor4{};
	//floor4.pos = Vector2D(-900, sdlutils().height() * 2.0f - 550 + 50.0f);
	//floor4.vel = Vector2D(0, 0);
	//floor4.size = Vector2D(50.0f, 50.0f);
	//floor4.friction = 0.2f;
	//floor4.physicType = DYNAMIC;
	//floor4.fixedRotation = true;
	//floor4.rotation = 0.0f;
	//floor4.col = GROUND;
	//floor4.colMask = GROUND_MASK;
	//auto* child = createBoxFloor(floor4);


	//auto* bodyA = father->getComponent<BoxCollider>()->getBody();
	//auto* bodyB = child->getComponent<BoxCollider>()->getBody();


	//b2RevoluteJointDef* b2joint = new b2RevoluteJointDef();
	//b2joint->Initialize(bodyA, bodyB, bodyB->GetWorldCenter());
	//b2joint->motorSpeed = 6;
	//b2joint->maxMotorTorque = 1000.0f;
	//b2joint->enableMotor = true;
	//b2joint->collideConnected = true;
	///*b2joint->localAnchorA.Set(1, 0);
	//b2joint->localAnchorB.Set(2, 0);*/
	//manager_->getWorld()->CreateJoint(b2joint);
#pragma endregion



#pragma endregion

//-----Enemies-----
#pragma region Enemies
	/*#pragma region PompeyWorm
	Config pompeyWorm{};
	pompeyWorm.pos = Vector2D(700, sdlutils().height() * 2.0f - 200);
	pompeyWorm.vel = Vector2D(0, 0);
	pompeyWorm.size = Vector2D(100.0f, 60.0f);
	pompeyWorm.friction = 0.7;
	pompeyWorm.physicType = DYNAMIC;
	pompeyWorm.fixedRotation = true;
	pompeyWorm.rotation = 0.0f;
	pompeyWorm.col = ENEMY;
	pompeyWorm.colMask = ENEMY_MASK;
	createPompeyWorm(pompeyWorm);
	#pragma endregion*/

	/*#pragma region ElfShark
	Config elfShark{};
	elfShark.pos = Vector2D(sdlutils().width() * 1.6f, sdlutils().height() * 0.3f);
	elfShark.vel = Vector2D(0, 0);
	elfShark.size = Vector2D(240.0f, 200.0f);
	elfShark.friction = 0.7f;
	elfShark.physicType = DYNAMIC;
	elfShark.fixedRotation = true;
	elfShark.rotation = 0.0f;
	elfShark.col = ENEMY;
	elfShark.colMask = ENEMY_MASK;
	createElfShark(elfShark);
	#pragma endregion*/

	/*#pragma region FringeHead
	Config fringeHead{};
	fringeHead.pos = Vector2D(300, sdlutils().height() * 1.7f);
	fringeHead.vel = Vector2D(0, 0);
	fringeHead.size = Vector2D(60.0f,60.0f);
	fringeHead.friction = 0.7;
	fringeHead.physicType = DYNAMIC;
	fringeHead.fixedRotation = true;
	fringeHead.rotation = 0.0f;
	fringeHead.col = ENEMY;
	fringeHead.colMask = ENEMY_MASK;
	createFringeHead(fringeHead);
	#pragma endregion*/
	#pragma region ThornFish
	Config thornfish{};
	thornfish.pos = Vector2D(300, sdlutils().height() * 1.7f);
	thornfish.vel = Vector2D(0, 0);
	thornfish.size = Vector2D(180.0f, 120.0f);
	thornfish.friction = 0.7;
	thornfish.physicType = DYNAMIC;
	thornfish.fixedRotation = true;
	thornfish.rotation = 0.0f;
	thornfish.col = ENEMY;
	thornfish.colMask = ENEMY_MASK;
	createThornFish(thornfish);

	#pragma endregion
//#pragma endregion
//
//#pragma region Weapons
//	Config swordGiverConfig{};
//	swordGiverConfig.pos = Vector2D(sdlutils().width() / 2.5f, sdlutils().height() * 2.0f);
//	swordGiverConfig.vel = Vector2D();
//	swordGiverConfig.size = Vector2D(50, 50);
//	swordGiverConfig.friction = 0.2f;
//	swordGiverConfig.physicType = STATIC;
//	swordGiverConfig.fixedRotation = true;
//	swordGiverConfig.rotation = 0.0f;
//	swordGiverConfig.spriteId = "espada";
//	int swordNumber = 0;
//	createWeaponGiver(swordGiverConfig, swordNumber);
//
//	Config hammerGiverConfig{};
//	hammerGiverConfig.pos = Vector2D(sdlutils().width() / 2.0f, sdlutils().height() * 2.0f);
//	hammerGiverConfig.vel = Vector2D();
//	hammerGiverConfig.size = Vector2D(50, 50);
//	hammerGiverConfig.friction = 0.2f;
//	hammerGiverConfig.physicType = STATIC;
//	hammerGiverConfig.fixedRotation = true;
//	hammerGiverConfig.rotation = 0.0f;
//	hammerGiverConfig.spriteId = "martillo";
//	int hammerNumber = 1;
//	createWeaponGiver(hammerGiverConfig, hammerNumber);
//
//	Config chainsawGiverConfig{};
//	chainsawGiverConfig.pos = Vector2D(sdlutils().width() / 2.5f, sdlutils().height() * 2.0f - 100);
//	chainsawGiverConfig.vel = Vector2D();
//	chainsawGiverConfig.size = Vector2D(50, 50);
//	chainsawGiverConfig.friction = 0.2f;
//	chainsawGiverConfig.physicType = STATIC;
//	chainsawGiverConfig.fixedRotation = true;
//	chainsawGiverConfig.rotation = 0.0f;
//	chainsawGiverConfig.spriteId = "sierra";
//	int chainsawNumber = 2;
//	createWeaponGiver(chainsawGiverConfig, chainsawNumber);
//
//	Config crabGiverConfig{};
//	crabGiverConfig.pos = Vector2D(sdlutils().width() / 2.0f, sdlutils().height() * 2.0f - 100);
//	crabGiverConfig.vel = Vector2D();
//	crabGiverConfig.size = Vector2D(50, 50);
//	crabGiverConfig.friction = 0.2f;
//	crabGiverConfig.physicType = STATIC;
//	crabGiverConfig.fixedRotation = true;
//	crabGiverConfig.rotation = 0.0f;
//	crabGiverConfig.spriteId = "crab";
//	int crabNumber = 3;
//	createWeaponGiver(crabGiverConfig, crabNumber);
//
//	Config machineGunGiverConfig{};
//	machineGunGiverConfig.pos = Vector2D(sdlutils().width() / 1.5f, sdlutils().height() * 2.0f);
//	machineGunGiverConfig.vel = Vector2D();
//	machineGunGiverConfig.size = Vector2D(50, 50);
//	machineGunGiverConfig.friction = 0.2f;
//	machineGunGiverConfig.physicType = STATIC;
//	machineGunGiverConfig.fixedRotation = true;
//	machineGunGiverConfig.rotation = 0.0f;
//	machineGunGiverConfig.spriteId = "machine_gun";
//	int machineGunNumber = 4;
//	createWeaponGiver(machineGunGiverConfig, machineGunNumber);
//#pragma endregion
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
	anim_controller->addAnimation("jump", &sdlutils().images().at("player_jump"), 4, 6, 24, 36, 0);
	anim_controller->addAnimation("dash_ground", &sdlutils().images().at("player_dash_ground"), 3, 4, 12, 60, 0, 0, 11, Vector2D(0.5,0.2));
	anim_controller->addAnimation("dash_air", &sdlutils().images().at("player_dash_air"), 3, 4, 12, 60, 0, 0, 11, Vector2D(0.5,0.5));
	anim_controller->addAnimation("death", &sdlutils().images().at("player_death"), 6, 8, 48, 20, 0, 0, 47, Vector2D(0.5, 0.7));
	//Proportion?
	anim_controller->keepProportion("idle", Vector2D(player->getComponent<Transform>()->getW(), player->getComponent<Transform>()->getH()));
	//Transitions 
	anim_controller->addTransition("idle", "run", "Speed", 1, false);
	anim_controller->addTransition("run", "idle", "Speed", 0, false);
	anim_controller->addTransition("run", "jump", "NotOnFloor", 1, false);	//Anim fuente, anim destino, parametro, valor de parametro, esperar a que termine la animacion
	anim_controller->addTransition("jump", "run", "NotOnFloor", 0, false);
	anim_controller->addTransition("idle", "jump", "NotOnFloor", 1, false);
	anim_controller->addTransition("jump", "idle", "NotOnFloor", 0, true);
#pragma endregion
#pragma region dash&Death
	//Transitions
	anim_controller->addTransition("jump", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("dash_air", "jump", "Dash_Air", 0, true);
	anim_controller->addTransition("run", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_ground", "run", "Dash_Ground", 0, true);
	anim_controller->addTransition("idle", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_ground", "idle", "Dash_Ground", 0, true);
	//death
	anim_controller->addTransition("death", "idle", "Dead", 23, false);
	anim_controller->addTransition("idle", "death", "Dead", 1, false);
	anim_controller->addTransition("run", "death", "Dead", 1, false);
	anim_controller->addTransition("jump", "death", "Dead", 1, false);
	anim_controller->addTransition("dash_ground", "death", "Dead", 1, false);
	anim_controller->addTransition("dash_air", "death", "Dead", 1, false);
#pragma endregion
#pragma region parametros
	//--Parametros--
	anim_controller->setParamValue("NotOnFloor", 0);	//AVISO: Si no existe el parametro, no hara nada
	anim_controller->setParamValue("Speed", 0);
	anim_controller->setParamValue("Dash_Air", 0);
	anim_controller->setParamValue("Dash_Ground", 0);
	anim_controller->setParamValue("Dead", 0);
#pragma endregion
	//--------------------------------------------------------------------------------------------------------------
#pragma region Weapons
	//-WEAPONS------------------------------------------------------------------------------------------------------
#pragma region Chainsaw
	//---CHAINSAW---------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("chainsaw_attack1", &sdlutils().images().at("chainsaw_combo"), 6, 8, 48, 15, 0, 1, 8, Vector2D(0.75, 0.72));
	anim_controller->addAnimation("chainsaw_attack2", &sdlutils().images().at("chainsaw_combo"), 6, 8, 48, 15, 0, 9, 18, Vector2D(0.75, 0.72));
	anim_controller->addAnimation("chainsaw_attack3", &sdlutils().images().at("chainsaw_combo"), 6, 8, 48, 28, 0, 19, 27, Vector2D(0.75, 0.72));
	anim_controller->addAnimation("chainsaw_attack4", &sdlutils().images().at("chainsaw_combo"), 6, 8, 48, 24, -1, 28, 47, Vector2D(0.75, 0.72));

	anim_controller->addTransition("run", "chainsaw_attack1", "chainsaw_att", 1, false);
	anim_controller->addTransition("idle", "chainsaw_attack1", "chainsaw_att", 1, false);
	anim_controller->addTransition("jump", "chainsaw_attack1", "chainsaw_att", 1, false);
	anim_controller->addTransition("chainsaw_attack1", "run", "chainsaw_att", 0, true);
	anim_controller->addTransition("chainsaw_attack1", "chainsaw_attack2", "chainsaw_att", 2, true);
	anim_controller->addTransition("run", "chainsaw_attack2", "chainsaw_att", 2, false);
	anim_controller->addTransition("idle", "chainsaw_attack2", "chainsaw_att", 2, false);
	anim_controller->addTransition("jump", "chainsaw_attack2", "chainsaw_att", 2, false);
	anim_controller->addTransition("chainsaw_attack2", "run", "chainsaw_att", 0, true);
	anim_controller->addTransition("chainsaw_attack2", "chainsaw_attack3", "chainsaw_att", 3, true);
	anim_controller->addTransition("run", "chainsaw_attack3", "chainsaw_att", 3, false);
	anim_controller->addTransition("idle", "chainsaw_attack3", "chainsaw_att", 3, false);
	anim_controller->addTransition("jump", "chainsaw_attack3", "chainsaw_att", 3, false);
	anim_controller->addTransition("chainsaw_attack3", "chainsaw_attack4", "chainsaw_att", 4, true);
	anim_controller->addTransition("chainsaw_attack4", "run", "chainsaw_att", 0, false);
	anim_controller->addTransition("chainsaw_attack4", "chainsaw_attack1", "chainsaw_att", 1, false);
	//death
	anim_controller->addTransition("chainsaw_attack1", "death", "Dead", 1, false);
	anim_controller->addTransition("chainsaw_attack2", "death", "Dead", 1, false);
	anim_controller->addTransition("chainsaw_attack3", "death", "Dead", 1, false);
	anim_controller->addTransition("chainsaw_attack4", "death", "Dead", 1, false);

	anim_controller->setParamValue("chainsaw_att", 0);
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion

#pragma region Sword
	//---SWORD------------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("sword_attack1", &sdlutils().images().at("sword_combo"), 6, 8, 48, 48, 0, 1, 9, Vector2D(0.71, 0.69));
	anim_controller->addAnimation("sword_attack2", &sdlutils().images().at("sword_combo"), 6, 8, 48, 48, 0, 12, 20, Vector2D(0.71, 0.69));
	anim_controller->addAnimation("sword_attack3", &sdlutils().images().at("sword_combo"), 6, 8, 48, 48, 0, 20, 28, Vector2D(0.71, 0.69));
	anim_controller->addAnimation("sword_attack4", &sdlutils().images().at("sword_combo"), 6, 8, 48, 48, -1, 28, 44, Vector2D(0.71, 0.69));

	anim_controller->addTransition("run", "sword_attack1", "sword_att", 1, false);
	anim_controller->addTransition("idle", "sword_attack1", "sword_att", 1, false);
	anim_controller->addTransition("jump", "sword_attack1", "sword_att", 1, false);
	anim_controller->addTransition("sword_attack1", "run", "sword_att", 0, true);
	anim_controller->addTransition("sword_attack1", "sword_attack2", "sword_att", 2, true);
	anim_controller->addTransition("run", "sword_attack2", "sword_att", 2, false);
	anim_controller->addTransition("idle", "sword_attack2", "sword_att", 2, false);
	anim_controller->addTransition("jump", "sword_attack2", "sword_att", 2, false);
	anim_controller->addTransition("sword_attack2", "run", "sword_att", 0, true);
	anim_controller->addTransition("sword_attack2", "sword_attack3", "sword_att", 3, true);
	anim_controller->addTransition("run", "sword_attack3", "sword_att", 3, false);
	anim_controller->addTransition("idle", "sword_attack3", "sword_att", 3, false);
	anim_controller->addTransition("jump", "sword_attack3", "sword_att", 3, false);
	anim_controller->addTransition("sword_attack3", "sword_attack4", "sword_att", 4, true);
	anim_controller->addTransition("sword_attack4", "run", "sword_att", 0, false);
	anim_controller->addTransition("sword_attack4", "sword_attack1", "sword_att", 1, false);
	//death
	anim_controller->addTransition("sword_attack1", "death", "Dead", 1, false);
	anim_controller->addTransition("sword_attack2", "death", "Dead", 1, false);
	anim_controller->addTransition("sword_attack4", "death", "Dead", 1, false);
	anim_controller->addTransition("sword_attack4", "death", "Dead", 1, false);

	anim_controller->setParamValue("sword_att", 0);
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion

#pragma region Hammer
//---HAMMER---------------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("hammer_attack1", &sdlutils().images().at("hammer_combo"), 5, 7, 31, 60, 0, 1, 16, Vector2D(0.75, 0.69));
	anim_controller->addAnimation("hammer_attack2", &sdlutils().images().at("hammer_combo"), 5, 7, 31, 60, 0, 17, 30, Vector2D(0.75, 0.69));

	anim_controller->addTransition("run", "hammer_attack1", "hammer_att", 1, false);
	anim_controller->addTransition("idle", "hammer_attack1", "hammer_att", 1, false);
	anim_controller->addTransition("jump", "hammer_attack1", "hammer_att", 1, false);
	anim_controller->addTransition("hammer_attack1", "run", "hammer_att", 0, false);
	anim_controller->addTransition("hammer_attack1", "hammer_attack2", "hammer_att", 2, true);
	anim_controller->addTransition("run", "hammer_attack2", "hammer_att", 2, false);
	anim_controller->addTransition("idle", "hammer_attack2", "hammer_att", 2, false);
	anim_controller->addTransition("jump", "hammer_attack2", "hammer_att", 2, false);
	anim_controller->addTransition("hammer_attack2", "run", "hammer_att", 0, false);
	anim_controller->addTransition("hammer_attack2", "hammer_attack1", "hammer_att", 1, false);
	//death
	anim_controller->addTransition("hammer_attack1", "death", "Dead", 1, false);
	anim_controller->addTransition("hammer_attack2", "death", "Dead", 1, false);

	anim_controller->setParamValue("hammer_att", 0);
	//	//--------------------------------------------------------------------------------------------------------------
	//#pragma endregion
	//
	//#pragma region Crab
	////---CRAB---------------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("crab_attack1", &sdlutils().images().at("crab_combo"), 6, 7, 39, 35, 0, 1, 8, Vector2D(0.71, 0.5));
	anim_controller->addAnimation("crab_attack2", &sdlutils().images().at("crab_combo"), 6, 7, 39, 48, 0, 8, 25, Vector2D(0.71, 0.5));
	anim_controller->addAnimation("crab_attack3", &sdlutils().images().at("crab_combo"), 6, 7, 39, 48, 0, 27, 38, Vector2D(0.71, 0.5));

	anim_controller->addTransition("run", "crab_attack1", "crab_att", 1, false);
	anim_controller->addTransition("idle", "crab_attack1", "crab_att", 1, false);
	anim_controller->addTransition("jump", "crab_attack1", "crab_att", 1, false);
	anim_controller->addTransition("crab_attack1", "run", "crab_att", 0, true);
	anim_controller->addTransition("crab_attack1", "crab_attack2", "crab_att", 2, true);
	anim_controller->addTransition("run", "crab_attack2", "crab_att", 2, false);
	anim_controller->addTransition("idle", "crab_attack2", "crab_att", 2, false);
	anim_controller->addTransition("jump", "crab_attack2", "crab_att", 2, false);
	anim_controller->addTransition("crab_attack2", "run", "crab_att", 0, false);
	anim_controller->addTransition("crab_attack2", "crab_attack3", "crab_att", 3, false);
	anim_controller->addTransition("run", "crab_attack3", "crab_att", 3, false);
	anim_controller->addTransition("idle", "crab_attack3", "crab_att", 3, false);
	anim_controller->addTransition("jump", "crab_attack3", "crab_att", 3, false);
	anim_controller->addTransition("crab_attack3", "run", "crab_att", 0, false);
	anim_controller->addTransition("crab_attack3", "crab_attack1", "crab_att", 1, true);
	//death
	anim_controller->addTransition("crab_attack1", "death", "Dead", 1, false);
	anim_controller->addTransition("crab_attack2", "death", "Dead", 1, false);
	anim_controller->addTransition("crab_attack3", "death", "Dead", 1, false);

	anim_controller->setParamValue("crab_att", 0);
	//	//--------------------------------------------------------------------------------------------------------------
	//#pragma endregion
	//
	//#pragma region MachineGun
	////---CRAB---------------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("machine_gun1", &sdlutils().images().at("machineGun_combo"), 5, 6, 29, 48, -1, 1, 7, Vector2D(0.65, 0.5));
	anim_controller->addAnimation("machine_gun2", &sdlutils().images().at("machineGun_combo"), 5, 6, 29, 48, 0, 8, 27, Vector2D(0.65, 0.5));

	anim_controller->addTransition("run", "machine_gun1", "machineGun_att", 1, false);
	anim_controller->addTransition("idle", "machine_gun1", "machineGun_att", 1, false);
	anim_controller->addTransition("jump", "machine_gun1", "machineGun_att", 1, false);
	anim_controller->addTransition("machine_gun1", "run", "machineGun_att", 0, false);
	anim_controller->addTransition("machine_gun1", "machine_gun2", "machineGun_att", 2, false);
	anim_controller->addTransition("machine_gun2", "run", "machineGun_att", 0, false);
	anim_controller->addTransition("machine_gun2", "idle", "machineGun_att", 0, false);
	anim_controller->addTransition("machine_gun2", "machine_gun1", "machineGun_att", 1, false);
	//death
	anim_controller->addTransition("machine_gun1", "death", "Dead", 1, false);
	anim_controller->addTransition("machine_gun2", "death", "Dead", 1, false);

	anim_controller->setParamValue("machineGun_att", 0);
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion
#pragma endregion

	player->addComponent<BoxCollider>(playerConfig.physicType, PLAYER, PLAYER_MASK, false,
		playerConfig.friction, playerConfig.fixedRotation, playerConfig.rotation, Vector2D(playerConfig.size.getX() * 0.6, playerConfig.size.getY()*0.95));
	player->addComponent<TriggerCollider>("Feet", PLAYER_JUMP, PLAYER_JUMP_MASK, Vector2D(0, 0.28), Vector2D(50.0f, 10.0f));
	player->addComponent<Player_Health>(&sdlutils().images().at("fullvida"), &sdlutils().images().at("mediavida"), &sdlutils().images().at("vacio"), 300.0f, app, 5, 2);
	player->addComponent<Armas_HUD>(app);

	player->addComponent<PlayerController>();

	player->addComponent<CameraFollow>(Vector2D(200.0f, -80.0f), 0.1f, true, true); //Vector2D offset y porcentaje de la velocidad de la camara, mas bajo mas lento sigue

	player->addComponent<Inventory>()->addWeapon(4);

	player->addComponent<LoseLife>();

	//Seteamos al Player como MainHandler
	manager_->setHandler<Player>(player);
}
/// <summary>
/// Crea un suelo Statico
Entity* TestZoneState::createBoxFloor(const Config& entityConfig) {

	auto* box_Floor = createBasicEntity(entityConfig.pos, entityConfig.size, entityConfig.rotation, entityConfig.vel);
	box_Floor->addComponent<Animation>("Floor", &sdlutils().images().at("red_square"), 1, 1, 1, 1, 0);
	box_Floor->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask, false, entityConfig.friction, entityConfig.fixedRotation, entityConfig.rotation);
	return box_Floor;
}
#pragma region PompeyWorm
void TestZoneState::createPompeyWorm(const Config& enemy1Config)
{
	auto* gusano = createBasicEntity(enemy1Config.pos, enemy1Config.size, enemy1Config.rotation, enemy1Config.vel);
	gusano->addComponent<BoxCollider>(enemy1Config.physicType, enemy1Config.col, enemy1Config.colMask, enemy1Config.isTrigger,enemy1Config.friction,
										enemy1Config.fixedRotation, enemy1Config.rotation, Vector2D(), Vector2D(),10000);
	AnimBlendGraph* gusano_anim_controller = gusano->addComponent<AnimBlendGraph>();
	gusano_anim_controller->addAnimation("idle", &sdlutils().images().at("pompey_worm_idle"), 1, 2, 2, 12, -1);
	gusano_anim_controller->addAnimation("move", &sdlutils().images().at("pompey_worm_move"), 1, 2, 2, 12, -1);
	gusano_anim_controller->addAnimation("attack", &sdlutils().images().at("pompey_worm_attack"), 2, 3, 6, 12, 0, 0, 4);
	gusano_anim_controller->addAnimation("death", &sdlutils().images().at("pompey_worm_death"), 2, 3, 6, 12, 0, 0, 4);
	//Proportion?
	gusano_anim_controller->keepProportion("idle", Vector2D(gusano->getComponent<Transform>()->getW(), gusano->getComponent<Transform>()->getH()));
	gusano_anim_controller->addTransition("idle", "move", "Speed", 1, false);
	gusano_anim_controller->addTransition("move", "idle", "Speed", 0, false);
	gusano_anim_controller->addTransition("idle", "attack", "Attack", 1, false);
	gusano_anim_controller->addTransition("attack", "idle", "Attack", 0, true);
	gusano_anim_controller->addTransition("move", "attack", "Attack", 1, false);
	gusano_anim_controller->addTransition("attack", "move", "Attack", 0, true);
	gusano_anim_controller->addTransition("idle", "death", "Dead", 1, false);
	gusano_anim_controller->addTransition("move", "death", "Dead", 1, false);
	gusano_anim_controller->addTransition("attack", "death", "Dead", 1, false);
	gusano_anim_controller->addTransition("death", "idle", "Dead", 99, false);	//Necesario crear una transicion para crear un animstate
	gusano_anim_controller->setParamValue("Dead", 0);
	gusano_anim_controller->setParamValue("Speed", 0);
	gusano_anim_controller->setParamValue("Attack", 0);
	auto* trigger_gusano = gusano->addComponent<EnemyTrigger>(Vector2D(800.0f, 500.0f));
	trigger_gusano->addTriggerComponent<PompeyWormAttack>(gusano);
	gusano->addComponent<Enemy_Health>(300, 20, Vector2D(300, 20), build_sdlcolor(255, 0, 0, 255), 50);
	gusano->addComponent<ContactDamage>();
}
#pragma endregion
#pragma region ElfShark
void TestZoneState::createElfShark(const Config& entityConfig) {
	auto* elf = createBasicEntity(entityConfig.pos, entityConfig.size, entityConfig.rotation, entityConfig.vel);
	elf->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask, false,
		entityConfig.friction, entityConfig.fixedRotation, entityConfig.rotation, Vector2D(entityConfig.size.getX() * 0.5, entityConfig.size.getY() * 0.4),
		Vector2D(entityConfig.pos.getX() * 1.5, entityConfig.pos.getY() * 0.8),35000);
	AnimBlendGraph* elf_anim_controller = elf->addComponent<AnimBlendGraph>();
	elf_anim_controller->addAnimation("idle", &sdlutils().images().at("elfshark_idle"), 1, 2, 2, 12, -1, 0, 1, Vector2D(0.66,0.8));
	elf_anim_controller->addAnimation("move", &sdlutils().images().at("elfshark_move"), 1, 2, 2, 12, -1, 0, 1, Vector2D(0.66, 0.8));
	elf_anim_controller->addAnimation("attack_ini", &sdlutils().images().at("elfshark_attack"), 1, 19, 19, 48, 0, 0, 10, Vector2D(0.5, 0.8));
	elf_anim_controller->addAnimation("attack_end", &sdlutils().images().at("elfshark_attack"), 1, 19, 19, 24, 0, 11, 18, Vector2D(0.5, 0.8));
	elf_anim_controller->addAnimation("death", &sdlutils().images().at("elfshark_death"), 2, 3, 6, 12, 0, 0, 4, Vector2D(0.5, 0.8));
	//Proportion?
	elf_anim_controller->keepProportion("idle", Vector2D(elf->getComponent<Transform>()->getW(), elf->getComponent<Transform>()->getH()));
	elf_anim_controller->addTransition("idle", "move", "Speed", 1, false);
	elf_anim_controller->addTransition("move", "idle", "Speed", 0, false);
	elf_anim_controller->addTransition("idle", "attack_ini", "Attack", 1, false);
	elf_anim_controller->addTransition("attack_end", "idle", "Attack", 0, true);
	elf_anim_controller->addTransition("move", "attack_ini", "Attack", 1, false);
	elf_anim_controller->addTransition("attack_end", "move", "Attack", 0, true);
	elf_anim_controller->addTransition("attack_ini", "move", "Attack", 0, false);
	elf_anim_controller->addTransition("attack_ini", "idle", "Attack", 0, false);
	elf_anim_controller->addTransition("attack_ini", "attack_end", "Attack", 2, true);
	elf_anim_controller->addTransition("idle", "death", "Dead", 1, false);
	elf_anim_controller->addTransition("move", "death", "Dead", 1, false);
	elf_anim_controller->addTransition("attack_ini", "death", "Dead", 1, false);
	elf_anim_controller->addTransition("attack_end", "death", "Dead", 1, false);
	elf_anim_controller->addTransition("death", "idle", "Dead", 99, false);	//Necesario crear una transicion para crear un animstate
	elf_anim_controller->setParamValue("Dead", 0);
	elf_anim_controller->setParamValue("Speed", 0);
	elf_anim_controller->setParamValue("Attack", 0);
	auto* trigger_elf = elf->addComponent<EnemyTrigger>(Vector2D(1000.0f, 600.0f));
	trigger_elf->addTriggerComponent<ElfSharkAttack>(elf);
	elf->addComponent<Enemy_Health>(300, 20, Vector2D(50, 5), build_sdlcolor(255, 0, 0, 255), 50);
	elf->addComponent<ContactDamage>();
}
#pragma endregion
#pragma region JellyHat
void TestZoneState::createFlowerJellyHat(const Config& entityConfig) {
	auto* fjh1 = createBasicEntity(entityConfig.pos, entityConfig.size, entityConfig.rotation, entityConfig.vel);
	AnimBlendGraph* fjh1_anim_controller = fjh1->addComponent<AnimBlendGraph>();
	fjh1_anim_controller->addAnimation("idle", &sdlutils().images().at("Medusa"), 7, 6, 38, 8, -1);
	fjh1_anim_controller->addAnimation("death", &sdlutils().images().at("medusa_death"), 2, 3, 6, 12, 0, 0, 4);
	fjh1_anim_controller->addTransition("idle", "death", "Dead", 1, false);
	fjh1_anim_controller->addTransition("death", "idle", "Dead", 99, false);	//Necesario crear una transicion para crear un animstate
	fjh1_anim_controller->setParamValue("Dead", 0);
	fjh1->addComponent<Enemy_Health>(300, 20, Vector2D(300, 20), build_sdlcolor(255, 0, 0, 255), 50);
	fjh1->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask, false, 0.7, true, 0.0, Vector2D(), Vector2D(),10000);
	fjh1->addComponent<ContactDamage>();
	fjh1->addComponent<JellyHatBehavior>(fjh1);
}
#pragma endregion
#pragma region FringeHead
void TestZoneState::createFringeHead(const Config& entityConfig)
{
	auto* enemy = createBasicEntity(entityConfig.pos, entityConfig.size, entityConfig.rotation, entityConfig.vel);
	BoxCollider* collider = enemy->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask, false, entityConfig.friction,
		entityConfig.fixedRotation, entityConfig.rotation, Vector2D(), Vector2D(), 10000);
	AnimBlendGraph* anim_controller = enemy->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("idle", &sdlutils().images().at("fringehead_idle"), 1, 12, 12, 24, -1);
	anim_controller->addAnimation("attack", &sdlutils().images().at("fringehead_atk"), 1, 13, 13, 24, 0, 0, 12, Vector2D(0.5,0.7));
	anim_controller->addAnimation("death", &sdlutils().images().at("fringehead_death"), 2, 3, 6, 12, 0, 0, 4, Vector2D(0.5, 0.7));
	//Proportion?
	anim_controller->keepProportion("idle", Vector2D(enemy->getComponent<Transform>()->getW(), enemy->getComponent<Transform>()->getH()));
	anim_controller->addTransition("idle", "attack", "Shoot", 1, false);
	anim_controller->addTransition("attack", "idle", "Shoot", 0, true);
	anim_controller->addTransition("idle", "death", "Dead", 1, false);
	anim_controller->addTransition("attack", "death", "Dead", 1, false);
	anim_controller->addTransition("death", "idle", "Dead", 99, false);	//Necesario crear una transicion para crear un animstate
	anim_controller->setParamValue("Dead", 0);
	anim_controller->setParamValue("Shoot", 0);
	enemy->addComponent<FringeHeadAtack>();
	enemy->addComponent<Enemy_Health>(200, 20, Vector2D(50, 5), build_sdlcolor(255, 0, 0, 200), 50);
	enemy->addComponent<ContactDamage>();
}
#pragma endregion

#pragma region ThornFish
void TestZoneState::createThornFish(const Config& entityConfig) {
	auto* thorn = createBasicEntity(entityConfig.pos, entityConfig.size, entityConfig.rotation, entityConfig.vel);
	thorn->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask, false,
		entityConfig.friction, entityConfig.fixedRotation, entityConfig.rotation, Vector2D(entityConfig.size.getX() * 0.3, entityConfig.size.getY() * 0.7),
		Vector2D(), 20000);
	AnimBlendGraph* thorn_anim_controller = thorn->addComponent<AnimBlendGraph>();
	thorn_anim_controller->addAnimation("idle", &sdlutils().images().at("thornfish_idle"), 1, 12, 8, 12, -1, Vector2D(0.5, 0.6));
	thorn_anim_controller->addAnimation("attack_ini", &sdlutils().images().at("thornfish_attack"), 1, 8, 8, 12, 0, 0, 4, Vector2D(0.5, 0.6));
	thorn_anim_controller->addAnimation("attack_end", &sdlutils().images().at("thornfish_attack"), 1, 8, 8, 24, 0, 5, 7, Vector2D(0.5, 0.6));
	thorn_anim_controller->addAnimation("death", &sdlutils().images().at("thornfish_death"), 2, 3, 6, 24, 0, 0, 4, Vector2D(0.5, 0.6));
	//Proportion?
	thorn_anim_controller->keepProportion("idle", Vector2D(thorn->getComponent<Transform>()->getW(), thorn->getComponent<Transform>()->getH()));
	thorn_anim_controller->addTransition("idle", "attack_ini", "Attack", 1, false);
	thorn_anim_controller->addTransition("attack_end", "idle", "Attack", 0, true);
	thorn_anim_controller->addTransition("attack_ini", "idle", "Attack", 0, false);
	thorn_anim_controller->addTransition("attack_ini", "attack_end", "Attack", 2, true);
	thorn_anim_controller->addTransition("idle", "death", "Dead", 1, false);
	thorn_anim_controller->addTransition("attack_ini", "death", "Dead", 1, false);
	thorn_anim_controller->addTransition("attack_end", "death", "Dead", 1, false);
	thorn_anim_controller->addTransition("death", "idle", "Dead", 99, false);	//Necesario crear una transicion para crear un animstate
	thorn_anim_controller->setParamValue("Dead", 0);
	thorn_anim_controller->setParamValue("Attack", 0);
	thorn->addComponent<ThornFishAttack>();
	thorn->addComponent<Enemy_Health>(150, 20, Vector2D(50, 5), build_sdlcolor(255, 0, 0, 255), 50);
	thorn->addComponent<ContactDamage>();
}
#pragma endregion
void TestZoneState::createWeaponGiver(const Config& weaponGiverConfig, const int& weaponType) {
	auto* weaponGiver = createBasicEntity(weaponGiverConfig.pos, weaponGiverConfig.size, weaponGiverConfig.rotation, weaponGiverConfig.vel);

	auto* anim_controller = weaponGiver->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("idle", &sdlutils().images().at(weaponGiverConfig.spriteId), 1, 1, 1, 24, 1);
	weaponGiver->addComponent<BoxCollider>(weaponGiverConfig.physicType, PLAYER_DETECTION, PLAYER_DETECTION_MASK, true);
	weaponGiver->addComponent<GetAWeaponOnCollision>(weaponType);
}