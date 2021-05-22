#include "Enemy_Health.h"


Enemy_Health::Enemy_Health(int health, int reward, const Vector2D& sizeBar, const SDL_Color& color, int offsetY = 25) {
	lifes = initLifes = health;
	reward_ = reward;
	offsetY_ = offsetY;

	barColor = color;
	baseColor = build_sdlcolor(0,0,0,125);

	barSize = initBarSize = sizeBar;
	skip_reset_ = false;
}

Enemy_Health::~Enemy_Health()
{
	if (particle_hit != nullptr) {
		particle_hit->setActive(false);
		particle_hit = nullptr;
	}
	if (particle_coins != nullptr) {
		particle_coins->setActive(false);
		particle_coins = nullptr;
	}
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
		

	if (lifes <= 0)
	{
		//Animacion muerte
		entity_->getComponent<AnimBlendGraph>()->setParamValue("Dead", 1);
		// Sonido de muerte del monstruo
		entity_->getMngr()->getSoundMngr()->playSoundEffect("muerte_monstruo", 15);
		//Borrar collider
		entity_->removeComponent<ContactDamage>();

		//Particulas monedas
		particle_coins = entity_->getMngr()->addEntity();
		particle_coins->addComponent<Transform>(Vector2D(trParent_->getPos().getX(), trParent_->getPos().getY()+ trParent_->getH()*0.5), Vector2D(), trParent_->getW()*0.9, trParent_->getW()*0.75, 0.0);
		particle_coins->addComponent<Animation>("coins", &sdlutils().images().at("particle_coins"), 3, 4, 12, 48, 0, 0, 10);
	}
	else
	{
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

	barSize.setX((lifes * initBarSize.getX()) / initLifes);
	if (barSize.getX() <= 0) barSize.setX(0);

	//Cambio color 
	entity_->getComponent<AnimBlendGraph>()->setColor(207, 59, 82);
	skip_reset_ = true;
	cd_reset_ = 6;
	//Particula HIT
	if (particle_hit == nullptr) {
		std::string name;
		int z = sdlutils().rand().teCuoto(0, 3);
		switch (z)
		{
		case 0:
			name = "enemy_hit_1";
			break;
		case 1:
			name = "enemy_hit_2";
			break;
		case 2:
			name = "enemy_hit_3";
			break;
		}
		particle_hit = entity_->getMngr()->addEntity();
		particle_hit->addComponent<Transform>(trParent_->getPos(), Vector2D(), trParent_->getW()*0.75, trParent_->getW()*0.75, 0.0);
		particle_hit->addComponent<Animation>("hit", &sdlutils().images().at(name), 2, 3, 6, 30, 0);
		particle_hit->getComponent<Animation>()->setAlpha(160);
	}
	SDL_Delay(App::FPS);	//Se salta un frame
}

int Enemy_Health::getMaxHealth() {
	return initLifes;
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
	//Reset color
	if (!skip_reset_) {
		entity_->getComponent<AnimBlendGraph>()->setColor(255, 255, 255);
	}
	else {
		if (cd_reset_ <= 0) {
			//Esperar cd_reset_ ticks para el reset de color
			skip_reset_ = false;
		}
		cd_reset_--;
	}
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
				entity_->getComponent<AnimBlendGraph>()->setParamValue("Dead", 1);
			}
			barSize.setX((lifes * initBarSize.getX()) / initLifes);
			if (barSize.getX() <= 0) barSize.setX(0);

			//Cambio color 
			entity_->getComponent<AnimBlendGraph>()->setColor(207, 59, 82);
			skip_reset_ = true;
			cd_reset_ = 6;
			//Particula HIT
			if (particle_hit == nullptr) {
				std::string name;
				int z = sdlutils().rand().teCuoto(0, 3);
				switch (z)
				{
				case 0:
					name = "enemy_hit_1";
					break;
				case 1:
					name = "enemy_hit_2";
					break;
				case 2:
					name = "enemy_hit_3";
					break;
				}
				particle_hit = entity_->getMngr()->addEntity();
				particle_hit->addComponent<Transform>(trParent_->getPos(), Vector2D(), trParent_->getW() * 0.75, trParent_->getH() * 0.75, 0.0);
				particle_hit->addComponent<Animation>("hit", &sdlutils().images().at(name), 2, 3, 6, 30, 0);
				particle_hit->getComponent<Animation>()->setAlpha(160);
			}
			SDL_Delay(App::FPS);	//Se salta un frame
		}
	}
	//Particula Hit
	if (particle_hit != nullptr) {
		if (particle_hit->getComponent<Animation>()->getState() == 0) {
			particle_hit->setActive(false);
			particle_hit = nullptr;
		}
		else {
			particle_hit->getComponent<Transform>()->getPos() = entity_->getComponent<Transform>()->getPos();
		}
	}
	//Particula Coins
	if (particle_coins != nullptr) {
		if (particle_coins->getComponent<Animation>()->getState() == 0) {
			particle_coins->setActive(false);
			particle_coins = nullptr;
		}
		else {
			particle_coins->getComponent<Transform>()->getPos() = entity_->getComponent<Transform>()->getPos();
		}
	}
	if (entity_->getComponent<AnimBlendGraph>()->getCurrentAnimation()->getID() == "death" && entity_->getComponent<AnimBlendGraph>()->isComplete()) {
		entity_->setActive(false);
		//Tenemos que eliminar el Trigger del enemigo (si tiene)
		EnemyTrigger* enT = entity_->getComponent<EnemyTrigger>();
		if (enT != nullptr) enT->getTriggerEntity()->setActive(false);
		if (particle_hit != nullptr) particle_hit->setActive(false);
		//Dar recompensas
		entity_->getMngr()->getHandler<Player>()->getComponent<Inventory>()->addCoins(reward_);
		int rand = sdlutils().rand().teCuoto(0,10);
		if (rand == 0)
			entity_->getMngr()->getHandler<Player>()->getComponent<Inventory>()->addBaits(1);
		SDL_Delay(100);
	}
}

