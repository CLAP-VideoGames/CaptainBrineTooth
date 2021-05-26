#include "SkillTreeState.h"
#include "../components/SkillTree.h"
#include "../components/Inventory.h"
#include "../components/ExplanationMessage.h"
#include "../components/Player_Health.h"

SkillTreeState::SkillTreeState(GameState* stateToRender, App* a, std::shared_ptr<b2World> mundo, SoundManager* snd, Entity* player_) : GameState(a, mundo, snd){
	stRend = stateToRender;
	cam = a->camera;
	player = player_;
	skillTree_ = nullptr;
	points = -1;
	plH = nullptr;

	explMssgs_ = {};
}

void SkillTreeState::init(){
	skillTree_ = player->getComponent<SkillTree>();
	points = player->getComponent<Inventory>()->getCoins();

	plH = player->getComponent<Player_Health>();
	
	createTable();
}

void SkillTreeState::render() const {
	stRend->render();
	GameState::render();
}

void SkillTreeState::update() {

	for (size_t i = 0; i < 5; i++){
		//skillTree_->hasSkill(1);
		int limitPoints = skillTree_->getSkillLimitPoints(i);

		if (points < limitPoints) 
			explMssgs_[i]->setCurrentTexture(1);
	}

	if (ih().keyDownEvent()) {
		if (ih().isKeyDown(SDL_SCANCODE_ESCAPE)) {
			app->getStateMachine()->popState();
		}
	}
	GameState::update();
}

void SkillTreeState::activateExtraLives(App* app, SoundManager* snd){
	StateMachine* sM = app->getStateMachine();
	SkillTreeState* sTS = dynamic_cast<SkillTreeState*>(sM->currentState());

	if (sTS) sTS->setExtraLives();
}

void SkillTreeState::activateExtraHeals(App* app, SoundManager* snd){
	StateMachine* sM = app->getStateMachine();
	SkillTreeState* sTS = dynamic_cast<SkillTreeState*>(sM->currentState());

	if (sTS) sTS->setExtraHeal();
}

void SkillTreeState::activateSpines(App* app, SoundManager* snd){
	StateMachine* sM = app->getStateMachine();
	SkillTreeState* sTS = dynamic_cast<SkillTreeState*>(sM->currentState());

	if (sTS) sTS->setSpines();
}

void SkillTreeState::activateDoubleDamage(App* app, SoundManager* snd) {
	StateMachine* sM = app->getStateMachine();
	SkillTreeState* sTS = dynamic_cast<SkillTreeState*>(sM->currentState());

	if (sTS) sTS->setDoubleDamage();
}

void SkillTreeState::activateSpeedAttack(App* app, SoundManager* snd){
	StateMachine* sM = app->getStateMachine();
	SkillTreeState* sTS = dynamic_cast<SkillTreeState*>(sM->currentState());

	if (sTS) sTS->setSpeedAttack();
}

void SkillTreeState::createTable(){
	// Fondo
	Vector2D sizeSkills(120, 120);

	auto* background = manager_->addEntity();
	int w = sdlutils().width() * App::camera_Zoom_Out;
	int h = sdlutils().height() * App::camera_Zoom_Out;	//mantener aspect ratio
	int x = 0;
	int y = 0;
	background->addComponent<Image>(&sdlutils().images().at("skillBackground"), SDL_Rect{ x, y, w, h }, "title");

	//Title
	auto* title = manager_->addEntity();
	Texture* skillsTitle = &sdlutils().msgs().at("skillsTitle");
	w = skillsTitle->width() * App::camera_Zoom_Out * 0.7;
	h = skillsTitle->height() * App::camera_Zoom_Out * 0.7;	//mantener aspect ratio
	x = (int)(App::camera.w*0.5 - w*0.5);
	y = (int)(App::camera.h * 0.05);
	title->addComponent<Image>(skillsTitle, SDL_Rect{ x, y, w, h }, "title");


	// Table
	auto* table = manager_->addEntity();
	w = (int)sdlutils().width() * App::camera_Zoom_Out * 0.65;
	h = App::camera.h * 0.92;	//mantener aspect ratio
	x = (int)((App::camera.w - w) * 0.5);
	y = /*(int)((App::camera.h - h) * 0.5)*/0;
	table->addComponent<Image>(&sdlutils().images().at("skillTable"), SDL_Rect{ x, y, w, h }, "title");

	//Rama Izquierda
	//Baits
	Texture* textureBait;
	if(skillTree_->hasSkill(ExtraLives))
		textureBait = &sdlutils().images().at("extraBait_taken");
	else textureBait = &sdlutils().images().at("extraBait");

	int x_ = (int)(((App::camera.w / 2) - sizeSkills.getX() * 0.5));
	int y_ = (int)((App::camera.h) * 0.63);
	auto* baits = manager_->addEntity();
	baits->addComponent<Transform>(Vector2D(x_, y_), Vector2D(0, 0), sizeSkills.getX() * 0.7, sizeSkills.getY() * 0.7, 0.0f);
	baits->addComponent<Button>(textureBait, activateExtraLives, app, manager_->getSoundMngr());
	ExplanationMessage* eMthird = baits->addComponent<ExplanationMessage>();
	eMthird->push_backTexture(&sdlutils().images().at("bait_avail"));
	eMthird->push_backTexture(&sdlutils().images().at("bait_unavail"));
	eMthird->setCurrentTexture(0);
	explMssgs_.push_back(eMthird);
	//Heal
	Texture* textureHeal;
	if (skillTree_->hasSkill(ExtraHeal))
		textureHeal = &sdlutils().images().at("healBait_taken");
	else textureHeal = &sdlutils().images().at("healBait");
	x_ = (int)(((App::camera.w/2) - sizeSkills.getX()*2.2));
	y_ = (int)((App::camera.h) * 0.45);
	auto* heal = manager_->addEntity();
	heal->addComponent<Transform>(Vector2D(x_, y_), Vector2D(0, 0), sizeSkills.getX() * 0.7, sizeSkills.getY() * 0.7, 0.0f);
	heal->addComponent<Button>(textureHeal, activateExtraHeals, app, manager_->getSoundMngr());
	ExplanationMessage* eMsecond = heal->addComponent<ExplanationMessage>();
	eMsecond->push_backTexture(&sdlutils().images().at("heal_avail"));
	eMsecond->push_backTexture(&sdlutils().images().at("heal_unavail"));
	eMsecond->setCurrentTexture(0);
	explMssgs_.push_back(eMsecond);
	//Spines
	Texture* textureSpine;
	if (skillTree_->hasSkill(Spines))
		textureSpine = &sdlutils().images().at("spineBait_taken");
	else textureSpine = &sdlutils().images().at("spineBait");
	x_ = (int)(((App::camera.w / 2) - sizeSkills.getX() * 2.2));
	y_ = (int)((App::camera.h) * 0.27);
	auto* spines = manager_->addEntity();
	spines->addComponent<Transform>(Vector2D(x_, y_), Vector2D(0, 0), sizeSkills.getX() * 0.7, sizeSkills.getY() * 0.7, 0.0f);
	spines->addComponent<Button>(textureSpine, activateSpines, app, manager_->getSoundMngr());
	ExplanationMessage* eM = spines->addComponent<ExplanationMessage>();
	eM->push_backTexture(&sdlutils().images().at("spines_avail"));
	eM->push_backTexture(&sdlutils().images().at("spines_unavail"));
	eM->setCurrentTexture(0);
	explMssgs_.push_back(eM);
	
	//Rama Derecha
	//ExtraDamage
	Texture* textureDamage;
	if (skillTree_->hasSkill(ExtraDamage))
		textureDamage = &sdlutils().images().at("doubleBait_taken");
	else textureDamage = &sdlutils().images().at("doubleBait");
	x_ = (int)(((App::camera.w / 2) + sizeSkills.getX() * 1.2));
	y_ = (int)((App::camera.h) * 0.27);
	auto* damage = manager_->addEntity();
	damage->addComponent<Transform>(Vector2D(x_, y_), Vector2D(0, 0), sizeSkills.getX() * 0.7, sizeSkills.getY() * 0.7, 0.0f);
	damage->addComponent<Button>(textureDamage, activateDoubleDamage, app, manager_->getSoundMngr());
	ExplanationMessage* eMforth = damage->addComponent<ExplanationMessage>();
	eMforth->push_backTexture(&sdlutils().images().at("double_avail"));
	eMforth->push_backTexture(&sdlutils().images().at("double_unavail"));
	eMforth->setCurrentTexture(0);
	explMssgs_.push_back(eMforth);
	//ExtraSpeed
	Texture* textureSpeed;
	if (skillTree_->hasSkill(SpeedAttack))
		textureSpeed = &sdlutils().images().at("speedBait_taken");
	else textureSpeed = &sdlutils().images().at("speedBait");
	x_ = (int)(((App::camera.w / 2) + sizeSkills.getX() * 1.2));
	y_ = (int)((App::camera.h) * 0.45);
	auto* speed = manager_->addEntity();
	speed->addComponent<Transform>(Vector2D(x_, y_), Vector2D(0, 0), sizeSkills.getX() * 0.7, sizeSkills.getY() * 0.7, 0.0f);
	speed->addComponent<Button>(textureSpeed, activateSpeedAttack, app, manager_->getSoundMngr());
	ExplanationMessage* eMfifth = speed->addComponent<ExplanationMessage>();
	eMfifth->push_backTexture(&sdlutils().images().at("speed_avail"));
	eMfifth->push_backTexture(&sdlutils().images().at("speed_unavail"));
	eMfifth->setCurrentTexture(0);
	explMssgs_.push_back(eMfifth);
}

void SkillTreeState::setExtraLives() {
	//Si se ha podido comprar la habilidad, que se actualicen las monedas
	if (!skillTree_->hasSkill(ExtraLives) && skillTree_->setSkill(ExtraLives, true, points)) {
		Inventory* invent = player->getComponent<Inventory>();
		invent->substractCoins(skillTree_->getSkillLimitPoints(ExtraLives));
		points = invent->getCoins();
		//Accion
		plH->setMaxLifes(plH->getMaxLifes() + 2);
		plH->resetLifes();
		manager_->getSoundMngr()->playSoundEffect("Kit_anzuelos",0);
	}
}

void SkillTreeState::setExtraHeal(){
	//Si se ha podido comprar la habilidad, que se actualicen las monedas
	if (!skillTree_->hasSkill(ExtraHeal) && skillTree_->setSkill(ExtraHeal, true, points)) {
		Inventory* invent = player->getComponent<Inventory>();
		invent->substractCoins(skillTree_->getSkillLimitPoints(ExtraHeal));
		points = invent->getCoins();
		//Accion
		plH->setMaxHeals(plH->getMaxHeals() + 1);
		plH->createHeal();
		manager_->getSoundMngr()->playSoundEffect("Agua_potable", 0);
	}
}

void SkillTreeState::setSpines() {
	//Si se ha podido comprar la habilidad, que se actualicen las monedas
	if (!skillTree_->hasSkill(Spines) && skillTree_->setSkill(Spines, true, points)){
		Inventory* invent = player->getComponent<Inventory>();
		invent->substractCoins(skillTree_->getSkillLimitPoints(Spines));
		//Accion
		points = invent->getCoins();
		skillTree_->setCounterAttackPercentage(0.07f);
		manager_->getSoundMngr()->playSoundEffect("Escamas_Arapaima", 0);
	}
}

void SkillTreeState::setDoubleDamage(){
	//Si se ha podido comprar la habilidad, que se actualicen las monedas
	if (!skillTree_->hasSkill(ExtraDamage) && skillTree_->setSkill(ExtraDamage, true, points)) {
		Inventory* invent = player->getComponent<Inventory>();
		invent->substractCoins(skillTree_->getSkillLimitPoints(ExtraDamage));
		points = invent->getCoins();
		//Accion
		skillTree_->setAttackModifier(2);
		manager_->getSoundMngr()->playSoundEffect("Collar_piranas", 0);
	}
}

void SkillTreeState::setSpeedAttack(){//Si se ha podido comprar la habilidad, que se actualicen las monedas
	if (!skillTree_->hasSkill(SpeedAttack) && skillTree_->setSkill(SpeedAttack, true, points)) {
		Inventory* invent = player->getComponent<Inventory>();
		invent->substractCoins(skillTree_->getSkillLimitPoints(SpeedAttack));
		points = invent->getCoins();
		//Accion
		skillTree_->setSpeedModifier(1.3f);
		manager_->getSoundMngr()->playSoundEffect("BrineStone", 0);
	}
}
