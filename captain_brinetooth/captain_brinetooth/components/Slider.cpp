#include "Slider.h"

Slider::Slider(const Vector2D& pos_, const std::pair<Vector2D, Vector2D>& sizes_, Texture* texts[NUM_TEXTURES], void(*callback)(float)){
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
	SDL_Rect backgroundPos = build_sdlrect(pos, sizes.first.getX(), sizes.first.getY());
	background->addComponent<Image>(textures[0], backgroundPos, "fondo");


	slide = mngr->addEntity();
	Vector2D posSlide = Vector2D(pos.getX()* 1.1f, pos.getY());
	SDL_Rect slideRect = build_sdlrect(posSlide, sizes.second.getX(), sizes.second.getY());
	background->addComponent<Image>(textures[1], backgroundPos, "fondo");
	float m = 3.4f;
	callback_(m);
}

void Slider::update()
{
}
