// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <iostream>
#include <SDL.h>
#include <array>

#include "../utils/Singleton.h"

// Instead of a Singleton class, we could make it part of
// SDLUtils as well.

#define DEATH_ZONE_X 23200
#define DEATH_ZONE_Y 3200


#define MAX_ANALOG 32767

class InputHandler: public Singleton<InputHandler> {

	friend Singleton<InputHandler> ;

public:
	enum MOUSEBUTTON : uint8_t {
		LEFT = 0, MIDDLE = 1, RIGHT = 2
	};

	virtual ~InputHandler() {
	}

	// clear the state
	inline void clearState() {
		isKeyDownEvent_ = false;
		isKeyUpEvent_ = false;
		isMouseButtonEvent_ = false;
		isMouseMotionEvent_ = false;
		for (auto i = 0u; i < 3; i++) {
			mbState_[i] = false;
		}
	}

	// update the state with a new event
	inline void update(const SDL_Event &event) {
		switch (event.type) {
		case SDL_KEYDOWN:
			onKeyDown(event);
			break;
		case SDL_KEYUP:
			onKeyUp(event);
			break;
		case SDL_MOUSEMOTION:
			onMouseMotion(event);
			break;
		case SDL_MOUSEBUTTONDOWN:
			onMouseButtonChange(event, true);
			break;
		case SDL_MOUSEBUTTONUP:
			onMouseButtonChange(event, false);
			break;
		case SDL_CONTROLLERAXISMOTION:
		case SDL_CONTROLLERBUTTONDOWN:
			onKeyDown(event);
			break;
		case SDL_CONTROLLERBUTTONUP:
			onKeyUp(event);
			break;
		default:
			break;
		}
	}

	// keyboard
	inline bool keyDownEvent() {
		return isKeyDownEvent_;
	}

	inline bool isGamePadButtonDown(SDL_Scancode key, MOUSEBUTTON b) {
		bool pressGamePadButton = false;

		switch (key){
		case SDL_SCANCODE_SPACE:
			pressGamePadButton = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A);
			break;
		case SDL_SCANCODE_LSHIFT:
			pressGamePadButton = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B);
			break;
		case SDL_SCANCODE_A: {
			int ret = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);

			if (ret < 0 + DEATH_ZONE_X && ret > 0 - DEATH_ZONE_X) ret = 0;
			else if (ret < 0 - DEATH_ZONE_X)
				ret = 1;
			else ret = 0;

			pressGamePadButton = ret;
		}
			break;
		case SDL_SCANCODE_D: {
			int ret = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);

			if (ret < 0 + DEATH_ZONE_X && ret > 0 - DEATH_ZONE_X) ret = 0;
			else if (ret > 0 + DEATH_ZONE_X)
				ret = 1;
			else ret = 0;

			pressGamePadButton = ret;
		}
			break;
		default:
			break;
		}

		switch (b) {
		case InputHandler::MOUSEBUTTON::LEFT:
			pressGamePadButton = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X);
			break;
		case InputHandler::MOUSEBUTTON::MIDDLE:
			pressGamePadButton = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y);
			break;
		default:
			break;
		}


		
		return pressGamePadButton;
	}


	inline bool keyUpEvent() {
		return isKeyUpEvent_;
	}

	inline bool isKeyDown(SDL_Scancode key) {
		return (keyDownEvent() && kbState_[key] == 1) || (keyDownEvent() && isGamePadButtonDown(key, InputHandler::MOUSEBUTTON::RIGHT));
	}

	inline bool isKeyDown(SDL_Keycode key) {
		return isKeyDown(SDL_GetScancodeFromKey(key));
	}

	inline bool isKeyUp(SDL_Scancode key) {
		return keyUpEvent() && kbState_[key] == 0;
	}

	inline bool isKeyUp(SDL_Keycode key) {
		return isKeyUp(SDL_GetScancodeFromKey(key));
	}

	// mouse
	inline bool mouseMotionEvent() {
		return isMouseMotionEvent_;
	}

	inline bool mouseButtonEvent() {
		return isMouseButtonEvent_;
	}

	inline const std::pair<Sint32, Sint32>& getMousePos() {
		return mousePos_;
	}

	inline int getMouseButtonState(MOUSEBUTTON b) {
		return mbState_[b] || isGamePadButtonDown(SDL_SCANCODE_0,b);
	}

	inline bool getMouseButtonHeld(){
		return isMouseButtonHeldDown_;
	}

	inline bool getRightMouseButtonPressed() {
		return isRightMousePressed_;
	}
	
	inline bool getLeftMouseButtonPressed() {
		return isLeftMousePressed_;
	}

	// TODO add support for Joystick, see Chapter 4 of
	// the book 'SDL Game Development'

private:
	InputHandler() {
		kbState_ = SDL_GetKeyboardState(0);
		clearState();

		int i = 0;
		while (controller == nullptr && i < SDL_NumJoysticks()) {
			if (SDL_IsGameController(i))
				controller = SDL_GameControllerOpen(i);
			i++;
		}
	}

	inline void onKeyDown(const SDL_Event&) {
		isKeyDownEvent_ = true;
	}

	inline void onKeyUp(const SDL_Event&) {
		isKeyUpEvent_ = true;
	}

	inline void onGamePadButtonDown(const SDL_Event&) {
		isGamePadButtonDownEvent_ = true;
	}

	inline void onMouseMotion(const SDL_Event &event) {
		isMouseMotionEvent_ = true;
		mousePos_.first = event.motion.x;
		mousePos_.second = event.motion.y;
	}

	inline void onMouseButtonChange(const SDL_Event &event, bool isDown) {
		isMouseButtonEvent_ = true;
		isMouseButtonHeldDown_ = isDown;
		switch (event.button.button) {
		case SDL_BUTTON_LEFT:
			mbState_[LEFT] = isDown;
			isLeftMousePressed_ = isDown;
			break;
		case SDL_BUTTON_MIDDLE:
			mbState_[MIDDLE] = isDown;
			break;
		case SDL_BUTTON_RIGHT:
			mbState_[RIGHT] = isDown;
			isRightMousePressed_ = isDown;
			break;
		default:
			break;
		}
		switch (event.type)
		{
		case SDL_CONTROLLERBUTTONUP:
			isMouseButtonEvent_ = true;
			break;
		case SDL_CONTROLLERBUTTONDOWN:
			isMouseButtonEvent_ = true;
			break;
		default:
			break;
		}
	}

	bool isKeyUpEvent_;
	bool isGamePadButtonDownEvent_;
	bool isKeyDownEvent_;
	bool isMouseMotionEvent_;
	bool isMouseButtonEvent_;
	//Esto es para ver que se sigue pulsando
	bool isMouseButtonHeldDown_;
	bool isRightMousePressed_;
	bool isLeftMousePressed_;
	std::pair<Sint32, Sint32> mousePos_;
	std::array<bool, 3> mbState_;
	_SDL_GameController* controller = nullptr;
	const Uint8 *kbState_;
};

// This macro defines a compact way for using the singleton InputHandler, instead of
// writing InputHandler::instance()->method() we write ih().method()
//
inline InputHandler& ih() {
	return *InputHandler::instance();
}
