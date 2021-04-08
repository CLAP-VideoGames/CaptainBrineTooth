#pragma once
#include <stack>
#include <list>
class GameState;
using namespace std;

class StateMachine
{
private:
	stack<GameState*> states;
public:
	StateMachine() {};
	~StateMachine();
	GameState* currentState();
	void changeState(GameState* gameState);
	void popState();
	void pushState(GameState* gameState);
};