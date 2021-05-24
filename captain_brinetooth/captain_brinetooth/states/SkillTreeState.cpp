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
}

void SkillTreeState::init(){
	skillTree_ = player->getComponent<SkillTree>();
	points = player->getComponent<Inventory>()->getCoins();

	plH = player->getComponent<Player_Health>();

	//int w = (int)sdlutils().width() * App::camera_Zoom_Out * 0.35;
	//int h = (int)w * 0.6;	//mantener aspect ratio
	////Creditos
	//auto* creditos = manager_->addEntity();
	//int x_ = (int)((App::camera.w - w) * 0.02);
	//int y_ = (int)((App::camera.h - h) * 0.98);
	//creditos->addComponent<Transform>(Vector2D(x_, y_), Vector2D(0, 0), w * 0.7, h * 0.7, 0.0f);

	//creditos->addComponent<Button>(&sdlutils().images().at("healBait"), activateExtraLives, app, manager_->getSoundMngr());
	
	createTable();

	//setExtraLives();
	//setExtraHeal();
	//setSpines();
	//setDoubleDamage();
	//setSpeedAttack();
}

void SkillTreeState::render() const
{
	stRend->render();
	GameState::render();
}

void SkillTreeState::update()
{
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
	//Spines
	Texture* textureSpine = &sdlutils().images().at("spineBait");
	int x_ = (int)(((App::camera.w/2) - sizeSkills.getX()*2.2));
	int y_ = (int)((App::camera.h) * 0.27);
	auto* spines = manager_->addEntity();
	spines->addComponent<Transform>(Vector2D(x_, y_), Vector2D(0, 0), sizeSkills.getX() * 0.7, sizeSkills.getY() * 0.7, 0.0f);
	spines->addComponent<Button>(textureSpine, activateSpines, app, manager_->getSoundMngr());
	ExplanationMessage* eM = spines->addComponent<ExplanationMessage>();
	eM->push_backTexture(&sdlutils().images().at("spines_avail"));
	eM->push_backTexture(&sdlutils().images().at("spines_unavail"));
	eM->setCurrentTexture(0);
	//Heal
	Texture* textureHeal = &sdlutils().images().at("healBait");
	x_ = (int)(((App::camera.w/2) - sizeSkills.getX()*2.2));
	y_ = (int)((App::camera.h) * 0.45);
	auto* heal = manager_->addEntity();
	heal->addComponent<Transform>(Vector2D(x_, y_), Vector2D(0, 0), sizeSkills.getX() * 0.7, sizeSkills.getY() * 0.7, 0.0f);
	heal->addComponent<Button>(textureHeal, activateExtraHeals, app, manager_->getSoundMngr());
	//Baits
	Texture* textureBait = &sdlutils().images().at("extraBait");
	x_ = (int)(((App::camera.w/2) - sizeSkills.getX()*2.2));
	y_ = (int)((App::camera.h) * 0.63);
	auto* baits = manager_->addEntity();
	baits->addComponent<Transform>(Vector2D(x_, y_), Vector2D(0, 0), sizeSkills.getX() * 0.7, sizeSkills.getY() * 0.7, 0.0f);
	baits->addComponent<Button>(textureBait, activateExtraLives, app, manager_->getSoundMngr());
	
	//Rama Derecha
	//ExtraDamage
	Texture* textureDamage = &sdlutils().images().at("doubleBait");
	x_ = (int)(((App::camera.w / 2) + sizeSkills.getX() * 1.2));
	y_ = (int)((App::camera.h) * 0.27);
	auto* damage = manager_->addEntity();
	damage->addComponent<Transform>(Vector2D(x_, y_), Vector2D(0, 0), sizeSkills.getX() * 0.7, sizeSkills.getY() * 0.7, 0.0f);
	damage->addComponent<Button>(textureDamage, activateDoubleDamage, app, manager_->getSoundMngr());
	//ExtraSpeed
	Texture* textureSpeed = &sdlutils().images().at("speedBait");
	x_ = (int)(((App::camera.w / 2) + sizeSkills.getX() * 1.2));
	y_ = (int)((App::camera.h) * 0.45);
	auto* speed = manager_->addEntity();
	speed->addComponent<Transform>(Vector2D(x_, y_), Vector2D(0, 0), sizeSkills.getX() * 0.7, sizeSkills.getY() * 0.7, 0.0f);
	speed->addComponent<Button>(textureSpeed, activateSpeedAttack, app, manager_->getSoundMngr());
}

void SkillTreeState::setExtraLives() {
	//Si se ha podido comprar la habilidad, que se actualicen las monedas
	if(skillTree_->setSkill(ExtraLives, true, points))
		points = player->getComponent<Inventory>()->getCoins();

	
	plH->setMaxLifes(plH->getMaxLifes() + 2);
	plH->resetLifes();
	manager_->getSoundMngr()->playSoundEffect("Kit_anzuelos",0);
}

void SkillTreeState::setExtraHeal(){
	//Si se ha podido comprar la habilidad, que se actualicen las monedas
	if(skillTree_->setSkill(ExtraHeal, true, points))
		points = player->getComponent<Inventory>()->getCoins();
	//Accion
	plH->setMaxHeals(plH->getMaxHeals() + 1);
	plH->createHeal();
	manager_->getSoundMngr()->playSoundEffect("Agua_potable", 0);
}

void SkillTreeState::setSpines() {
	//Si se ha podido comprar la habilidad, que se actualicen las monedas
	if(skillTree_->setSkill(Spines, true, points))
		points = player->getComponent<Inventory>()->getCoins();
	//Accion
	skillTree_->setCounterAttackPercentage(0.07f);
	manager_->getSoundMngr()->playSoundEffect("Escamas_Arapaima", 0);

}

void SkillTreeState::setDoubleDamage(){
	//Si se ha podido comprar la habilidad, que se actualicen las monedas
	if(skillTree_->setSkill(ExtraDamage, true, points)) 
		points = player->getComponent<Inventory>()->getCoins();
	//Accion
	skillTree_->setAttackModifier(2);
	manager_->getSoundMngr()->playSoundEffect("Collar_piranas", 0);
}

void SkillTreeState::setSpeedAttack(){//Si se ha podido comprar la habilidad, que se actualicen las monedas
	if(skillTree_->setSkill(SpeedAttack, true, points))
		points = player->getComponent<Inventory>()->getCoins();
	//Accion
	skillTree_->setSpeedModifier(1.3f);
	manager_->getSoundMngr()->playSoundEffect("BrineStone", 0);

}
