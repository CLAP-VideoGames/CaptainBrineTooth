#include "ParallaxScroll.h"
#define _CRTDBG_MAP_ALLOC
#include<iostream>
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
ParallaxScroll::~ParallaxScroll(){
	for (std::vector<Layer*> vectorLayer : layerGroup){
		for (Layer* l : vectorLayer){
			delete l;
		}
	}

	layers_.clear();
}

void ParallaxScroll::init(){
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);
	//level 1
	addLayer(&sdlutils().images().at("bg_layer1"), 0.2);
	addLayer(&sdlutils().images().at("bg_layer2"), 0.25);
	addLayer(&sdlutils().images().at("bg_layer3"), 0.35);
	addLayer(&sdlutils().images().at("bg_layer4"), 0.4);

	layerGroup.push_back(layers_);
	layers_.clear();
	//level 3
	addLayer(&sdlutils().images().at("bg_ice_layer1"), 0.2);
	addLayer(&sdlutils().images().at("bg_ice_layer2"), 0.25);
	addLayer(&sdlutils().images().at("bg_ice_layer3"), 0.35);
	addLayer(&sdlutils().images().at("bg_ice_layer4"), 0.4);
	addLayer(&sdlutils().images().at("bg_ice_layer5"), 0.45);

	layerGroup.push_back(layers_);
	layers_.clear();
}

void ParallaxScroll::render()
{
	for (int i = 0; i < layerGroup[levelBackGround].size(); i++) {
		//Construccion de los rectangulos fuente(textura) y destino (entidad)
		Vector2D destPos = Vector2D(tr_->getPos().getX() - (tr_->getW() * 0.5f), tr_->getPos().getY() - (tr_->getH() * 0.5f));
		SDL_Rect dest = build_sdlrect(destPos.getX() - (App::camera.x* layerGroup[levelBackGround][i]->scrollRatio_), destPos.getY() - App::camera.y,
			tr_->getW(), tr_->getH());
		layerGroup[levelBackGround][i]->tex_->render(dest, 0);
	}
}


void ParallaxScroll::addLayer(Texture* text, float scrollRatio){
	if (scrollRatio > 1.0) scrollRatio = 1.0;
	else if (scrollRatio < 0.0) scrollRatio = 0.0;
	//Juan me cago en tus muertos <3 no limpias el vector al final
	Layer* l = new Layer({ text,scrollRatio });
	layers_.push_back(l);
}

void ParallaxScroll::setLevelBackground(const int& index_){
	if (index_ < layerGroup.size()){
		levelBackGround = index_;
	}
}

