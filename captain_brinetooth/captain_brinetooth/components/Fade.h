#pragma once
#include "../ecs/Component.h"
#include "../sdlutils/SDLUtils.h"
#include <math.h>
#include "../game/App.h"

class Fade : public Component{
public:
	enum class STATE_FADE{In, Out};

	Fade(const Vector2D& size, const Vector2D& pos, const int& timeIn, const int& timeOut, STATE_FADE state = STATE_FADE::In);
	~Fade();

	void init();
	void render() override;
	void update() override;

	inline void setState(STATE_FADE state){
		state_ = state;
	}

	inline void setColor(const SDL_Color& color){
		colorFade = color;
	}

	inline bool getFadeInComplete() const {
		return fadeInComplete;
	}
	
	inline bool getFadeOutComplete() const {
		return fadeOutComplete;
	}

	/// <summary>
	/// Inicia el Fade seg˙n el estado en que estÅE
	/// </summary>
	void triggerFade();

	/// <summary>
	/// Asigna el tiempo de Fade Out en milisegundos
	/// </summary>
	/// <param name="time"> en milisegundos</param>
	void setTimeIn(int time);
	
	/// <summary>
	/// Asigna el tiempo de Fade Out en milisegundos
	/// </summary>
	/// <param name="time"> en milisegundos</param>
	void setTimeOut(int time);

private:

	int timeIn_, timeOut_;
	bool play;
	float percentageIn, percentageOut;

	bool fadeOutComplete = false, fadeInComplete = false;

	SDL_Color colorFade{};
	SDL_Rect dest{};
	STATE_FADE state_{};
};
