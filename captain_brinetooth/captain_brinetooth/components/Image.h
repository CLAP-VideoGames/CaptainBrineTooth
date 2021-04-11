#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"

class Image : public Component {
public:
	Image(Texture* tex, SDL_Rect d) :
		dest(d), //
		tex_(tex) //
	{
	}
	virtual ~Image() {
	}

	void render() override {
		tex_->render(dest);
	}

	void actualizar(SDL_Rect d)
	{
		dest = d;
	}

	Texture* tex()
	{
		return tex_;
	}

private:
	SDL_Rect dest;
	Texture* tex_;
};