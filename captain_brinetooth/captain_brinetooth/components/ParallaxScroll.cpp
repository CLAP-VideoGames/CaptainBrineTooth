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
	layers_.push_back(addLayer(&sdlutils().images().at("bg_layer1"), 0.2));
	layers_.push_back(addLayer(&sdlutils().images().at("bg_layer2"), 0.25));
	layers_.push_back(addLayer(&sdlutils().images().at("bg_layer3"), 0.35));
	layers_.push_back(addLayer(&sdlutils().images().at("bg_layer4"), 0.4));
	layerGroup.push_back(layers_);
	layers_.clear();

	//level2
	layers_.push_back(addLayer(&sdlutils().images().at("bg_aztec_layer1"), 0.2));
	layers_.push_back(addLayer(&sdlutils().images().at("bg_aztec_layer2"), 0.3));
	layers_.push_back(addLayer(&sdlutils().images().at("bg_aztec_layer3"), 0.4));
	layers_.push_back(addLayer(&sdlutils().images().at("bg_aztec_layer4"), 0.5));
	layers_.push_back(addLayer(&sdlutils().images().at("bg_aztec_layer5"), 0.6));
	layerGroup.push_back(layers_);
	layers_.clear();

	//level 3
	layers_.push_back(addLayer(&sdlutils().images().at("bg_ice_layer1"), 0.2));
	layers_.push_back(addLayer(&sdlutils().images().at("bg_ice_layer2"), 0.25));
	layers_.push_back(addLayer(&sdlutils().images().at("bg_ice_layer3"), 0.35));
	layers_.push_back(addLayer(&sdlutils().images().at("bg_ice_layer4"), 0.4));
	layers_.push_back(addLayer(&sdlutils().images().at("bg_ice_layer5"), 0.45));
	layerGroup.push_back(layers_);
	layers_.clear();
}

void ParallaxScroll::render(){
	for (int i = 0; i < layerGroup[levelBackGround].size(); i++) {
		//Construccion de los rectangulos fuente(textura) y destino (entidad)
		Vector2D size;
		Vector2D pos;
		if (levelBackGround == 0) {
			size = Vector2D(tr_->getW(), tr_->getH());
			pos = Vector2D(960, 640);
		}
		else if (levelBackGround == 1) {
			size = Vector2D(2560, 2560);
			pos = Vector2D(900, 1280);
		}
		else if (levelBackGround == 2) {
			size = Vector2D(2560, 2560);
			pos = Vector2D(900, 1280);
		}
		else {
			size = Vector2D(tr_->getW(), tr_->getH());
			pos = Vector2D(960, 640);
		}

		Vector2D destPos = Vector2D(pos.getX() - (size.getX() * 0.5f), pos.getY() - (size.getY() * 0.5f));
		SDL_Rect dest = build_sdlrect(destPos.getX() - (App::camera.x* layerGroup[levelBackGround][i]->scrollRatio_), destPos.getY() - App::camera.y,
			size.getX(), size.getY());
		layerGroup[levelBackGround][i]->tex_->render(dest, 0);
	}
}

void ParallaxScroll::addLayerToGroup(Texture* text, float scrollRatio, int indexGroup){
	//Si se pasa, que salte un error
	assert(indexGroup < layerGroup.size());

	layerGroup[indexGroup].push_back(addLayer(text, scrollRatio));
}

void ParallaxScroll::addGroupLayer(std::vector<std::pair<Texture*, float>> groupLayer){
	for (std::pair<Texture*, float> layer : groupLayer){
		layers_.push_back(addLayer(layer.first, layer.second));
	}

	layerGroup.push_back(layers_);
	layers_.clear();
}

Layer* ParallaxScroll::addLayer(Texture* text, float scrollRatio){
	if (scrollRatio > 1.0) scrollRatio = 1.0;
	else if (scrollRatio < 0.0) scrollRatio = 0.0;
	//Juan me cago en tus muertos <3 no limpias el vector al final
	return  new Layer({ text,scrollRatio });
}

void ParallaxScroll::setLevelBackground(const int& index_){
	if (index_ < layerGroup.size()){
		levelBackGround = index_;
	}
	else if(index_ >= layerGroup.size()){
		//Si el indice recibido se pasa, pone el ultimo grupo de layers
		levelBackGround = layerGroup.size() - 1;
	}
}

