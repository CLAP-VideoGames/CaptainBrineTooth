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
	ParallaxScroll(): levelBackGround(0){};
	virtual ~ParallaxScroll();
	void init()override;
	void render()override;

	void addLayer(Texture* text, float scrollRatio);

	void setLevelBackground(const int& index_);

private:
	Transform* tr_;
	std::vector<Layer*> layers_;

	std::vector<std::vector<Layer*>> layerGroup;
	
	int levelBackGround;
};