#include "Player_Health.h"

#define _CRTDBG_MAP_ALLOC
#include<iostream>
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#include "../states/PlayState.h"

Player_Health::~Player_Health()
{
	for (Entity* e : heals) {
		e->setActive(false);
		e = nullptr;
	}
	if (particle_heal != nullptr) {
		particle_heal->setActive(false);
		particle_heal = nullptr;
	}
}

void Player_Health::init()
{
	frame = Vector2D(0, 15);
	auto w = fVida->width() / 8;
	auto h = fVida->height();

	src = build_sdlrect(frame, w, h);
	time_ = 0;
	frameSize = Vector2D(w, h);

	#pragma region Curas
	//Curas y valor (todas llenas)
	maxValueHeal_ = 1500;
	for (int i = 0; i < maxHeals_; i++) {
		createHeal();
	}
	#pragma endregion
	resetLifes();
	invulnerability_ = false;
	elpased_time_invul_ = 0;
	deadCountdown = 0;
	playerIsDying = false;
}

void Player_Health::createHeal() {
	auto* e = entity_->getMngr()->addEntity();
	e->addComponent<Transform>(Vector2D(), Vector2D(), 48 * App::camera_Zoom_Out, 48 * App::camera_Zoom_Out, 0.0f);
	auto* e_anim_controller = e->addComponent<AnimBlendGraph>();
	e_anim_controller->addAnimation("heal_0", &sdlutils().images().at("player_heal"), 3, 5, 15, 1, 0, 0, 0);
	e_anim_controller->addAnimation("heal_20", &sdlutils().images().at("player_heal"), 3, 5, 15, 1, 0, 1, 1);
	e_anim_controller->addAnimation("heal_40", &sdlutils().images().at("player_heal"), 3, 5, 15, 1, 0, 2, 2);
	e_anim_controller->addAnimation("heal_60", &sdlutils().images().at("player_heal"), 3, 5, 15, 1, 0, 3, 3);
	e_anim_controller->addAnimation("heal_80", &sdlutils().images().at("player_heal"), 3, 5, 15, 1, 0, 4, 4);
	e_anim_controller->addAnimation("heal_100", &sdlutils().images().at("player_heal"), 3, 5, 15, 12, -1, 5, 12);
	e_anim_controller->addTransition("heal_0", "heal_20", "Value", 1, false);
	e_anim_controller->addTransition("heal_0", "heal_40", "Value", 2, false);
	e_anim_controller->addTransition("heal_0", "heal_60", "Value", 3, false);
	e_anim_controller->addTransition("heal_0", "heal_80", "Value", 4, false);
	e_anim_controller->addTransition("heal_0", "heal_100", "Value", 5, false);
	e_anim_controller->addTransition("heal_20", "heal_0", "Value", 0, false);
	e_anim_controller->addTransition("heal_20", "heal_40", "Value", 2, false);
	e_anim_controller->addTransition("heal_20", "heal_60", "Value", 3, false);
	e_anim_controller->addTransition("heal_20", "heal_80", "Value", 4, false);
	e_anim_controller->addTransition("heal_20", "heal_100", "Value", 5, false);
	e_anim_controller->addTransition("heal_40", "heal_0", "Value", 0, false);
	e_anim_controller->addTransition("heal_40", "heal_60", "Value", 3, false);
	e_anim_controller->addTransition("heal_40", "heal_80", "Value", 4, false);
	e_anim_controller->addTransition("heal_40", "heal_100", "Value", 5, false);
	e_anim_controller->addTransition("heal_60", "heal_0", "Value", 0, false);
	e_anim_controller->addTransition("heal_60", "heal_80", "Value", 4, false);
	e_anim_controller->addTransition("heal_60", "heal_100", "Value", 5, false);
	e_anim_controller->addTransition("heal_80", "heal_0", "Value", 0, false);
	e_anim_controller->addTransition("heal_80", "heal_100", "Value", 5, false);
	e_anim_controller->addTransition("heal_100", "heal_0", "Value", 0, false);
	e_anim_controller->addTransition("heal_100", "heal_20", "Value", 1, false);
	e_anim_controller->addTransition("heal_100", "heal_40", "Value", 2, false);
	e_anim_controller->addTransition("heal_100", "heal_60", "Value", 3, false);
	e_anim_controller->addTransition("heal_100", "heal_80", "Value", 4, false);
	e_anim_controller->setParamValue("Value", 5);
	heals.push_back(e);
	healsValues.push_back(maxValueHeal_);
}

void Player_Health::render()
{
	float vidaRestante = vidas;
	float currentVidas = vidas+0.5;
	src = build_sdlrect(frame, frameSize.getX(), frameSize.getY());
	Vector2D aux = Vector2D(10 + 35 * 0, 10);
	float zoom = g->camera_Zoom_Out;

	for (int i = 0; i < vidas-0.5; i++)
	{
		aux = Vector2D(16 + 48 * i * zoom, 16);
		SDL_Rect dest = build_sdlrect(aux, (src.w / 3.5) * zoom, (src.h / 4)* zoom);

		fVida->render(src, dest);
		vidaRestante--;
	}
	
	// Si hay media vida, renderizamos medio anzuelo
	if (vidaRestante > 0)
	{
		aux = Vector2D(16 + 48 * (vidas-0.5) * zoom, 16);
		SDL_Rect dest = build_sdlrect(aux, (src.w / 3.5) * zoom, (src.h / 4)* zoom);
		hVida->render(src, dest);
	}
	
	if (currentVidas < maxVidas)
	{
		if (vidas == 0)
			aux.setX(-32);
		for (int i = 0; i < (maxVidas - currentVidas); i++)
		{
			aux = Vector2D(aux.getX() + (zoom * 48), 8);
			SDL_Rect dest = build_sdlrect(aux, (src.w / 3.5)* zoom, (src.h / 3.5)*zoom);
			vVida->render(dest);
		}
	}
	// Si pasa mas de X tiempo, pasamos al siguiente frame
	if (sdlutils().currRealTime() > time_ + tiempoanimacion)
	{
		time_ = sdlutils().currRealTime();

		// Avanzamos de frame
		frame = Vector2D(frame.getX() + 194, frame.getY()); // 194 es la cantidad exacta a avanzar al siguiente frame sin que ocurra nada
		nFrame++;

		// Si llegamos al ultimo frame, volvemos al primero
		if (nFrame > 7) { frame = Vector2D(0, 15); nFrame = 0; }
	}

	//Invulnerabilidad
	if (invulnerability_ && sdlutils().currRealTime() > elpased_time_invul_ + cd_invul_)
		invulnerability_ = false;

}

void Player_Health::update()
{
	//Muerte automatica
	if (ih().keyDownEvent()) {
		if (ih().isKeyDown(SDL_SCANCODE_0)) {
			vidas = 0.5;
			loseLife();
		}
	}
	//Curacion
	if (ih().mouseButtonEvent() || ih().getMouseButtonHeld()) {
		//Si el boton fue el izquierdo o si el izquierdo se sigue presionando
		if (ih().getMouseButtonState(InputHandler::MOUSEBUTTON::MIDDLE)) {
			heal();
		}
	}
	//Tiempo despues de muerto
	if (entity_->getComponent<AnimBlendGraph>()->getCurrentAnimation()->getID() == "death" && entity_->getComponent<AnimBlendGraph>()->isComplete() && deadCountdown == 0)
		deadCountdown = sdlutils().currRealTime();

	if(deadCountdown + dead_time < sdlutils().currRealTime() && deadCountdown != 0)
		respawn();

	//Curas
	for (size_t i = 0; i < maxHeals_; i++) {
		int v = healsValues[i] / (maxValueHeal_ / 5);
		heals[i]->getComponent<AnimBlendGraph>()->setParamValue("Value", v);
		heals[i]->getComponent<Transform>()->getPos().set(
			Vector2D(App::camera.x + 24 + heals[i]->getComponent<Transform>()->getW() * 0.5 + heals[i]->getComponent<Transform>()->getW() * i, App::camera.y + 64 + heals[i]->getComponent<Transform>()->getH()));
	}
	if (particle_heal != nullptr) {
		if (particle_heal->getComponent<Animation>()->getState() == 0) {
			particle_heal->setActive(false);
			particle_heal = nullptr;
		}
		else {
			particle_heal->getComponent<Transform>()->getPos() = entity_->getComponent<Transform>()->getPos();
		}
	}
}

void Player_Health::loseLife() 
{
	int x = sdlutils().rand().teCuoto(0, 2);

	// Elegimos un sonido de jugador herido y lo tocamos
	switch (x)
	{
	case 0:
		entity_->getMngr()->getSoundMngr()->playSoundEffect("player_hurt2", 0);
		break;
	case 1:
		entity_->getMngr()->getSoundMngr()->playSoundEffect("player_hurt1", 0);
		break;
	
	default:
		break;
	}
	if (vidas >= 0.5) { vidas -= 0.5f;  g->ShakeCamera(20); }

	invulnerability_ = true;
	elpased_time_invul_ = sdlutils().currRealTime();

	if (vidas <= 0)
	{
		entity_->getComponent<BoxCollider>()->setSpeed(Vector2D(0,0));
		entity_->getMngr()->getSoundMngr()->fadeOutMusic();
		entity_->getMngr()->getSoundMngr()->playSoundEffect("muerte_jugador", 0);
		entity_->getComponent<AnimBlendGraph>()->setParamValue("Dead", 1);
		playerIsDying = true;
	}
}

void Player_Health::setMaxLifes(int n){
	maxVidas = n;
}

void Player_Health::setMaxHeals(int n){
	maxHeals_ = n;
}

float Player_Health::getMaxLifes()
{
	return maxVidas;
}

void Player_Health::heal()
{
	if (vidas > 0) {
		int i = healsValues.size() - 1;
		while (i >= 0 && healsValues[i] != maxValueHeal_) i--;
		if (i >= 0) {
			entity_->getMngr()->getSoundMngr()->playSoundEffect("heal", 0);
			vidas += 3;	//VALOR A CURAR
			if (vidas > maxVidas)
				vidas = maxVidas;
			healsValues[i] = 0;
			//Actualizar valor de las curas
			if (i + 1 < healsValues.size()) {
				healsValues[i] = healsValues[i + 1];
				healsValues[i + 1] = 0;
			}
			//Particulas
			particle_heal = entity_->getMngr()->addEntity();
			particle_heal->addComponent<Transform>(entity_->getComponent<Transform>()->getPos(), Vector2D(), entity_->getComponent<Transform>()->getW()*0.75, entity_->getComponent<Transform>()->getW() * 1.25, 0.0);
			particle_heal->addComponent<Animation>("h", &sdlutils().images().at("healing_effect"), 2, 3, 6, 12, 0);
			particle_heal->getComponent<Animation>()->setAlpha(192);
		}
	}
}

void Player_Health::chargeHeal(int charge)
{
	size_t i = 0;
	while (i < maxHeals_ && healsValues[i] >= maxValueHeal_) i++;
	if (i < maxHeals_) {
		healsValues[i] += charge;
		if (healsValues[i] >= maxValueHeal_) {
			charge = healsValues[i] - maxValueHeal_;
			healsValues[i] = maxValueHeal_;
			if (i + 1 < maxHeals_) {
				healsValues[i + 1] += charge;
				if (healsValues[i + 1] >= maxValueHeal_)
					healsValues[i + 1] = maxValueHeal_;
			}
		}
	}
}

int Player_Health::getMaxHeals(){
	return maxHeals_;
}

int Player_Health::getLife(){
	return vidas;
}

void Player_Health::resetLifes(){  
	vidas = maxVidas;
}

const bool& Player_Health::getInvulnerable(){
	return invulnerability_;
}

const bool& Player_Health::getPlayerIsDying(){
	return playerIsDying;
}

void Player_Health::setLife(float life){
	vidas = life;
}

void Player_Health::respawn(){
	StateMachine* sM = g->getStateMachine();
	Manager* mngr = entity_->getMngr();
	mngr->getSoundMngr()->ChangeMainMusic("Nivel1");

	sM->changeState(new PlayState(g, mngr->getWorld(), mngr->getSoundMngr(),false));
}