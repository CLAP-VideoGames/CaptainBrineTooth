#include "StateMachine.h"
#include "GameState.h"

StateMachine::~StateMachine() {
	while (!states.empty()) popState();
}
GameState* StateMachine::currentState() {
	return states.top();
}
void StateMachine::changeState(GameState* gameState) {
	popState();
	pushState(gameState);
}
void StateMachine::popState() {
	if (!states.empty()) {
		delete states.top();
		states.pop();
	}
}
void StateMachine::pushState(GameState* gameState) {
	states.push(gameState);
}

