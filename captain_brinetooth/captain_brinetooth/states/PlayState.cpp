#include "PlayState.h"
#include "PasueState.h"
#include <fstream>
struct infopartida;
const auto MAP_PATH = "assets/maps/levelTest/levelTest - copia.tmx";

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW (_NORMAL_BLOCK, __FILE__, __LINE__)
#define  DEBUG_NEW
#endif

PlayState::PlayState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd,int coins, bool ab[6], bool died ) : GameState(a, mundo, snd) {
	fadeComp = nullptr;
}


void PlayState::init() {
	GameState::init();
	fadeComp = manager_->getHandler<Fader>()->getComponent<Fade>();
	if (fadeComp != nullptr) {
		fadeComp->setTimeIn(1000);
		fadeComp->setTimeOut(2000);
		fadeComp->triggerFade();
	}
	//---BG----
	backgroundLevel = createBasicEntity(Vector2D(960, 640), Vector2D(2560, 1440), 0.0f, Vector2D(0, 0));
	auto* bgParallax = backgroundLevel->addComponent<ParallaxScroll>();

	//SE TIENE QUE CREAR PRIMERO EL NIVEL Y LUEGO EL PLAYER
	createMap();

	Config playerConfig{};
	playerConfig.pos = getMngr()->getHandler<Map>()->getComponent<MapProcedural>()->getPlayerPos();
	//playerConfig.pos = Vector2D(0, 0);
	playerConfig.vel = Vector2D(0, 0);
	playerConfig.size = Vector2D(100.0f, 100.0f);
	playerConfig.friction = 0.0f;
	playerConfig.physicType = DYNAMIC;
	playerConfig.fixedRotation = true;
	playerConfig.rotation = 0.0f;
	createPlayer(playerConfig);


	auto* map = getMngr()->getHandler<Map>();
	if (map != nullptr) {
		map->getComponent<MapProcedural>()->setPlayer2spawn();
	}
}


void PlayState::update() {
	//Queremos que, en el estado que hayan objetos físicos, se actualicen las físicas
	manager_->getWorld()->Step(1.0f / 60.0f, 6, 2);
	if (ih().keyDownEvent()) {
		if (ih().isKeyDown(SDL_SCANCODE_ESCAPE)) {
			manager_->getSoundMngr()->playPauseMusic();
			StateMachine* sM = app->getStateMachine();
			//Pusheamos nuevo estado de pausa 
			sM->pushState(new PauseState(this, app, sM->currentState()->getMngr()->getWorld(), sM->currentState()->getMngr()->getSoundMngr()));
		}
	}
	GameState::update();
}

/// <summary>
/// Crea el tile nivel 0 con f�sicas
/// </summary>
void PlayState::createMap() {
	auto* nivel = manager_->addEntity();
	nivel->addComponent<EnemyGenerator>();
	Level0* levelTile = nivel->addComponent<Level0>(MAP_PATH, manager_->getWorld());
	map = nivel->addComponent<MapProcedural>(7, 0, app);
	getMngr()->setHandler<Map>(nivel);
	camLimits = levelTile->getMaxCoordenates();
}

void PlayState::createPlayer(const Config& playerConfig) {
	auto* player = createBasicEntity(playerConfig.pos, playerConfig.size, playerConfig.rotation, playerConfig.vel);

	SkillTree* skillTree_ = player->addComponent<SkillTree>();
	float* speedModifer = skillTree_->getSpeedModifier();

#pragma region Animations
	//Plantilla de uso de ANIMATION CONTROLLER
	auto* anim_controller = player->addComponent<AnimBlendGraph>();
#pragma region idle, run & jump
	//-idle, run & jump---------------------------------------------------------------------------------------------------
	//Animations
	anim_controller->addAnimation("idle", &sdlutils().images().at("player_idle"), 4, 6, 24, 24, -1);
	anim_controller->addAnimation("run", &sdlutils().images().at("player_run"), 4, 5, 20, 24, -1);
	anim_controller->addAnimation("jump", &sdlutils().images().at("player_jump"), 4, 6, 24, 36, 0);
	anim_controller->addAnimation("dash_ground", &sdlutils().images().at("player_dash_ground"), 3, 4, 12, 60, 0, 0, 11, Vector2D(0.5, 0.2));
	anim_controller->addAnimation("dash_air", &sdlutils().images().at("player_dash_air"), 3, 4, 12, 60, 0, 0, 11, Vector2D(0.5, 0.5));
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
#pragma region dash
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
#pragma region Weapons
	//-WEAPONS------------------------------------------------------------------------------------------------------
#pragma region Chainsaw
	//---CHAINSAW---------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("chainsaw_attack1", &sdlutils().images().at("chainsaw_combo"), 6, 8, 48, 15, 0, 1, 8, speedModifer, Vector2D(0.75, 0.72));
	anim_controller->addAnimation("chainsaw_attack2", &sdlutils().images().at("chainsaw_combo"), 6, 8, 48, 15, 0, 9, 18, speedModifer, Vector2D(0.75, 0.72));
	anim_controller->addAnimation("chainsaw_attack3", &sdlutils().images().at("chainsaw_combo"), 6, 8, 48, 28, 0, 19, 27, speedModifer, Vector2D(0.75, 0.72));
	anim_controller->addAnimation("chainsaw_attack4", &sdlutils().images().at("chainsaw_combo"), 6, 8, 48, 24, -1, 28, 47, speedModifer, Vector2D(0.75, 0.72));

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

	anim_controller->addTransition("chainsaw_attack1", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("chainsaw_attack1", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "chainsaw_attack1", "chainsaw_att", 1, true);
	anim_controller->addTransition("dash_ground", "chainsaw_attack1", "chainsaw_att", 1, true);
	anim_controller->addTransition("chainsaw_attack2", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("chainsaw_attack2", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "chainsaw_attack2", "chainsaw_att", 2, true);
	anim_controller->addTransition("dash_ground", "chainsaw_attack2", "chainsaw_att", 2, true);
	anim_controller->addTransition("chainsaw_attack3", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("chainsaw_attack3", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "chainsaw_attack3", "chainsaw_att", 3, true);
	anim_controller->addTransition("dash_ground", "chainsaw_attack3", "chainsaw_att", 3, true);
	anim_controller->addTransition("chainsaw_attack4", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("chainsaw_attack4", "dash_ground", "Dash_Ground", 1, false);
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
	anim_controller->addAnimation("sword_attack1", &sdlutils().images().at("sword_combo"), 6, 8, 48, 60, 0, 1, 9, speedModifer, Vector2D(0.71, 0.69));
	anim_controller->addAnimation("sword_attack2", &sdlutils().images().at("sword_combo"), 6, 8, 48, 60, 0, 12, 22, speedModifer, Vector2D(0.71, 0.69));
	anim_controller->addAnimation("sword_attack3", &sdlutils().images().at("sword_combo"), 6, 8, 48, 60, 0, 23, 35, speedModifer, Vector2D(0.71, 0.69));
	anim_controller->addAnimation("sword_attack4", &sdlutils().images().at("sword_combo"), 6, 8, 48, 60, -1, 36, 47, speedModifer, Vector2D(0.71, 0.69));

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

	anim_controller->addTransition("sword_attack1", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("sword_attack1", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "sword_attack1", "sword_att", 1, true);
	anim_controller->addTransition("dash_ground", "sword_attack1", "sword_att", 1, true);
	anim_controller->addTransition("sword_attack2", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("sword_attack2", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "sword_attack2", "sword_att", 2, true);
	anim_controller->addTransition("dash_ground", "sword_attack2", "sword_att", 2, true);
	anim_controller->addTransition("sword_attack3", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("sword_attack3", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "sword_attack3", "sword_att", 3, true);
	anim_controller->addTransition("dash_ground", "sword_attack3", "sword_att", 3, true);
	anim_controller->addTransition("sword_attack4", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("sword_attack4", "dash_ground", "Dash_Ground", 1, false);
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
	anim_controller->addAnimation("hammer_attack1", &sdlutils().images().at("hammer_combo"), 5, 7, 31, 48, 0, 1, 16, speedModifer, Vector2D(0.75, 0.69));
	anim_controller->addAnimation("hammer_attack2", &sdlutils().images().at("hammer_combo"), 5, 7, 31, 48, 0, 17, 30, speedModifer, Vector2D(0.75, 0.69));

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

	anim_controller->addTransition("hammer_attack1", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("hammer_attack1", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "hammer_attack1", "hammer_att", 1, true);
	anim_controller->addTransition("dash_ground", "hammer_attack1", "hammer_att", 1, true);
	anim_controller->addTransition("hammer_attack2", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("hammer_attack2", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "hammer_attack2", "hammer_att", 2, true);
	anim_controller->addTransition("dash_ground", "hammer_attack2", "hammer_att", 2, true);
	//death
	anim_controller->addTransition("hammer_attack1", "death", "Dead", 1, false);
	anim_controller->addTransition("hammer_attack2", "death", "Dead", 1, false);

	anim_controller->setParamValue("hammer_att", 0);
	//	//--------------------------------------------------------------------------------------------------------------
#pragma endregion
//
//#pragma region Crab
////---CRAB---------------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("crab_attack1", &sdlutils().images().at("crab_combo"), 6, 7, 39, 35, 0, 1, 8, speedModifer, Vector2D(0.71, 0.5));
	anim_controller->addAnimation("crab_attack2", &sdlutils().images().at("crab_combo"), 6, 7, 39, 48, 0, 8, 25, speedModifer, Vector2D(0.71, 0.5));
	anim_controller->addAnimation("crab_attack3", &sdlutils().images().at("crab_combo"), 6, 7, 39, 48, 0, 27, 38, speedModifer, Vector2D(0.71, 0.5));

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

	anim_controller->addTransition("crab_attack1", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("crab_attack1", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "crab_attack1", "crab_att", 1, true);
	anim_controller->addTransition("dash_ground", "crab_attack1", "crab_att", 1, true);
	anim_controller->addTransition("crab_attack2", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("crab_attack2", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "crab_attack2", "crab_att", 2, true);
	anim_controller->addTransition("dash_ground", "crab_attack2", "crab_att", 2, true);
	anim_controller->addTransition("crab_attack3", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("crab_attack3", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "crab_attack3", "crab_att", 3, true);
	anim_controller->addTransition("dash_ground", "crab_attack3", "crab_att", 3, true);
	//death
	anim_controller->addTransition("crab_attack1", "death", "Dead", 1, false);
	anim_controller->addTransition("crab_attack2", "death", "Dead", 1, false);
	anim_controller->addTransition("crab_attack3", "death", "Dead", 1, false);

	anim_controller->setParamValue("crab_att", 0);

	//FISTS

	anim_controller->addAnimation("fist_attack1", &sdlutils().images().at("fists_combo"), 6, 7, 39, 35, 0, 1, 8, speedModifer,Vector2D(0.71, 0.5));
	anim_controller->addAnimation("fist_attack2", &sdlutils().images().at("fists_combo"), 6, 7, 39, 48, 0, 8, 25, speedModifer, Vector2D(0.71, 0.5));
	anim_controller->addAnimation("fist_attack3", &sdlutils().images().at("fists_combo"), 6, 7, 39, 48, 0, 27, 38, speedModifer, Vector2D(0.71, 0.5));

	anim_controller->addTransition("run", "fist_attack1", "fist_att", 1, false);
	anim_controller->addTransition("idle", "fist_attack1", "fist_att", 1, false);
	anim_controller->addTransition("jump", "fist_attack1", "fist_att", 1, false);
	anim_controller->addTransition("fist_attack1", "run", "fist_att", 0, true);
	anim_controller->addTransition("fist_attack1", "fist_attack2", "fist_att", 2, true);
	anim_controller->addTransition("run", "fist_attack2", "fist_att", 2, false);
	anim_controller->addTransition("idle", "fist_attack2", "fist_att", 2, false);
	anim_controller->addTransition("jump", "fist_attack2", "fist_att", 2, false);
	anim_controller->addTransition("fist_attack2", "run", "fist_att", 0, false);
	anim_controller->addTransition("fist_attack2", "fist_attack3", "fist_att", 3, false);
	anim_controller->addTransition("run", "fist_attack3", "fist_att", 3, false);
	anim_controller->addTransition("idle", "fist_attack3", "fist_att", 3, false);
	anim_controller->addTransition("jump", "fist_attack3", "fist_att", 3, false);
	anim_controller->addTransition("fist_attack3", "run", "fist_att", 0, false);
	anim_controller->addTransition("fist_attack3", "fist_attack1", "fist_att", 1, true);

	anim_controller->addTransition("fist_attack1", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("fist_attack1", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "fist_attack1", "fist_att", 1, true);
	anim_controller->addTransition("dash_ground", "fist_attack1", "fist_att", 1, true);
	anim_controller->addTransition("fist_attack2", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("fist_attack2", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "fist_attack2", "fist_att", 2, true);
	anim_controller->addTransition("dash_ground", "fist_attack2", "fist_att", 2, true);
	anim_controller->addTransition("fist_attack3", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("fist_attack3", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "fist_attack3", "fist_att", 3, true);
	anim_controller->addTransition("dash_ground", "fist_attack3", "fist_att", 3, true);
	//death
	anim_controller->addTransition("fist_attack1", "death", "Dead", 1, false);
	anim_controller->addTransition("fist_attack2", "death", "Dead", 1, false);
	anim_controller->addTransition("fist_attack3", "death", "Dead", 1, false);

	anim_controller->setParamValue("fist_att", 0);

	//	//--------------------------------------------------------------------------------------------------------------
	//#pragma endregion
	//
	//#pragma region MachineGun
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

	anim_controller->addTransition("machine_gun1", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("machine_gun1", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "machine_gun1", "machineGun_att", 1, true);
	anim_controller->addTransition("dash_ground", "machine_gun1", "machineGun_att", 1, true);
	anim_controller->addTransition("machine_gun2", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("machine_gun2", "dash_ground", "Dash_Ground", 1, false);
	//death
	anim_controller->addTransition("machine_gun1", "death", "Dead", 1, false);
	anim_controller->addTransition("machine_gun2", "death", "Dead", 1, false);

	anim_controller->setParamValue("machineGun_att", 0);

	//Bellow
	anim_controller->addAnimation("bellow", &sdlutils().images().at("squid_combo"), 3, 4, 12, 48, 1, Vector2D(0.67, 0.5));

	anim_controller->addTransition("run", "bellow", "bellow_att", 1, false);
	anim_controller->addTransition("idle", "bellow", "bellow_att", 1, false);
	anim_controller->addTransition("jump", "bellow", "bellow_att", 1, false);
	anim_controller->addTransition("bellow", "run", "bellow_att", 0, false);
	anim_controller->addTransition("bellow", "idle", "bellow_att", 0, false);

	anim_controller->addTransition("bellow", "dash_air", "Dash_Air", 1, false);
	anim_controller->addTransition("bellow", "dash_ground", "Dash_Ground", 1, false);
	anim_controller->addTransition("dash_air", "bellow", "bellow_att", 1, true);
	anim_controller->addTransition("dash_ground", "bellow", "bellow_att", 1, true);
	//death
	anim_controller->addTransition("bellow", "death", "Dead", 1, false);

	anim_controller->setParamValue("bellow_att", 0);
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion
#pragma endregion

	player->addComponent<BoxCollider>(playerConfig.physicType, PLAYER, PLAYER_MASK, false,
		playerConfig.friction, playerConfig.fixedRotation, playerConfig.rotation, Vector2D(playerConfig.size.getX() * 0.6, playerConfig.size.getY() * 0.95));
	player->addComponent<TriggerCollider>("Feet", PLAYER_JUMP, PLAYER_JUMP_MASK, Vector2D(0, 0.25), Vector2D(55.0f, 15.0f));
	player->addComponent<Player_Health>(&sdlutils().images().at("fullvida"), &sdlutils().images().at("mediavida"), &sdlutils().images().at("vacio"), 300.0f, app, 5, 2);
	player->addComponent<Armas_HUD>(app);

	player->addComponent<PlayerController>();

	player->addComponent<CameraFollow>(Vector2D(100.0f, -80.0f), 0.08f, false, false, manager_->getHandler<Map>()->getComponent<Level0>()->getMaxCoordenates()); //Vector2D offset y porcentaje de la velocidad de la camara, mas bajo mas lento sigue
	player->addComponent<Inventory>();

	player->addComponent<LoseLife>();


	//Carga de archivo
	manager_->setHandler<Player>(player);
	if (app->IsSavedGame())
		loadGame();
}

Entity* PlayState::getBackgroundLevel() {
	return backgroundLevel;
}

void PlayState::createWeaponGiver(const Config& weaponGiverConfig, const int& weaponType) {
	auto* weaponGiver = createBasicEntity(weaponGiverConfig.pos, weaponGiverConfig.size, weaponGiverConfig.rotation, weaponGiverConfig.vel);

	auto* anim_controller = weaponGiver->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("idle", &sdlutils().images().at(weaponGiverConfig.spriteId), 1, 1, 1, 24, 1);
	weaponGiver->addComponent<BoxCollider>(weaponGiverConfig.physicType, PLAYER_DETECTION, PLAYER_DETECTION_MASK, true);
	weaponGiver->addComponent<GetAWeaponOnCollision>(weaponType);
}
