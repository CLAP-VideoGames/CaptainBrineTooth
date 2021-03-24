#pragma once
#include <stack>
#include <list>
class GameState;
using namespace std;

class GameStateMachine
{
private:
	stack<GameState*> states;
public:
	GameStateMachine() {};
	~GameStateMachine();
	GameState* currentState();
	void changeState(GameState* gameState);
	void popState();
	void pushState(GameState* gameState);
};