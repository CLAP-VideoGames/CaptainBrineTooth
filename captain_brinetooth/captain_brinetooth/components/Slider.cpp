#include "Slider.h"

Slider::Slider(const Vector2D& pos_, const std::pair<Vector2D, Vector2D>& sizes_, Texture* texts[NUM_TEXTURES], void(*callback)(float, Entity*)){
	pos = pos_;
	sizes = sizes_;
	for (int i = 0; i < NUM_TEXTURES; i++) textures[i] = texts[i];

	callback_ = callback;

}

Slider::~Slider()
{
}

void Slider::init(){
	mngr = entity_->getMngr();

	background = mngr->addEntity();
	backgroundSlideRct = build_sdlrect(pos, sizes.first.getX(), sizes.first.getY());
	background->addComponent<Image>(textures[0], backgroundSlideRct, "fondo");


	slide = mngr->addEntity();
	Vector2D posSlide = Vector2D(pos.getX()* 1.1f, pos.getY() - (sizes.second.getY()*0.5f));
	SDL_Rect slideRect = build_sdlrect(posSlide, sizes.second.getX(), sizes.second.getY());
	slide->addComponent<Image>(textures[1], slideRect, "fondo");
	
}

void Slider::update(){
	SDL_Point mouseP = { ih().getMousePos().first, ih().getMousePos().second };
	SDL_Rect dest = slide->getComponent<Image>()->destino();

	if (SDL_PointInRect(&mouseP, &dest) == SDL_TRUE) {
		if (ih().mouseButtonEvent()){
			int m = 5;
			callback_(m, entity_);
		}
	}

}

