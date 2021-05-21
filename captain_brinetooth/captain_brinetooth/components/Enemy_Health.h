#pragma once
#include "../ecs/Component.h"
#include "AnimBlendGraph.h"
#include "Animation.h"
#include  "../ecs/Entity.h"
#include  "../ecs/Manager.h"
#include "../game/App.h"
#include "EnemyTrigger.h"
#include "Inventory.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"

class Enemy_Health : public Component
{
public:
	Enemy_Health(int health, int reward, const Vector2D& sizeBar, const SDL_Color& color, int offsetY);
	virtual ~Enemy_Health();

	void init() override;
	void render() override;
	void update() override;

	void loseLife(int damage, int typeOfDamage = 0);
	void setHealth(int health) { lifes = health; };
	int getHealth() { return lifes; };

private:
	SDL_Color barColor;
	SDL_Color baseColor;
	bool skip_reset_;
	int cd_reset_;
	Vector2D barSize, initBarSize;

	Entity* particle_hit;

	SDL_Rect bar, base;
	Transform* trParent_;
	int offsetY_;

	int lifes, initLifes;

	bool isPoisoned;
	int poisonDamage = 7;
	float poisonMaxTime = 10000.0f, poisonCurrentTime;
	float poisonDamageTickTime = 1000.0f, poisonCurrentTickTime;

	bool isElectrified;
	int electrifyDamage = 150;

	int reward_;
};

