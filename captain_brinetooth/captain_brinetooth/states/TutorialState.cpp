#include "TutorialState.h"
#include "PlayState.h"

void TutorialState::init()
{
	//Fade. Para que se sette el Fade
	GameState::init();
	fadeComp = manager_->getHandler<Fader>()->getComponent<Fade>();
	if (fadeComp != nullptr) {
		fadeComp->setTimeIn(100);
		fadeComp->setTimeOut(2000);
		fadeComp->triggerFade();
	}

	auto* bg = createBasicEntity(Vector2D(sdlutils().width() * App::camera_Zoom_Out, sdlutils().height()*App::camera_Zoom_Out *0.75), 
		Vector2D(sdlutils().width() * App::camera_Zoom_Out * 3.0f, sdlutils().height() * App::camera_Zoom_Out * 2.0f), 0.0f, Vector2D(0, 0));
	bg->addComponent<Animation>("1", &sdlutils().images().at("sky"), 1, 1, 1, 1, 0);
	skipTutorial();
	step_ = -1;
	nextStep_ = false;
	App::camera.x = 0;
	App::camera.y = 0;
}

void TutorialState::update()
{
	manager_->getWorld()->Step(1.0f / 60.0f, 6, 2);
	receiveInput();
	for (int i = 0; i < stepImg.size(); i++) {
		if (stepImg[i]->hasComponent<Transform>()) {
			auto& pos = stepImg[i]->getComponent<Transform>()->getPos();
			pos.set(Vector2D(stepImgPos[i].getX() + App::camera.x, stepImgPos[i].getY() + App::camera.y));
		}
	}
	if (countDown + wait_Time < sdlutils().currRealTime()) {
		if (step_ < 7 && step_ >= 0 && nextStep_) {
			if (heal_) {
				clearStep();
				stepHealth();
			}
			else if (weapon_) {
				clearStep();
				stepHeal();
			}
			else if (attack_) {
				clearStep();
				stepWeapon();
			}
			else if (dash_) {
				clearStep();
				stepAttack();
			}
			else if (camera_Down_ && camera_Up_) {
				clearStep();
				stepDash();
			}
			else if (jump_) {
				clearStep();
				stepCamera();
			}
			else if (moveLeft_ && moveRight_) {
				clearStep();
				stepJump();
			}
		}
		else if (step_ >= 7) {
			step_++;
			if (step_ == 8) {
				clearStep();
				stepCoin();
			}
			else if (step_ == 9) {
				clearStep();
				stepBait();
			}
			else {
				clearStep();
				readyPanel();
			}
		}
		nextStep_ = false;
	}
	GameState::update();
}

void TutorialState::startGame(App* app, SoundManager* snd){
	StateMachine* sM = app->getStateMachine();
	sM->changeState(new PlayState(app, sM->currentState()->getMngr()->getWorld(), snd,false,0,nullptr)); //Cuando accedes al juego desde el tutorial no se carga partida
}

void TutorialState::startTutorial(App* app, SoundManager* snd)
{
	TutorialState* state = dynamic_cast<TutorialState*>(app->getStateMachine()->currentState());
	if (state != nullptr) {
		state->clearPanel();
		state->initTutorial();
	}
}

void TutorialState::skipTutorial()
{
	//Atributos de los botones
	int w = (int)sdlutils().width() * 0.1 * App::camera_Zoom_Out;
	int h = (int)w * 0.5;
	int x = (int)((App::camera.w - w) * 0.35);
	int y = (int)((App::camera.h - h) * 0.55);
	createButton(&sdlutils().images().at("yes_button"), Vector2D(x, y), Vector2D(w, h), startTutorial);
	x = (int)((App::camera.w - w) * 0.65);
	y = (int)((App::camera.h - h) * 0.55);
	createButton(&sdlutils().images().at("no_button"), Vector2D(x, y), Vector2D(w, h), startGame, true);
	//Message
	w = (int)sdlutils().width() * 0.5 * App::camera_Zoom_Out;
	h = (int)w * 0.04;
	x = (int)((App::camera.w - w) * 0.5);
	y = (int)((App::camera.h - h) * 0.4);
	Entity* m = manager_->addEntity();
	m->addComponent<Image>(&sdlutils().msgs().at("skipTutorial"), SDL_Rect{ x,y,w,h }, "t");
	panel.push_back(m);
}

void TutorialState::readyPanel()
{
	player->setActive(false);
	enemy->setActive(false);
	for (Entity* e : room_) {
		e->setActive(false);
		e = nullptr;
	}

	int w = (int)sdlutils().width() * 0.15 * App::camera_Zoom_Out;
	int h = (int)w * 0.4;
	int x = (int)((App::camera.w - w) * 0.5);
	int y = (int)((App::camera.h - h) * 0.55);
	// Continue
	createButton(&sdlutils().images().at("continuar_menu"), Vector2D(x, y), Vector2D(w, h), startGame, true);
	//Message
	w = (int)sdlutils().width() * 0.5 * App::camera_Zoom_Out;
	h = (int)w * 0.04;
	x = (int)((App::camera.w - w) * 0.5);
	y = (int)((App::camera.h - h) * 0.4);
	Entity* m = manager_->addEntity();
	m->addComponent<Image>(&sdlutils().msgs().at("ready?"), SDL_Rect{ x,y,w,h }, "z");
	panel.push_back(m);
}

void TutorialState::initTutorial()
{

	SDL_Delay(App::FPS*10);
	//Initialize flags
	moveLeft_ = moveRight_ = jump_ = camera_Up_ = camera_Down_ = dash_ = attack_ = weapon_ = false;
	//--Player--
	Config playerConfig{};
	playerConfig.pos = Vector2D(sdlutils().width() * 0.5f * App::camera_Zoom_Out, sdlutils().height() * 0.35f * App::camera_Zoom_Out);
	playerConfig.vel = Vector2D(0, 0);
	playerConfig.size = Vector2D(160.0f, 160.0f);
	playerConfig.friction = 0.2f;
	playerConfig.physicType = DYNAMIC;
	playerConfig.fixedRotation = true;
	playerConfig.rotation = 0.0f;
	playerConfig.col = PLAYER;
	playerConfig.colMask = PLAYER_MASK;
	createPlayer(playerConfig);
	createRoom();
	countDown = sdlutils().currRealTime();
	stepMovement();
}

void TutorialState::stepMovement()
{
	step_ = 0;
	//Imagenes
	Entity* a = manager_->addEntity();
	Vector2D aPos = Vector2D(sdlutils().width() * 0.3f * App::camera_Zoom_Out, sdlutils().height() * 0.1f * App::camera_Zoom_Out);
	stepImgPos.push_back(aPos);
	a->addComponent<Transform>(aPos,Vector2D(0, 0),50.0f*App::camera_Zoom_Out, 50.0f* App::camera_Zoom_Out, 0.0f);
	a->addComponent<Animation>("p", &sdlutils().images().at("player_controls"), 3, 4, 12, 1, 0, 0, 0);
	stepImg.push_back(a);

	Entity* d = manager_->addEntity();
	Vector2D dPos = Vector2D(sdlutils().width() * 0.7f * App::camera_Zoom_Out, sdlutils().height() * 0.1f * App::camera_Zoom_Out);
	stepImgPos.push_back(dPos);
	d->addComponent<Transform>(dPos,Vector2D(0, 0), 50.0f * App::camera_Zoom_Out, 50.0f * App::camera_Zoom_Out, 0.0f);
	d->addComponent<Animation>("p", &sdlutils().images().at("player_controls"), 3, 4, 12, 1, 0, 1, 1);
	stepImg.push_back(d);

	//Messages
	int x = (int)(aPos.getX());
	int y = (int)(aPos.getY()+ a->getComponent<Transform>()->getH());
	stepImgPos.push_back(Vector2D(x, y));
	Entity* m = manager_->addEntity();
	m->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), 75.0f * App::camera_Zoom_Out, 10.0f * App::camera_Zoom_Out, 0.0f);
	m->addComponent<Animation>("a",&sdlutils().msgs().at("izquierda"), 1, 1, 1, 1, 0);
	stepImg.push_back(m);

	x = (int)(dPos.getX());
	y = (int)(dPos.getY() + d->getComponent<Transform>()->getH());
	stepImgPos.push_back(Vector2D(x, y));
	Entity* m2 = manager_->addEntity();
	m2->addComponent<Transform>(Vector2D(x,y), Vector2D(0, 0), 75.0f * App::camera_Zoom_Out, 10.0f * App::camera_Zoom_Out, 0.0f);
	m2->addComponent<Animation>("d",&sdlutils().msgs().at("derecha"), 1, 1, 1, 1, 0);
	stepImg.push_back(m2);
}

void TutorialState::stepJump()
{
	step_ = 1;
	//Imagenes
	Entity* a = manager_->addEntity();
	Vector2D aPos = Vector2D(sdlutils().width() * 0.5f * App::camera_Zoom_Out, sdlutils().height() * 0.1f * App::camera_Zoom_Out);
	stepImgPos.push_back(aPos);
	a->addComponent<Transform>(aPos, Vector2D(0, 0), 180.0f * App::camera_Zoom_Out, 120.0f * App::camera_Zoom_Out, 0.0f);
	a->addComponent<Animation>("p", &sdlutils().images().at("player_controls"), 3, 4, 12, 1, 0, 5, 5);
	stepImg.push_back(a);
	
	//Messages
	int x = (int)(aPos.getX());
	int y = (int)(aPos.getY() + a->getComponent<Transform>()->getH()*0.5);
	stepImgPos.push_back(Vector2D(x, y));
	Entity* m = manager_->addEntity();
	m->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), 100.0f * App::camera_Zoom_Out, 30.0f * App::camera_Zoom_Out, 0.0f);
	m->addComponent<Animation>("a", &sdlutils().msgs().at("jump"), 1, 1, 1, 1, 0);
	stepImg.push_back(m);
	countDown = sdlutils().currRealTime() - 2000;
}

void TutorialState::stepCamera()
{
	step_ = 2;
	//Imagenes
	Entity* a = manager_->addEntity();
	Vector2D aPos = Vector2D(sdlutils().width() * 0.3f * App::camera_Zoom_Out, sdlutils().height() * 0.1f * App::camera_Zoom_Out);
	stepImgPos.push_back(aPos);
	a->addComponent<Transform>(aPos, Vector2D(0, 0), 50.0f * App::camera_Zoom_Out, 50.0f * App::camera_Zoom_Out, 0.0f);
	a->addComponent<Animation>("p", &sdlutils().images().at("player_controls"), 3, 4, 12, 1, 0, 2, 2);
	stepImg.push_back(a);

	Entity* d = manager_->addEntity();
	Vector2D dPos = Vector2D(sdlutils().width() * 0.7f * App::camera_Zoom_Out, sdlutils().height() * 0.1f * App::camera_Zoom_Out);
	stepImgPos.push_back(dPos);
	d->addComponent<Transform>(dPos, Vector2D(0, 0), 50.0f * App::camera_Zoom_Out, 50.0f * App::camera_Zoom_Out, 0.0f);
	d->addComponent<Animation>("p", &sdlutils().images().at("player_controls"), 3, 4, 12, 1, 0, 3, 3);
	stepImg.push_back(d);

	//Messages
	int x = (int)(aPos.getX());
	int y = (int)(aPos.getY() + a->getComponent<Transform>()->getH());
	stepImgPos.push_back(Vector2D(x, y));
	Entity* m = manager_->addEntity();
	m->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), 200.0f * App::camera_Zoom_Out, 20.0f * App::camera_Zoom_Out, 0.0f);
	m->addComponent<Animation>("a", &sdlutils().msgs().at("cam_up"), 1, 1, 1, 1, 0);
	stepImg.push_back(m);

	x = (int)(dPos.getX());
	y = (int)(dPos.getY() + d->getComponent<Transform>()->getH());
	stepImgPos.push_back(Vector2D(x, y));
	Entity* m2 = manager_->addEntity();
	m2->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), 200.0f * App::camera_Zoom_Out, 20.0f * App::camera_Zoom_Out, 0.0f);
	m2->addComponent<Animation>("d", &sdlutils().msgs().at("cam_down"), 1, 1, 1, 1, 0);
	stepImg.push_back(m2);
}

void TutorialState::stepDash()
{
	step_ = 3;
	createEnemy();
	//Imagenes
	Entity* a = manager_->addEntity();
	Vector2D aPos = Vector2D(sdlutils().width() * 0.5f * App::camera_Zoom_Out, sdlutils().height() * 0.1f * App::camera_Zoom_Out);
	stepImgPos.push_back(aPos);
	a->addComponent<Transform>(aPos, Vector2D(0, 0), 120.0f * App::camera_Zoom_Out, 80.0f * App::camera_Zoom_Out, 0.0f);
	a->addComponent<Animation>("p", &sdlutils().images().at("player_controls"), 3, 4, 12, 1, 0, 4, 4);
	stepImg.push_back(a);

	//Messages
	int x = (int)(aPos.getX());
	int y = (int)(aPos.getY() + a->getComponent<Transform>()->getH() * 0.5);
	stepImgPos.push_back(Vector2D(x, y));
	Entity* m = manager_->addEntity();
	m->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), 100.0f * App::camera_Zoom_Out, 20.0f * App::camera_Zoom_Out, 0.0f);
	m->addComponent<Animation>("a", &sdlutils().msgs().at("dash"), 1, 1, 1, 1, 0);
	stepImg.push_back(m);
	//Messages
	x = (int)(sdlutils().width() * 0.5f * App::camera_Zoom_Out);
	y = (int)(sdlutils().height() * 0.9f * App::camera_Zoom_Out);
	stepImgPos.push_back(Vector2D(x, y));
	Entity* m2 = manager_->addEntity();
	m2->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), sdlutils().width() * 0.7 * App::camera_Zoom_Out, sdlutils().width() * 0.5 * App::camera_Zoom_Out * 0.07, 0.0f);
	m2->addComponent<Animation>("a", &sdlutils().msgs().at("dash_explanation"), 1, 1, 1, 1, 0);
	stepImg.push_back(m2);
	countDown = sdlutils().currRealTime() - 2000;
}

void TutorialState::stepAttack()
{
	step_ = 4;
	player->addComponent<Armas_HUD>(manager_->getApp());
	player->addComponent<Inventory>()->addWeapon(0);
	//Imagenes
	Entity* a = manager_->addEntity();
	Vector2D aPos = Vector2D(sdlutils().width() * 0.5f * App::camera_Zoom_Out, sdlutils().height() * 0.1f * App::camera_Zoom_Out);
	stepImgPos.push_back(aPos);
	a->addComponent<Transform>(aPos, Vector2D(0, 0), 120.0f * App::camera_Zoom_Out, 120.0f * App::camera_Zoom_Out, 0.0f);
	a->addComponent<Animation>("p", &sdlutils().images().at("player_controls"), 3, 4, 12, 1, 0, 6, 6);
	stepImg.push_back(a);

	//Messages
	int x = (int)(aPos.getX());
	int y = (int)(aPos.getY() + a->getComponent<Transform>()->getH());
	stepImgPos.push_back(Vector2D(x, y));
	Entity* m = manager_->addEntity();
	m->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), 100.0f * App::camera_Zoom_Out, 20.0f * App::camera_Zoom_Out, 0.0f);
	m->addComponent<Animation>("a", &sdlutils().msgs().at("attack"), 1, 1, 1, 1, 0);
	stepImg.push_back(m);
	countDown = sdlutils().currRealTime() + 2000;
}



void TutorialState::stepWeapon()
{
	step_ = 5;
	player->getComponent<Inventory>()->addWeapon(1);
	//Imagenes
	Entity* a = manager_->addEntity();
	Vector2D aPos = Vector2D(sdlutils().width() * 0.5f * App::camera_Zoom_Out, sdlutils().height() * 0.1f * App::camera_Zoom_Out);
	stepImgPos.push_back(aPos);
	a->addComponent<Transform>(aPos, Vector2D(0, 0), 120.0f * App::camera_Zoom_Out, 120.0f * App::camera_Zoom_Out, 0.0f);
	a->addComponent<Animation>("p", &sdlutils().images().at("player_controls"), 3, 4, 12, 1, 0, 7, 7);
	stepImg.push_back(a);

	//Indicacion
	Entity* b = manager_->addEntity();
	Vector2D bPos = Vector2D(sdlutils().width() * 0.25f * App::camera_Zoom_Out, sdlutils().height() * 0.9f * App::camera_Zoom_Out);
	stepImgPos.push_back(bPos);
	b->addComponent<Transform>(bPos, Vector2D(0, 0), 80.0f*App::camera_Zoom_Out, 50.0f * App::camera_Zoom_Out,0.0f);
	b->addComponent<Animation>("p", &sdlutils().images().at("arrow"), 1, 1, 1, 1, 0);
	stepImg.push_back(b);

	//Messages
	int x = (int)(aPos.getX());
	int y = (int)(aPos.getY() + a->getComponent<Transform>()->getH());
	stepImgPos.push_back(Vector2D(x, y));
	Entity* m = manager_->addEntity();
	m->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), 180.0f * App::camera_Zoom_Out, 30.0f * App::camera_Zoom_Out, 0.0f);
	m->addComponent<Animation>("a", &sdlutils().msgs().at("switch"), 1, 1, 1, 1, 0);
	stepImg.push_back(m);

	x = (int)(sdlutils().width() * 0.55f * App::camera_Zoom_Out);
	y = (int)(sdlutils().height() * 0.9f * App::camera_Zoom_Out);
	stepImgPos.push_back(Vector2D(x, y));
	Entity* m2 = manager_->addEntity();
	m2->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), sdlutils().width() * 0.5 * App::camera_Zoom_Out, sdlutils().width() * 0.45 * App::camera_Zoom_Out * 0.08, 0.0f);
	m2->addComponent<Animation>("a", &sdlutils().msgs().at("switch_explanation"), 1, 1, 1, 1, 0);
	stepImg.push_back(m2);
	countDown = sdlutils().currRealTime() + 2000;
}

void TutorialState::stepHeal()
{
	step_ = 6;
	player->addComponent<Player_Health>(&sdlutils().images().at("fullvida"), &sdlutils().images().at("mediavida"), &sdlutils().images().at("vacio"), 300.0f, app, 5, 2);
	player->addComponent<LoseLife>();
	//Perder 5 vidas
	player->getComponent<Player_Health>()->loseLife();
	player->getComponent<Player_Health>()->loseLife();
	player->getComponent<Player_Health>()->loseLife();
	player->getComponent<Player_Health>()->loseLife();
	player->getComponent<Player_Health>()->loseLife();
	//Imagenes
	Entity* a = manager_->addEntity();
	Vector2D aPos = Vector2D(sdlutils().width() * 0.5f * App::camera_Zoom_Out, sdlutils().height() * 0.1f * App::camera_Zoom_Out);
	stepImgPos.push_back(aPos);
	a->addComponent<Transform>(aPos, Vector2D(0, 0), 120.0f * App::camera_Zoom_Out, 120.0f * App::camera_Zoom_Out, 0.0f);
	a->addComponent<Animation>("p", &sdlutils().images().at("player_controls"), 3, 4, 12, 1, 0, 9, 9);
	stepImg.push_back(a);

	//Messages
	int x = (int)(aPos.getX());
	int y = (int)(aPos.getY() + a->getComponent<Transform>()->getH());
	stepImgPos.push_back(Vector2D(x, y));
	Entity* m = manager_->addEntity();
	m->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), 180.0f * App::camera_Zoom_Out, 30.0f * App::camera_Zoom_Out, 0.0f);
	m->addComponent<Animation>("a", &sdlutils().msgs().at("heal"), 1, 1, 1, 1, 0);
	stepImg.push_back(m);

	x = (int)(sdlutils().width() * 0.52f * App::camera_Zoom_Out);
	y = (int)(y + 50 * App::camera_Zoom_Out);
	stepImgPos.push_back(Vector2D(x, y));
	Entity* m2 = manager_->addEntity();
	m2->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), sdlutils().width() * 0.5 * App::camera_Zoom_Out, sdlutils().width() * 0.4 * App::camera_Zoom_Out * 0.08, 0.0f);
	m2->addComponent<Animation>("a", &sdlutils().msgs().at("heal_explanation"), 1, 1, 1, 1, 0);
	stepImg.push_back(m2);
	countDown = sdlutils().currRealTime() - 2000;

}

void TutorialState::stepHealth()
{
	step_ = 7; 
	//Indicacion
	Entity* b = manager_->addEntity();
	Vector2D bPos = Vector2D(sdlutils().width() * 0.25f * App::camera_Zoom_Out, sdlutils().height() * 0.06f * App::camera_Zoom_Out);
	stepImgPos.push_back(bPos);
	b->addComponent<Transform>(bPos, Vector2D(0, 0), 80.0f * App::camera_Zoom_Out, 50.0f * App::camera_Zoom_Out,0.0f);
	b->addComponent<Animation>("p", &sdlutils().images().at("arrow"), 1, 1, 1, 1, 0);
	stepImg.push_back(b);
	//Messages
	int x = (int)(sdlutils().width() * 0.55f * App::camera_Zoom_Out);
	int y = (int)(sdlutils().height() * 0.05f * App::camera_Zoom_Out);
	stepImgPos.push_back(Vector2D(x, y));
	Entity* m2 = manager_->addEntity();
	m2->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), sdlutils().width() * 0.5 * App::camera_Zoom_Out, sdlutils().width() * 0.5 * App::camera_Zoom_Out * 0.06, 0.0f);
	m2->addComponent<Animation>("a", &sdlutils().msgs().at("health_explanation"), 1, 1, 1, 1, 0);
	stepImg.push_back(m2);
	countDown = sdlutils().currRealTime() + 3000;
}

void TutorialState::stepCoin()
{
	//Indicacion
	Entity* b = manager_->addEntity();
	Vector2D bPos = Vector2D(sdlutils().width() * 0.11f * App::camera_Zoom_Out, sdlutils().height() * 0.26f * App::camera_Zoom_Out);
	stepImgPos.push_back(bPos);
	b->addComponent<Transform>(bPos, Vector2D(0, 0), 80.0f * App::camera_Zoom_Out, 50.0f * App::camera_Zoom_Out, 0.0f);
	b->addComponent<Animation>("p", &sdlutils().images().at("arrow"), 1, 1, 1, 1, 0);
	stepImg.push_back(b);
	//Messages
	int x = (int)(sdlutils().width() * 0.5f * App::camera_Zoom_Out);
	int y = (int)(sdlutils().height() * 0.25f * App::camera_Zoom_Out);
	stepImgPos.push_back(Vector2D(x, y));
	Entity* m2 = manager_->addEntity();
	m2->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), sdlutils().width() * 0.7 * App::camera_Zoom_Out, sdlutils().width() * 0.5 * App::camera_Zoom_Out * 0.06, 0.0f);
	m2->addComponent<Animation>("a", &sdlutils().msgs().at("coin_explanation"), 1, 1, 1, 1, 0);
	stepImg.push_back(m2);
	countDown = sdlutils().currRealTime() + 3000;
}

void TutorialState::stepBait()
{
	//Indicacion
	Entity* b = manager_->addEntity();
	Vector2D bPos = Vector2D(sdlutils().width() * 0.1f * App::camera_Zoom_Out, sdlutils().height() * 0.34f * App::camera_Zoom_Out);
	stepImgPos.push_back(bPos);
	b->addComponent<Transform>(bPos, Vector2D(0, 0), 80.0f * App::camera_Zoom_Out, 50.0f * App::camera_Zoom_Out, 0.0f);
	b->addComponent<Animation>("p", &sdlutils().images().at("arrow"), 1, 1, 1, 1, 0);
	stepImg.push_back(b);
	//Messages
	int x = (int)(sdlutils().width() * 0.5f * App::camera_Zoom_Out);
	int y = (int)(sdlutils().height() * 0.33f * App::camera_Zoom_Out);
	stepImgPos.push_back(Vector2D(x, y));
	Entity* m2 = manager_->addEntity();
	m2->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), sdlutils().width() * 0.7 * App::camera_Zoom_Out, sdlutils().width() * 0.5 * App::camera_Zoom_Out * 0.06, 0.0f);
	m2->addComponent<Animation>("a", &sdlutils().msgs().at("bait_explanation"), 1, 1, 1, 1, 0);
	stepImg.push_back(m2);
	countDown = sdlutils().currRealTime() + 3000;
}

void TutorialState::receiveInput()
{
	//Input
	if (ih().keyDownEvent()) {
		if (ih().isKeyDown(SDL_SCANCODE_A) && step_ == 0) {
			moveLeft_ = true;
			nextStep_ = true;
		}
		else if (ih().isKeyDown(SDL_SCANCODE_D) && step_ == 0) {
			moveRight_ = true;
			nextStep_ = true;
		}
		if (ih().isKeyDown(SDL_SCANCODE_SPACE) && step_ == 1) {
			jump_ = true;
			nextStep_ = true;
		}
		if (ih().isKeyDown(SDL_SCANCODE_S) && step_ == 2) {
			camera_Down_ = true;
			nextStep_ = true;
		}
		else if (ih().isKeyDown(SDL_SCANCODE_W) && step_ == 2) {
			camera_Up_ = true;
			nextStep_ = true;
		}
		if (ih().isKeyDown(SDL_SCANCODE_LSHIFT) && step_ == 3) {
			dash_ = true;
			nextStep_ = true;
		}
	}
	if (ih().mouseButtonEvent()) {
		if (step_ == 6 && ih().getMouseButtonState(InputHandler::MOUSEBUTTON::MIDDLE)) {
			heal_ = true;
			nextStep_ = true;
		}
		if (step_ == 5 && ih().getMouseButtonState(InputHandler::MOUSEBUTTON::RIGHT)) {
			weapon_ = true;
			nextStep_ = true;
		}
		if (step_ == 4 && ih().getMouseButtonState(InputHandler::MOUSEBUTTON::LEFT)) {
			attack_ = true;
			nextStep_ = true;
		}
	}
}

void TutorialState::createButton(Texture* t, Vector2D pos, Vector2D size, void(*callback)(App*, SoundManager*), bool canFadeOut_){
	auto* button = createBasicEntity(Vector2D(pos.getX(), pos.getY()), Vector2D(size.getX(), size.getY()), 0.0f, Vector2D(0, 0));
	button->addComponent<Button>(t, callback, app, manager_->getSoundMngr())->canFadeOut(canFadeOut_);
	panel.push_back(button);
}

void TutorialState::clearPanel()
{
	for (Entity* ent : panel) {
		Slider* slider = ent->getComponent<Slider>();
		//Desactivamos las entidades auxiliares del Slider, en caso de serlo.
		if (slider != nullptr) slider->desactivateSlider();
		ent->setActive(false);
	}
	panel.clear();
}

void TutorialState::clearStep()
{
	for (Entity* ent : stepImg) {
		ent->setActive(false);
	}
	manager_->getSoundMngr()->playSoundEffect("get_weapon", 0);
	stepImg.clear();
	stepImgPos.clear();
	countDown = sdlutils().currRealTime();
}

void TutorialState::createPlayer(const Config& playerConfig)
{
	player = createBasicEntity(playerConfig.pos, playerConfig.size, playerConfig.rotation, playerConfig.vel);
	player->addComponent<SkillTree>();

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

#pragma region Sword
	//---SWORD------------------------------------------------------------------------------------------------------
	anim_controller->addAnimation("sword_attack1", &sdlutils().images().at("sword_combo"), 6, 8, 48, 48, 0, 1, 9, Vector2D(0.71, 0.69));
	anim_controller->addAnimation("sword_attack2", &sdlutils().images().at("sword_combo"), 6, 8, 48, 48, 0, 12, 22, Vector2D(0.71, 0.69));
	anim_controller->addAnimation("sword_attack3", &sdlutils().images().at("sword_combo"), 6, 8, 48, 48, 0, 23, 35, Vector2D(0.71, 0.69));
	anim_controller->addAnimation("sword_attack4", &sdlutils().images().at("sword_combo"), 6, 8, 48, 48, -1, 36, 47, Vector2D(0.71, 0.69));

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
	anim_controller->addTransition("dash_air", "chainsaw_attack1", "sword_att", 1, true);
	anim_controller->addTransition("dash_ground", "chainsaw_attack1", "sword_att", 1, true);
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
	//--------------------------------------------------------------------------------------------------------------
#pragma endregion
#pragma endregion

	player->addComponent<BoxCollider>(playerConfig.physicType, PLAYER, PLAYER_MASK, false,
		playerConfig.friction, playerConfig.fixedRotation, playerConfig.rotation, Vector2D(playerConfig.size.getX() * 0.6, playerConfig.size.getY()));
	player->addComponent<TriggerCollider>("Feet", PLAYER_JUMP, PLAYER_JUMP_MASK, Vector2D(0, 0.44), Vector2D(80.0f, 10.0f));
	/*player->addComponent<Player_Health>(&sdlutils().images().at("fullvida"), &sdlutils().images().at("mediavida"), &sdlutils().images().at("vacio"), 300.0f, app);
	player->addComponent<LoseLife>();*/

	//player->addComponent<Armas_HUD>(app);
	//player->addComponent<Inventory>();

	player->addComponent<PlayerController>(3.0f, 2.1f, 3.0f);

	//Por testing basura
	player->addComponent<CameraFollow>(Vector2D(200.0f, -80.0f), 0.1f, false, true,
		Vector2D(sdlutils().width() * 2 * App::camera_Zoom_Out, 0.0f)); //Vector2D offset y porcentaje de la velocidad de la camara, mas bajo mas lento sigue

	//Seteamos al Player como MainHandler
	manager_->setHandler<Player>(player);
}

void TutorialState::createEnemy()
{
	Config entityConfig{};
	entityConfig.pos = Vector2D(sdlutils().width() * 1.2* App::camera_Zoom_Out, sdlutils().height() * 0.85 *App::camera_Zoom_Out);
	entityConfig.vel = Vector2D(0, 0);
	entityConfig.size = Vector2D(120.0f, 120.0f);
	entityConfig.physicType = DYNAMIC;
	entityConfig.col = ENEMY;
	entityConfig.colMask = ENEMY_MASK;
	enemy = createBasicEntity(entityConfig.pos, entityConfig.size, entityConfig.rotation, entityConfig.vel);
	AnimBlendGraph* enemy_anim_controller = enemy->addComponent<AnimBlendGraph>();
	enemy_anim_controller->addAnimation("idle", &sdlutils().images().at("Medusa"), 5, 5, 25, 8, -1, 0, 21);
	enemy_anim_controller->addAnimation("death", &sdlutils().images().at("medusa_death"), 2, 3, 6, 12, 0, 0, 4);
	enemy_anim_controller->addTransition("idle", "death", "Dead", 1, false);
	enemy_anim_controller->addTransition("death", "idle", "Dead", 99, false);	//Necesario crear una transicion para crear un animstate
	enemy_anim_controller->setParamValue("Dead", 0);
	enemy->addComponent<Enemy_Health>(150, 15, Vector2D(300, 20), build_sdlcolor(255, 0, 0, 255), 50);
	enemy->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask, false, 0.7f,true,0.0,Vector2D(),Vector2D(),10000);
	//enemy->addComponent<ContactDamage>();
}

void TutorialState::createRoom()
{
	//---SUELO---
	Config floor{};
	floor.pos = Vector2D(sdlutils().width() * App::camera_Zoom_Out, sdlutils().height()* App::camera_Zoom_Out);
	floor.vel = Vector2D(0, 0);
	floor.size = Vector2D(sdlutils().width() * 2.1f * App::camera_Zoom_Out, sdlutils().width() * 2.0f * App::camera_Zoom_Out*0.08);
	floor.friction = 0.2f;
	floor.physicType = STATIC;
	floor.fixedRotation = true;
	floor.rotation = 0.0f;
	floor.col = GROUND;
	floor.colMask = GROUND_MASK;
	createBoxFloor(floor);
	Config floor2{};
	floor2.pos = Vector2D(sdlutils().width() * 2 * App::camera_Zoom_Out, sdlutils().height() * 0.2f * App::camera_Zoom_Out);
	floor2.vel = Vector2D(0, 0);
	floor2.size = Vector2D(sdlutils().width() * App::camera_Zoom_Out, 50.0f);
	floor2.friction = 0.0f;
	floor2.physicType = STATIC;
	floor2.fixedRotation = true;
	floor2.rotation = 0.5 * M_PI;
	floor2.col = DEFAULT;
	floor2.colMask = DEFAULT_MASK;
	createBoxFloor(floor2);
	Config floor3{};
	floor3.pos = Vector2D(0, sdlutils().height() * 0.2f * App::camera_Zoom_Out);
	floor3.vel = Vector2D(0, 0);
	floor3.size = Vector2D(sdlutils().width() * App::camera_Zoom_Out, 50.0f);
	floor3.friction = 0.0f;
	floor3.physicType = STATIC;
	floor3.fixedRotation = true;
	floor3.rotation = 0.5 * M_PI;
	floor3.col = DEFAULT;
	floor3.colMask = DEFAULT_MASK;
	createBoxFloor(floor3);
}

void TutorialState::createBoxFloor(const Config& entityConfig)
{
	auto* box_Floor = createBasicEntity(entityConfig.pos, entityConfig.size, entityConfig.rotation, entityConfig.vel);
	if (entityConfig.col == GROUND) {
		box_Floor->addComponent<Animation>("f", &sdlutils().images().at("cloud"), 1, 1, 1, 1, 0);
		box_Floor->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask, false, 
			entityConfig.friction, entityConfig.fixedRotation, entityConfig.rotation,Vector2D(entityConfig.size.getX(), entityConfig.size.getY()*0.6));
	}
	else {
		box_Floor->addComponent<BoxCollider>(entityConfig.physicType, entityConfig.col, entityConfig.colMask, false, 
			entityConfig.friction, entityConfig.fixedRotation, entityConfig.rotation);
	}
	room_.push_back(box_Floor);
}
