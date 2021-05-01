#include "Animation.h"

Animation::Animation(std::string id, Texture* tex, int rows, int cols, int frames, Uint32 framerate, int repeat, Vector2D anchor):
	id_(id), tex_(tex), frames_(frames), framerate_(framerate), repeat_(repeat), startfr_(0), endfr_(frames - 1), anchorPoint_(anchor) {	//startfr_ y endfr_ se inicializan por defecto en esta constructora
	framewidth_ = tex->width() / cols;
	frameheight_ = tex->height() / rows;
}

Animation::Animation(std::string id, Texture* tex, int rows, int cols, int frames, Uint32 framerate, int repeat, int startfr, int endfr, Vector2D anchor) :
	id_(id), tex_(tex), frames_(frames), framerate_(framerate), repeat_(repeat), startfr_(startfr), endfr_(endfr), anchorPoint_(anchor) {	//startfr_ y endfr_ se inicializan por defecto en esta constructora
	framewidth_ = tex->width() / cols;
	frameheight_ = tex->height() / rows;
}

void Animation::init()
{
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);
	//Asignacion en memoria de las posiciones de los frames
	framepos_.reserve(frames_);
	for (int i = 0; i < frames_; i++) {
		Vector2D* p = new Vector2D((float)(framewidth_ * i % tex_->width() / framewidth_), (float)(framewidth_ * i / tex_->width()));
		framepos_.emplace_back(p);
	}
	srcSize_ = Vector2D((float)framewidth_, (float)frameheight_);
	destSize(tr_->getW(), tr_->getH());
	actfr_ = startfr_;	//1er frame es el inicial
	state_ = Playing;		//No empieza la animacion de base
	lastUpdateTime = sdlutils().currRealTime();
	flip_horizontal_ = false;
	alpha_ = 255;
}

void Animation::render()
{
	if (state_ == Playing) {
		//Actualiza el frame segun el framerate
		Uint32 frameTime = sdlutils().currRealTime() - lastUpdateTime;
		if (frameTime >= (1000 / framerate_)) {
			nextFrame();
			lastUpdateTime = sdlutils().currRealTime();
		}
	}

	Vector2D anchor_;
	SDL_RendererFlip flip;
	if (flip_horizontal_) { 
		flip = SDL_FLIP_HORIZONTAL; 
		anchor_ = Vector2D(1 - anchorPoint_.getX(), anchorPoint_.getY());
	}
	else { 
		flip = SDL_FLIP_NONE; 
		anchor_ = anchorPoint_;
	}
	//ALPHA
	if (alpha_ > 255) alpha_ = 255;
	else if (alpha_ < 0) alpha_ = 0;
	SDL_SetTextureAlphaMod(tex_->getTexture(), alpha_);
	//Construccion de los rectangulos fuente(textura) y destino (entidad)
	Vector2D destPos = Vector2D(tr_->getPos().getX() - (destSize_.getX() * anchor_.getX()), tr_->getPos().getY() - (destSize_.getY() * anchor_.getY()));
	SDL_Rect src = build_sdlrect(framepos_[actfr_].getX() * framewidth_, framepos_[actfr_].getY() * frameheight_, framewidth_, frameheight_);
	SDL_Rect dest = build_sdlrect(destPos.getX() - App::camera.x, destPos.getY() - App::camera.y, destSize_.getX(), destSize_.getY());
	tex_->render(src, dest, tr_->getRot(), nullptr, flip);
}

void Animation::nextFrame()
{
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

void Animation::stop()
{
	if (state_ != Complete) state_ = Stop;
}

void Animation::play()
{
	state_ = Playing;
}

void Animation::reset()
{
	actfr_ = startfr_;
}

void Animation::flipX(bool state)
{
	flip_horizontal_ = state;
}

void Animation::setAlpha(Uint8 alpha)
{
	alpha_ = alpha;
}

void Animation::setAnchor(float x, float y)
{
	anchorPoint_ = Vector2D(x, y);
}

void Animation::destSize(float w, float h)
{
	destSize_ = Vector2D(w, h);
}

const State& Animation::getState() const
{
	return state_;
}

const std::string& Animation::getID() const
{
	return id_;
}

const Vector2D& Animation::srcSize() const
{
	return srcSize_;
}

const Vector2D& Animation::anchor() const
{
	return anchorPoint_;
}
