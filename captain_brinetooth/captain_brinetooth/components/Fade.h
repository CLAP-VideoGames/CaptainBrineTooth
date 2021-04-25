#pragma once
#include "../ecs/Component.h"
#include "../sdlutils/SDLUtils.h"
#include <math.h>

class Fade : public Component{
	enum class STATE_FADE{In, Out};

public:
	Fade(const Vector2D& size, const Vector2D& pos, const int& timeIn, const int& timeOut, STATE_FADE state = STATE_FADE::In);
	~Fade();

	void init();
	void render() override;
	void update() override;

	inline void setInitState(STATE_FADE state){
		state_ = state;
	}

	inline void setColor(const SDL_Color& color){
		colorFade = color;
	}

private:
	int timeIn_, timeOut_;
	float percentageIn, percentageOut;
	SDL_Color colorFade{};
	SDL_Rect dest{};
	STATE_FADE state_{};
};
