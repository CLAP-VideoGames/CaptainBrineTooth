#pragma once
#include <stack>
class GameState;

class StateMachine
{
public:
	StateMachine() : stateToPush(nullptr) {};
	~StateMachine();
	GameState* currentState();
	void changeState(GameState* gameState);
	void popState();
	void pushState(GameState* gameState);
	void refreshStates();

private:
	std::stack<GameState*> states;
	void popState_();
	void pushState_();
	GameState* stateToPush;

	int timesToPop = 0;
};