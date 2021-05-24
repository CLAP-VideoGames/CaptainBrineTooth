#include "GameState.h"
#pragma once

class SkillTreeState : public GameState
{
public:
	SkillTreeState(GameState* stateToRender, App* a, std::shared_ptr<b2World> mundo, SoundManager* snd, Entity* player);

	virtual void init();
	virtual void render() const;
	virtual void update();
	
	static void activateExtraLives(App* app, SoundManager* snd);
	static void activateExtraHeals(App* app, SoundManager* snd);
	static void activateSpines(App* app, SoundManager* snd);
	static void activateDoubleDamage(App* app, SoundManager* snd);
	static void activateSpeedAttack(App* app, SoundManager* snd);


	void createTable();

	void setExtraLives();

	void setExtraHeal();

	void setSpines();

	void setDoubleDamage();

	void setSpeedAttack();

private:
	SDL_Rect cam;
	GameState* stRend;
	Entity* player;
	SkillTree* skillTree_;
	Player_Health* plH;
	int points;
	vector<ExplanationMessage*> explMssgs_;
};
