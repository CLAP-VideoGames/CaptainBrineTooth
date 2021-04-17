#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/SDLUtils.h"

class Image : public Component {
public:
	Image(Texture* tex, SDL_Rect d, std::string t) :
		dest(d), //
		tex_(tex), //
		tag(t)
	{
	}
	virtual ~Image() {
	}

	void render() override {

		tex_->render(dest);
		if (sdlutils().getDebug()){
			SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 0, 0, 255);
			SDL_RenderDrawRect(sdlutils().renderer(), &dest);
		}
	}

	void actRect(SDL_Rect d)
	{
		dest = d;
	}

	void moveRect(const int& x, const int& y){
		dest.x = x;
		dest.y = y;
	}

	Texture* tex()
	{
		return tex_;
	}

	bool compareTag(std::string t)
	{
		return (tag == t);
	}

	SDL_Rect* getDestRect()
	{
		return &dest;
	}



private:
	SDL_Rect dest;
	Texture* tex_;
	std::string tag;

	
};