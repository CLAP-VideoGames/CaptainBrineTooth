#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"


class Player_Health : public Component {
public:
	Player_Health(Texture* tex) :
		tex_(tex)
	{
	}
	virtual ~Player_Health() {}

	void init() override {}

	void render() override;

	void loseLife() { vidas--; }

	int getLife(){ return vidas; }

	void resetLifes() { vidas = 10; }

private:
	float vidas = 10;
	Texture* tex_;
};

