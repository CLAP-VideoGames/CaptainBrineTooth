#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Entity.h"
#include "Transform.h"
#include "../game/App.h"

typedef struct {
	Texture* tex_;
	float scrollRatio_;   //factor de seguimiento
}Layer;

class ParallaxScroll : public Component
{
public:
	ParallaxScroll(){};
	virtual ~ParallaxScroll() {};
	void init()override;
	void render()override;

	void addLayer(Texture* text, float scrollRatio);
private:
	Transform* tr_;
	std::vector<Layer*> layers_;
};