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

	/// <summary>
	/// Anyade una layer a un grupo de capas definido por indexGroup
	/// Si el indice es mayor que el número total de capas, salta un error
	/// </summary>
	void addLayerToGroup(Texture* text, float scrollRatio, int indexGroup);

	/// <summary>
	/// Anyade un grupo de capas a la ultima posicion del vector de grupo de capas
	/// </summary>
	/// <param name="groupLayer"></param>
	void addGroupLayer(std::vector<std::pair<Texture*, float>> groupLayer);

	/// <summary>
	/// Selecciona un grupo de capas para renderizar
	/// </summary>
	/// <param name="index_"></param>
	void setLevelBackground(const int& index_);

private:
	Layer* addLayer(Texture* text, float scrollRatio);

	Transform* tr_;
	std::vector<Layer*> layers_;

	std::vector<std::vector<Layer*>> layerGroup;
	
	int levelBackGround;
};