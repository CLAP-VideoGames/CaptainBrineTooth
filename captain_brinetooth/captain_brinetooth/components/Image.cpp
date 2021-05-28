#include "Image.h"

Image::Image(Texture* tex, SDL_Rect d, std::string t) :
	dest(d), //
	tex_(tex), //
	tag(t)
{
}

Image::~Image()
{
}

void Image::render() {

	tex_->render(dest);
	#ifdef _DEBUG
	if (sdlutils().getDebug()) {
		SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdlutils().renderer(), &dest);
	}
	#endif
}

void Image::actRect(SDL_Rect d)
{
	dest = d;
}

void Image::setDestRect(int x, int y, int w, int h) {
	dest.x = x;
	dest.y = y;
}

Texture* Image::getTexture()
{
	return tex_;
}

bool Image::compareTag(std::string t)
{
	return (tag == t);
}

SDL_Rect* Image::getDestRect()
{
	return &dest;
}
