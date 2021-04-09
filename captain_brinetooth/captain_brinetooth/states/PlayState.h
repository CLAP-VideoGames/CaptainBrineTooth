#pragma once
#include "GameState.h"
class Entity;

class PlayState : public GameState
{
public:
	PlayState(App* a, b2World* mundo, SoundManager* snd);
	virtual void init();

	void createLevel0();
	void createPlayer(const Config& playerConfig);
	void createBoxTest(const Config& entityConfig);
	void createElfShark(const Config& entityConfig);

private:
	SDL_Rect cam;
	MapProcedural* map;
};
