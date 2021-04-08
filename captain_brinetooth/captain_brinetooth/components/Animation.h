#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Entity.h"
#include "Transform.h"


#include "../game/App.h"


enum State { Complete, Playing, Stop };	//Estado de la animacion

class Animation : public Component
{
public:
	//Constructora de una animacion por defecto a partir de una textura
	//repeat -1, es un bucle
	Animation(std::string id, Texture* tex, int rows, int cols, int frames, Uint32 framerate, int repeat) :
		id_(id), tex_(tex), frames_(frames), framerate_(framerate), repeat_(repeat), startfr_(0), endfr_(frames - 1){	//startfr_ y endfr_ se inicializan por defecto en esta constructora
		framewidth_ = tex->width() / cols;
		frameheight_ = tex->height() / rows;
	}
	Animation(std::string id, Texture* tex, int rows, int cols, int frames, Uint32 framerate, int repeat, int startfr, int endfr) :
		id_(id), tex_(tex), frames_(frames), framerate_(framerate), repeat_(repeat), startfr_(startfr), endfr_(endfr) {	//startfr_ y endfr_ se inicializan por defecto en esta constructora
		framewidth_ = tex->width() / cols;
		frameheight_ = tex->height() / rows;
	}

	virtual ~Animation() {}

	void init() override {
		tr_ = entity_->getComponent<Transform>();
		assert(tr_ != nullptr);
		//Asignacion en memoria de las posiciones de los frames
		framepos_.reserve(frames_);
		for (int i = 0; i < frames_; i++) {
			Vector2D* p = new Vector2D(framewidth_ * i % tex_->width() / framewidth_,  framewidth_ * i / tex_->width());
			framepos_.emplace_back(p);
		}
		actfr_ = startfr_;	//1er frame es el inicial
		state_ = Playing;		//No empieza la animacion de base
		lastUpdateTime = sdlutils().currRealTime();
		flip_horizontal_ = false;
	}

	void render() override {
		if(state_ == Playing){
			//Actualiza el frame segun el framerate
			Uint32 frameTime = sdlutils().currRealTime() - lastUpdateTime;
			if (frameTime >= (1000 / framerate_)) {
				nextFrame();
				lastUpdateTime = sdlutils().currRealTime();
			}
		}
		
		//Construccion de los rectangulos fuente(textura) y destino (entidad)
		SDL_Rect src = build_sdlrect(framepos_[actfr_].getX() * framewidth_, framepos_[actfr_].getY() * frameheight_, framewidth_, frameheight_);
		SDL_Rect dest = build_sdlrect(tr_->getPos().getX() - App::camera.x, tr_->getPos().getY() - App::camera.y, tr_->getW(), tr_->getH());
		SDL_RendererFlip flip;
		if (flip_horizontal_) { flip = SDL_FLIP_HORIZONTAL; }
		else { flip = SDL_FLIP_NONE; }
		tex_->render(src, dest, tr_->getRot(),nullptr,flip);
	}

	void nextFrame() { 
		actfr_++;
		if (actfr_ > endfr_) {
			if (repeat_ < 0)	//LOOP
				actfr_ = startfr_;
			else if (repeat_ == 0) {	//Fin de la animacion
				state_ = Complete;
				actfr_ = endfr_;
			}
			else {	//NO LOOP, pero con repeticiones
				repeat_--;
				actfr_ = startfr_;
			}
		}
	}

	void stop() { if(state_ != Complete) state_ = Stop; }
	void play() {state_ = Playing; }
	void reset() { actfr_ = startfr_; }
	void flipX(bool state) { flip_horizontal_ = state; }
	const State& getState() const { return state_; }
	const std::string& getID() const { return id_; }

protected:
	Texture* tex_;	
	Transform* tr_;
	std::string id_;	//Identificador
	int framewidth_, frameheight_, frames_;		
	std::vector<Vector2D> framepos_;	//Posicion de frames no vacios de la textura
	int startfr_, endfr_, actfr_;		//Frame inicial, final y actual de la animacion
	State state_;
	Uint32 framerate_;	
	int repeat_;
	Uint32 lastUpdateTime;
	bool flip_horizontal_;
};

