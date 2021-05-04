#include "Enemy_Health.h"
#include  "../ecs/Entity.h"
#include  "../ecs/Manager.h"
#include "FringeHeadAtack.h"
#include "../game/App.h"
#include "EnemyTrigger.h"


Enemy_Health::Enemy_Health(int health, const Vector2D& sizeBar, const SDL_Color& color, int offsetY = 25) {
	lifes = initLifes = health;
	offsetY_ = offsetY;

	barColor = color;
	baseColor = build_sdlcolor(0,0,0,125);

	barSize = initBarSize = sizeBar;
}

/// <summary>
/// Realiza daño al enemigo. El segundo parametro es 0 por defecto,
/// pero si se asigna a 1 el enemigo es electrificado y a 2 el enemigo se envenena
/// </summary>
/// <param name="damage">Cantidad de daño a infligir</param>
/// <param name="typeOfDamage">1: electrificar; 2: envenenar</param>
void Enemy_Health::loseLife(int damage, int typeOfDamage){
	if (!isElectrified)
		lifes -= damage;
	else
		lifes = lifes - (damage + electrifyDamage);
		

	// Reproducimos sonido aleatorio de monstruo herido
	int x = sdlutils().rand().teCuoto(0, 3);
	switch (x)
	{
	case 0:
		entity_->getMngr()->getSoundMngr()->playSoundEffect("enemy_hurt", 15);
		break;
	case 1:
		entity_->getMngr()->getSoundMngr()->playSoundEffect("enemy_hurt1", 15);
		break;
	case 2:
		entity_->getMngr()->getSoundMngr()->playSoundEffect("enemy_hurt2", 15);
	}

	//Efecto electrificar
	if (typeOfDamage == 1 && isElectrified == false) {
		isElectrified = true;
	}
	else {
		isElectrified = false;
		//Veneno
		if (typeOfDamage == 2) {
			isPoisoned = true;
			poisonCurrentTime = sdlutils().currRealTime();
			poisonCurrentTickTime = sdlutils().currRealTime();
		}
	}

	if (lifes <= 0) {
		entity_->setActive(false);
		//Tenemos que eliminar el Trigger del enemigo (si tiene)
		EnemyTrigger* enT = entity_->getComponent<EnemyTrigger>();
		if(enT != nullptr) enT->getTriggerEntity()->setActive(false);
	}
	barSize.setX((lifes * initBarSize.getX()) / initLifes);
}

void Enemy_Health::init(){
	trParent_ = entity_->getComponent<Transform>();
}

void Enemy_Health::render(){
	//Barra fondo
	SDL_SetRenderDrawColor(sdlutils().renderer(), COLOREXP(baseColor));
	//Centramos la barra con respecto a la entidad
	float x = trParent_->getPos().getX() - (initBarSize.getX() / 2) - App::camera.x;
	float y = trParent_->getPos().getY() - (trParent_->getH() / 2) - offsetY_ - App::camera.y;
	Vector2D pos = Vector2D(x, y);
	base = build_sdlrect(pos, initBarSize.getX(), initBarSize.getY());
	SDL_RenderFillRect(sdlutils().renderer(), &base);
	//Barra vida
	SDL_SetRenderDrawColor(sdlutils().renderer(), COLOREXP(barColor));
	//Centramos la barra con respecto a la entidad, su width va a variar a lo largo del juego, usamos initBarSize
	x = trParent_->getPos().getX() - (initBarSize.getX() / 2) - App::camera.x;
	y = trParent_->getPos().getY() - (trParent_->getH() / 2) - offsetY_ - App::camera.y;
	pos = Vector2D(x, y);
	bar = build_sdlrect(pos, barSize.getX(), barSize.getY());
	SDL_RenderFillRect(sdlutils().renderer(), &bar);
}

void Enemy_Health::update(){
	//if (ih().keyDownEvent()) {
	//	//Parte Horizontal
	//	if (ih().isKeyDown(SDL_SCANCODE_A))
	//		loseLife(100);
	//}

	//Daño por veneno
	if (isPoisoned) {
		//Tiempo hasta fin del veneno
		if (poisonCurrentTime + poisonMaxTime < sdlutils().currRealTime()) {
			isPoisoned = false;
		}

		//Ticks daño por veneno
		if (poisonCurrentTickTime + poisonDamageTickTime < sdlutils().currRealTime()) {
			lifes -= poisonDamage;
			poisonCurrentTickTime = sdlutils().currRealTime();
			if (lifes <= 0) {
				entity_->setActive(false);
				//Tenemos que eliminar el Trigger del enemigo (si tiene)
				EnemyTrigger* enT = entity_->getComponent<EnemyTrigger>();
				if (enT != nullptr) enT->getTriggerEntity()->setActive(false);
			}
			barSize.setX((lifes * initBarSize.getX()) / initLifes);
		}
	}
}

