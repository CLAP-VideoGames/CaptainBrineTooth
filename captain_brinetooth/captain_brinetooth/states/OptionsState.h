#pragma once
#include "GameState.h"
#include "box2d.h"

class Entity;

class OptionsState : public GameState
{
public:
	OptionsState(App* app, std::shared_ptr<b2World> mundo, SoundManager* snd);
	virtual void init();
	void update() override;
	
	static void volverMenu(App* app, SoundManager* snd);
	static void controlVolume(float value, Entity* ent);
	static void controlBrightness(float value, Entity* ent);

	//Son estaticos porque los necesito usar en otras clases
	static Entity* createVolume(Manager* mngr, const Vector2D& pos, const std::pair<Vector2D, Vector2D>& sizes, Texture* textues2[2], const int& volume);
	static Entity* createBrightness(Manager* mngr, const Vector2D& pos, const std::pair<Vector2D, Vector2D>& sizes, Texture* textues2[2], const float& brightness);
	Entity* botonVolver();
	
private:
	SDL_Rect cam;
	SDL_Window* window;
};

