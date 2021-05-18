#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/SDLUtils.h"
#include "../game/App.h"




class Player_Health : public Component {
public:
	Player_Health(Texture* tex, Texture* tex2, Texture* tex3, float tanim, App* game, int maxLifes, int maxHeals) 
		: fVida(tex), hVida(tex2), vVida(tex3), tiempoanimacion(tanim), g(game), maxVidas(maxLifes), maxHeals_(maxHeals)
	{};
	virtual ~Player_Health() {};

	void init() override;

	void render() override;

	void update() override;

	void loseLife();

	void heal();
	void chargeHeal(int charge);

	int getLife();

	void resetLifes();

	void respawn();
	void setLife(float life);

	const bool& getInvulnerable();

private:
	float vidas, maxVidas;
	int maxHeals_, maxValueHeal_;
	std::vector<int> healsValues;
	std::vector<Entity*> heals;
	Entity* particle_heal;
	Texture* fVida;
	Texture* hVida;
	Texture* vVida;
	Vector2D frame;	// Posicion en x y en y del frame actual
	Vector2D frameSize; // Ancho y alto de un frame
	SDL_Rect src; //Rectangulo que se renderiza
	Uint32 time_; // Tiempo inicial
	//Invulnerabilidad
	bool invulnerability_;
	float cd_invul_ = 1500, elpased_time_invul_; 
	float deadCountdown, dead_time = 1500;

	App* g;
	float tiempoanimacion; // Tiempo entre frame y frame
	int nFrame = 0;
};

