#pragma once
#include "../ecs/Component.h"
#include "Animation.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"

class Enemy_Health : public Component
{
public:
	Enemy_Health(int health, const Vector2D& sizeBar, const SDL_Color& color, int offsetY);
	virtual ~Enemy_Health() {};

	void init() override;
	void render() override;
	void update() override;

	void loseLife(int damage, int typeOfDamage = 0);
	void setHealth(int health) { lifes = health; };
	int getHealth() { return lifes; };

private:
	SDL_Color barColor;
	SDL_Color baseColor;
	Vector2D barSize, initBarSize;

	SDL_Rect bar, base;
	Transform* trParent_;
	int offsetY_;

	int lifes, initLifes;

	bool isPoisoned;
	int poisonDamage = 10;
	float poisonMaxTime = 10.0f, poisonCurrentTime;
	float poisonDamageTickTime = 1.0f, poisonCurrentTickTime;

	bool isElectrified;
	int electrifyDamage = 150;
};

