#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/SDLUtils.h"
#include "../ecs/Entity.h"
#include "Transform.h"


#include "../game/App.h"


enum State { Complete, Playing, Stop };	//Estado de la animacion

class Animation : public Component
{
public:
	/// <summary>
	//Constructora de una animacion por defecto a partir de una textura
	//repeat -1, es un bucle
	//Anchor point por defecto es (0.5,0.5)
	/// </summary>
	Animation(std::string id, Texture* tex, int rows, int cols, int frames, Uint32 framerate, int repeat, Vector2D anchor = Vector2D(0.5, 0.5));
	/// <summary>
	//Constructora de una animacion a partir de una textura con inicio y fin definidos
	//repeat -1, es un bucle
	//Anchor point por defecto es (0.5,0.5)
	/// </summary>
	Animation(std::string id, Texture* tex, int rows, int cols, int frames, Uint32 framerate, int repeat, int startfr, int endfr, Vector2D anchor = Vector2D(0.5, 0.5));

	virtual ~Animation() {}

	void init() override;
	void render() override;

	void nextFrame();

	void stop();
	void play();
	void reset();
	void flipX(bool state);
	void setAlpha(Uint8 alpha);
	void setAnchor(float x, float y);
	void destSize(float w, float h);
	const State& getState() const;
	const std::string& getID() const;
	const Vector2D& srcSize() const;
	const Vector2D& anchor() const;

protected:
	Texture* tex_;	
	Transform* tr_;
	Vector2D srcSize_;
	Vector2D destSize_;
	std::string id_;	//Identificador
	int framewidth_, frameheight_, frames_;		
	std::vector<Vector2D> framepos_;	//Posicion de frames no vacios de la textura
	int startfr_, endfr_, actfr_;		//Frame inicial, final y actual de la animacion
	State state_;
	Uint32 framerate_;	
	int repeat_;
	Uint32 lastUpdateTime;
	bool flip_horizontal_;	//Flip
	Vector2D anchorPoint_;
	Uint8 alpha_;
};

