#pragma once
#include "GameState.h"
#include "box2d.h"
#include "../sdlutils/InputHandler.h"

class Entity;
class TutorialState : public GameState {
public:
	TutorialState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd) {};
	virtual void init();
	virtual void update();

	void initTutorial();
	void stepMovement();
	void stepJump();
	void stepCamera();
	void stepDash();
	void stepWeapon();
	void stepAttack();
	void stepHealth();
	void receiveInput();
	//Callbacks
	static void startGame(App* app, SoundManager* snd);
	static void startTutorial(App* app, SoundManager* snd);
	//Panels
	void skipTutorial();
	void readyPanel();
	void createButton(Texture* t, Vector2D pos, Vector2D size, void(*callback)(App*, SoundManager*), bool canFadeOut_ = false);
	void clearPanel();
	void clearStep();
	//Entities
	void createPlayer(const Config& playerConfig);
	void createEnemy();
	void createRoom();
	void createBoxFloor(const Config& entityConfig);
private:
	std::vector<Entity*> panel; 
	std::vector<Entity*> stepImg;
	std::vector<Entity*> room_;
	std::vector<Vector2D> stepImgPos;
	int step_;
	bool moveLeft_, moveRight_, jump_, camera_Up_, camera_Down_, dash_, attack_, weapon_, nextStep_;
	float wait_Time = 4000, countDown;
	Entity* player;
	Entity* enemy;
	Fade* fadeComp;
};