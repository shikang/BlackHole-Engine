// Precompiled Header
#include "Script/Stdafx.h"

#include "Script/ScriptEventManager.h"

namespace BH
{
	ScriptEventManager::ScriptEventManager()
	{
		mScriptEventManager = Script::CreateScriptObject( "BHEventManager", "gEventManager" );
	}

	ScriptEventManager::~ScriptEventManager()
	{
		Script::DestroyScriptObject( mScriptEventManager );
	}

	void ScriptEventManager::RaiseScriptKeyTriggeredEvent( Key::KeyCode keycode )
	{
		mScriptEventManager.CallMethod( "RaiseKeyTriggeredEvent", static_cast<s32>( keycode ) );
	}

	void ScriptEventManager::RaiseScriptKeyReleasedEvent( Key::KeyCode keycode )
	{
		mScriptEventManager.CallMethod( "RaiseKeyReleasedEvent", static_cast<s32>( keycode ) );
	}

	void ScriptEventManager::RaiseScriptKeyPressedEvent( Key::KeyCode keycode )
	{
		mScriptEventManager.CallMethod( "RaiseKeyPressedEvent", static_cast<s32>( keycode ) );
	}

	void ScriptEventManager::RaiseScriptMouseTriggeredEvent( s32 x, s32 y, Mouse::Button mousecode )
	{
		mScriptEventManager.CallMethod( "RaiseMouseTriggeredEvent", x, y, static_cast<s32>( mousecode ) );
	}

	void ScriptEventManager::RaiseScriptMouseReleasedEvent( s32 x, s32 y, Mouse::Button mousecode )
	{
		mScriptEventManager.CallMethod( "RaiseMouseReleasedEvent", x, y, static_cast<s32>( mousecode ) );
	}

	void ScriptEventManager::RaiseScriptMousePressedEvent( s32 x, s32 y, Mouse::Button mousecode )
	{
		mScriptEventManager.CallMethod( "RaiseMousePressedEvent", x, y, static_cast<s32>( mousecode ) );
	}
}