#include "../states/GameState.h"
#include "../states/MenuState.h"

#pragma once
class StoryState : public GameState {
public:
	StoryState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd);
	~StoryState();

	void init() override;
	void update() override;
	void render() const override;

private:
	Entity* video = nullptr, * fade = nullptr;
	Fade* fadeComp = nullptr;

	VideoPlayer* videoP = nullptr;
	Texture* skip;
	Vector2D pos;

	bool finalSong = false;
};