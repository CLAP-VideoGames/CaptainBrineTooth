#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"


class Player_Health : public Component {
public:
	Player_Health(Texture* tex) :
		tex_(tex)
	{
	}
	virtual ~Player_Health() {
	}

	void init() override {
	}

	void render() override {

		for (int i = 0; i < vidas; i++)
		{
			Vector2D aux = Vector2D(10 + 35 * i, 10);
			SDL_Rect dest = build_sdlrect(aux, 20, 27);
			tex_->render(dest);
		}

	}

	void loseLife()
	{
		vidas--;
	}

	int getLife()
	{
		return vidas;
	}

	void resetLifes()
	{
		vidas = 10;
	}


private:
	float vidas = 10;
	Texture* tex_;
};

