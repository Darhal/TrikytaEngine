#include <LuaEvents/LuaEvents.h>
#include "EventManager.h"
#include "core/Common/EngineInstance.h"
#include <PhysicsEngine/Core/Box2D.h>
#include <UI/UIManager.h>
#include <UI/Events/UIEventHandler.h>

EventManager* EventManager::_EventManager = 0;

EventManager* EventManager::GetEventManager()
{
	if (_EventManager == nullptr)
	{
		_EventManager = new EventManager();
	}
	return _EventManager;
}

void EventManager::HandleSDLEvents(SDL_Event& p_Event, EngineInstance* p_Engine)
{
	while (SDL_PollEvent(&p_Event) != 0) {
		if (p_Event.type == SDL_QUIT) { p_Engine->setEngineState(false); p_Engine->On_Engine_Quit(); };
		if (p_Event.type == SDL_KEYDOWN || p_Event.type == SDL_KEYUP) {
			if (p_Event.type == SDL_KEYDOWN && p_Event.key.keysym.sym == SDLK_DOLLAR) {
				Console::getConsole()->Activate(!Console::getConsole()->IsActive());
			}
			for (auto inputCallbackFunc : m_ON_KEYBOARD_INPUT_Callbacks) {
				inputCallbackFunc(p_Event.key.keysym.sym, p_Event.type);
			}
			LuaEngine::LuaEvents::GetLuaEventMnager()->CallLuaEventFunctions
			(
				Events::ON_KEYBOARD_INPUT, "ii>", 
				p_Event.key.keysym.sym, p_Event.type
			);
		}
		if (p_Event.type == SDL_MOUSEBUTTONDOWN || p_Event.type == SDL_MOUSEBUTTONUP || p_Event.type == SDL_MOUSEWHEEL) {
			for (auto mouseClickCallbackFunc : m_ON_MOUSE_CLICK_Callbacks) {
				Vec2i ClickMousePosition(p_Event.button.x, p_Event.button.y);
				mouseClickCallbackFunc(p_Event.type, p_Event.button.button, ClickMousePosition);
			}
			LuaEngine::LuaEvents::GetLuaEventMnager()->CallLuaEventFunctions
			(
				Events::ON_MOUSE_CLICK, "iiii>", 
				p_Event.type, p_Event.button.button, 
				p_Event.button.x, p_Event.button.y
			);
		}
		if (p_Event.type == SDL_MOUSEMOTION) {
			for (auto mouseMoveCallbackFunc : m_ON_MOUSE_MOVE_Callbacks) {
				Vec2i LastMousePosition(p_Event.motion.x, p_Event.motion.y);
				mouseMoveCallbackFunc(LastMousePosition);
			}
			LuaEngine::LuaEvents::GetLuaEventMnager()->CallLuaEventFunctions
			(
				Events::ON_MOUSE_MOVE, "ii>",
				p_Event.motion.x, p_Event.motion.y
			);
		}
		//Console::getConsole()->getCmdManager()->PorcessEvents(p_Event);
		UI::Manager::getEventManager()->processEvents(p_Event);
	}
}

void EventManager::HandleOnEngineLoadEvents()
{
	for (auto onEngineLoadCallback : m_ON_ENGINE_LOAD_Callbacks) {
		onEngineLoadCallback();
	}
	LuaEngine::LuaEvents::GetLuaEventMnager()->CallLuaEventFunctions
	(
		Events::ON_ENGINE_LOAD, 
		""
	);
}

void EventManager::HandleOnEnginePreInitEvents()
{
	for (auto onEnginePreInitCallback : m_ON_ENGINE_PRE_INIT_Callbacks) {
		onEnginePreInitCallback();
	}
	LuaEngine::LuaEvents::GetLuaEventMnager()->CallLuaEventFunctions
	(
		Events::ON_ENGINE_PRE_INIT,
		""
	);
}

void EventManager::HandleOnEngineRenderEvents(float dt)
{
	for (auto onEngineRenderCallback : m_ON_ENGINE_RENDER_Callbacks) {
		onEngineRenderCallback(dt);
	}
	LuaEngine::LuaEvents::GetLuaEventMnager()->CallLuaEventFunctions
	(
		Events::ON_ENGINE_RENDER,
		"d",
		dt
	);
	//printf("dt : %f\n", dt);
}