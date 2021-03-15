#pragma once
#include "../ecs/Component.h"
#include "Transform.h"
#include "../sdlutils/Texture.h"
#include "../ecs/Entity.h"
#include "../sdlutils/SDLUtils.h"


class FramedImage : public Component
{
public:
	FramedImage(Texture* tex, int fils, int cols,float tanim, int empty) : tex_(tex), tr_(nullptr), time(0), size(cols, fils) {
		frame = Vector2D(0, 0);
		auto w = tex_->width() / cols;
		auto h = tex_->height() / fils;

		src = build_sdlrect(frame, w, h);
		tiempoanimacion = tanim;
		emptyframes = empty;
		frameSize = Vector2D(w, h);
	};
	virtual ~FramedImage() {};

	void init() override {
		tr_ = entity_->getComponent<Transform>();
		assert(tr_ != nullptr);
	}
	void render() override {
		SDL_Rect dest = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH());
		tex_->render(src, dest);
		if (sdlutils().currRealTime() > time +tiempoanimacion) {
			time = sdlutils().currRealTime();
			src = nextSrcRect();
		}
	}

	SDL_Rect getFrameSize() { return src; }

private:
	SDL_Rect nextSrcRect() {
		if (frame.getX() < size.getX() - 1) frame.setX(frame.getX() + 1);
		else {
			frame.setX(0);
			if (frame.getY() < size.getY() - 1) frame.setY(frame.getY() + 1);
			else {
				frame.set(0, 0);
			}
		} 
		if (frame.getY() == size.getY() - 1 && frame.getX() > size.getX() - emptyframes - 1)frame.set(Vector2D(0, 0)); //Si estamos en el ultimo sprite a recorrer
		Vector2D a(frame.getX() * frameSize.getX(), frame.getY() * frameSize.getY());
		return build_sdlrect(a, frameSize.getX(), frameSize.getY());
	}

	Transform* tr_;
	Texture* tex_;
	Uint32 time;
	Vector2D frame;	//Posicion en x y en y del frame actual
	Vector2D frameSize; //Ancho y alto de un frame
	Vector2D size; //Filas y columnas del spritesheet
	SDL_Rect src; //Rectangulo que se renderiza
	float tiempoanimacion;
	int emptyframes;
};
