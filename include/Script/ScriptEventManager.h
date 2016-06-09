#ifndef BH_SCRIPT_EVENTMANAGER_H
#define BH_SCRIPT_EVENTMANAGER_H

#include "Core/CoreEssential.h"
#include "Core/FunctionsFramework.h"
#include "Platform/Keybind.h"
#include "Script/ScriptConfig.h"
#include "Script/ScriptMethod.h"
#include "Script/ScriptObject.h"

namespace BH
{
	// Script Event Manager that raise C# event in C++
	class BH_API ScriptEventManager
	{
	public:
		// Constructor
		ScriptEventManager();

		// Destructor
		~ScriptEventManager();

	public:
		// Key Event callback
		void RaiseScriptKeyTriggeredEvent( Key::KeyCode keycode );
		void RaiseScriptKeyReleasedEvent( Key::KeyCode keycode );
		void RaiseScriptKeyPressedEvent( Key::KeyCode keycode );

		// Mouse Event callback
		void RaiseScriptMouseTriggeredEvent( s32 x,s32 y, Mouse::Button mousecode );
		void RaiseScriptMouseReleasedEvent( s32 x, s32 y, Mouse::Button mousecode );
		void RaiseScriptMousePressedEvent( s32 x, s32 y, Mouse::Button mousecode );

	private:
		ScriptObject mScriptEventManager;	//!< Instance of ScriptEventManager in Script

	};
}

#endif