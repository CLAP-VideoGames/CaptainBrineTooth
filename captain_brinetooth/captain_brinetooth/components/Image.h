#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"

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
	}

	void actualizar(SDL_Rect d)
	{
		dest = d;
	}

	Texture* tex()
	{
		return tex_;
	}

	bool compareTag(std::string t)
	{
		return (tag == t);
	}

	SDL_Rect destino()
	{
		return dest;
	}

private:
	SDL_Rect dest;
	Texture* tex_;
	std::string tag;
};