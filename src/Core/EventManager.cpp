// Precompiled Header
#include "Core/Stdafx.h"

#include "Core/EventManager.h"

namespace BH
{
	EventManager::EventManager()
	: mEventsMutex()
	, mObjectMutex()
	{
	}

	EventManager::~EventManager()
	{
		ClearEvents(mEvents);
		ClearEvents(mObjectEvents);
	}

	void EventManager::ClearEvents(EventMap & events)
	{
		for (auto & i : events)
		{
			delete i.second;
		}

		events.clear();
	}
}