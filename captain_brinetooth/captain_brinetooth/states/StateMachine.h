#pragma once
#include <stack>
class GameState;
using namespace std;

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
	stack<GameState*> states;
	void popState_();
	void pushState_();

	GameState* stateToPush;
};