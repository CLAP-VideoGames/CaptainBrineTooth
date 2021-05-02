#include "ParallaxScroll.h"
#define _CRTDBG_MAP_ALLOC
#include<iostream>
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
ParallaxScroll::~ParallaxScroll(){

	for (Layer* l : layers_)
		delete l;

	layers_.clear();
}

void ParallaxScroll::init()
{
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);
}

void ParallaxScroll::render()
{
	for (int i = 0; i < layers_.size(); i++) {
		//Construccion de los rectangulos fuente(textura) y destino (entidad)
		Vector2D destPos = Vector2D(tr_->getPos().getX() - (tr_->getW() * 0.5f), tr_->getPos().getY() - (tr_->getH() * 0.5f));
		SDL_Rect dest = build_sdlrect(destPos.getX() - (App::camera.x*layers_[i]->scrollRatio_), destPos.getY() - App::camera.y,
			tr_->getW(), tr_->getH());
		layers_[i]->tex_->render(dest, 0);
	}
}


void ParallaxScroll::addLayer(Texture* text, float scrollRatio)
{
	if (scrollRatio > 1.0) scrollRatio = 1.0;
	else if (scrollRatio < 0.0) scrollRatio = 0.0;
	//Juan me cago en tus muertos <3 no limpias el vector al final
	Layer* l = new Layer({ text,scrollRatio });
	layers_.push_back(l);
}

