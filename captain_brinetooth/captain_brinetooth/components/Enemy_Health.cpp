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

void Enemy_Health::loseLife(int damage){
	lifes -= damage;
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
	float x = trParent_->getPos().getX() + (trParent_->getW() / 2) - (initBarSize.getX() / 2) - App::camera.x;
	float y = trParent_->getPos().getY() - offsetY_ - App::camera.y;
	Vector2D pos = Vector2D(x, y);
	base = build_sdlrect(pos, initBarSize.getX(), initBarSize.getY());
	SDL_RenderFillRect(sdlutils().renderer(), &base);
	//Barra vida
	SDL_SetRenderDrawColor(sdlutils().renderer(), COLOREXP(barColor));
	//Centramos la barra con respecto a la entidad, su width va a variar a lo largo del juego, usamos initBarSize
	x = trParent_->getPos().getX() + (trParent_->getW() / 2) - (initBarSize.getX() / 2) - App::camera.x;
	y = trParent_->getPos().getY() - offsetY_ - App::camera.y;
	pos = Vector2D(x, y);
	bar = build_sdlrect(pos, barSize.getX(), barSize.getY());
	SDL_RenderFillRect(sdlutils().renderer(), &bar);
}

void Enemy_Health::update(){
	if (ih().keyDownEvent()) {
		//Parte Horizontal
		if (ih().isKeyDown(SDL_SCANCODE_A))
			loseLife(100);
	}
}

