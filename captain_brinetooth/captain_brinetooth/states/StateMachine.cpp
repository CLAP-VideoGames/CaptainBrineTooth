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
	//Si el estado no ha sido declarado como Poped, lo asignamos
	if(!states.top()->isPopped())
		states.top()->popState();
	else{ //Si ya se ha asignado, significa que el estado de debjo de Top tambien será popped
		timesToPop++;
	}
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
		//Si la lista no está vacia y se quieren poppear más estados
		while (!states.empty() && timesToPop > 0){
			delete states.top();
			states.pop();
			timesToPop--;
		}
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

