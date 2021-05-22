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
	virtual ~Player_Health();

	void init() override;

	void render() override;

	void update() override;
	
	/// <summary>
	/// Crea una entidad que simboliza los frascos de cura del jugador
	/// </summary>
	void createHeal();

	/// <summary>
	/// 
	/// </summary>
	void loseLife();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="n"></param>
	void setMaxLifes(int n);
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="n"></param>
	void setMaxHeals(int n);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="life"></param>
	void setLife(float life);

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	float getMaxLifes();	
	
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	int getMaxHeals();

	/// <summary>
	/// 
	/// </summary>
	int getLife();

	/// <summary>
	/// 
	/// </summary>
	void resetLifes();

	/// <summary>
	/// 
	/// </summary>
	void heal();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="charge"></param>
	void chargeHeal(int charge);


	/// <summary>
	/// 
	/// </summary>
	void respawn();
	
	/// <summary>
	/// 
	/// </summary>
	const bool& getPlayerIsDying();

	/// <summary>
	/// 
	/// </summary>
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

	bool playerIsDying;
};

