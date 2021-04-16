#pragma once
#include "../ecs/Component.h"
#include "../ecs/Manager.h"
#include <functional>
#include "../sdlutils/SDLUtils.h"
#include "Image.h"

const int NUM_TEXTURES = 2;

class Slider : public Component {
public:
	Slider(const Vector2D& pos, const std::pair<Vector2D, Vector2D>& sizes_, Texture* texts[NUM_TEXTURES], void (*callback)(float));
	~Slider();

	void init() override;
	void update() override;

private:
	Vector2D pos;
	std::pair<Vector2D, Vector2D> sizes;


	/*template<>*/
	std::function<void(float)> callback_;
	Texture* textures[NUM_TEXTURES] = {};

	Entity* background, *slide;
	Manager* mngr;

};
