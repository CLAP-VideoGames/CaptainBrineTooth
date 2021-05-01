#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/SDLUtils.h"

class Image : public Component {
public:
	Image(Texture* tex, SDL_Rect d, std::string t);
	virtual ~Image();

	void render() override;

	void actRect(SDL_Rect d);

	void moveRect(const int& x, const int& y);

	Texture* getTexture();

	bool compareTag(std::string t);

	SDL_Rect* getDestRect();

private:
	SDL_Rect dest;
	Texture* tex_;
	std::string tag;
};