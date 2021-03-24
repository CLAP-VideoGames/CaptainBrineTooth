#include "GameStateMachine.h"
#include "GameState.h"

GameStateMachine::~GameStateMachine() {
	while (!states.empty()) popState();
}
GameState* GameStateMachine::currentState() {
	return states.top();
}
void GameStateMachine::changeState(GameState* gameState) {
	popState();
	pushState(gameState);
}
void GameStateMachine::popState() {
	if (!states.empty()) {
		delete states.top();
		states.pop();
	}
}
void GameStateMachine::pushState(GameState* gameState) {
	states.push(gameState);
}

