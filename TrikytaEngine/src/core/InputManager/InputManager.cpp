#include "InputManager.h"
#include <SDL/SDL.h>
#include "core/Common/TrikytaEngine.h"

InputManager* InputManager::_InputManager = nullptr;

InputManager* InputManager::initInputManager()
{
	_InputManager = new InputManager();
	return _InputManager;
}

InputManager* InputManager::getInputManager()
{
	return _InputManager;
}

void InputManager::ActivateInput(bool isActive)
{
	if (isActive) {
		if (activeCount == 0) {
			SDL_StartTextInput();
		}
		activeCount++;
	}else {
		activeCount--;
		if (activeCount < 0) {
			SDL_StopTextInput();
			activeCount = 0;
		}
	}
	if (activeCount == 0) {
		SDL_StopTextInput();
	}
}

void InputManager::DrawCursor(float dt)
{
	if (!activeCount) { return; };
	LastDeltaTime = LastDeltaTime + dt;
	if (LastDeltaTime > 0.2f) {
		if (IsShowCursor) {
			IsShowCursor = false;
		}
		else {
			IsShowCursor = true;
		}
		LastDeltaTime = 0;
	}
	if (IsShowCursor) {
		SDL_SetRenderDrawColor(ENGINE->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF); // white
		SDL_RenderFillRect(ENGINE->getRenderer(), &mCursorDim); // cursor
		SDL_SetRenderDrawColor(ENGINE->getRenderer(), 0x00, 0x00, 0x00, 0xFF); // set back to black
	}
}

void InputManager::setCurosrPosition(const Vec2i& pos,const Vec2i& size)
{
	mCursorDim = SDL_Rect{pos.x + size.x, pos.y + 2, 1, size.y - 3};
}