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
	states.top()->popState();
	
}

void StateMachine::pushState(GameState* gameState) {
	stateToPush = gameState;
}

void StateMachine::refreshStates(){
	popState_();
	pushState_();
}

void StateMachine::popState_(){
	if (!states.empty() && states.top()->isPopped()) {
		delete states.top();
		states.pop();
	}
}

void StateMachine::pushState_(){
	//Si hay algo que meter, se pushea en la pila
	if (stateToPush != nullptr){
		states.push(stateToPush);
		states.top()->init();
		stateToPush = nullptr;
	}
}

