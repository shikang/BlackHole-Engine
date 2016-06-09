#ifndef BH_EVENT_MANAGER_H
#define BH_EVENT_MANAGER_H

#include "Platform/Mutex.h"

#include "Core/CoreEssential.h"
#include "Core/Singleton.h"
#include "Core/Hash.h"
#include "Core/Delegate.h"
#include "Core/Event.h"
#include "Core/LogManager.h"

#include <unordered_map>
#include <utility>

namespace BH
{
	class EventManager
	{
	public:
		DECLARE_SINGLETON(EventManager);	//!< Declare Event Manager

	public:
		typedef std::unordered_map<Hash, void *> EventMap;

	public:
		// Destructor
		~EventManager();
    
		// Register Event
		template< typename... Arguments >
		void RegisterEvent(const String & name, const Delegate< void(Arguments...) > & d);

		// Unregister Event
		template< typename... Arguments >
		void UnregisterEvent(const String & name, const Delegate< void(Arguments...) > & d);

		/*
			Raise event of specified name

			Usage:
				#define EVENT1_ARGS const A &, B &&
				EVENT_MANAGER.RegisterEvent("Event1", BH::Delegate<void(EVENT1_ARGS)>::FromFunction<&Event1Function>());
				RaiseEvent<EVENT1_ARGS>("Event1, a, b);

			Reason for instantiation is because:
				RaiseEvent("Event1, a, b);			//!< RaiseEvent<A, B>
		*/
		template< typename... Arguments >
		void RaiseEvent(const String & name, Arguments... args);
    
		// Delete event by name
		void DestroyEvent(const String & name);

		// Register Event (It does not use the object pointer inside the delegate class)
		template< typename... Arguments >
		void RegisterObjectEvent(const String & name, const Delegate< void(Arguments...) > & d);

		// Unregister Event (It does not use the object pointer inside the delegate class)
		template< typename... Arguments >
		void UnregisterObjectEvent(const String & name, const Delegate< void(Arguments...) > & d);

		// Raise event of specified name
		template < typename... Arguments >
		void RaiseObjectEvent(const String & name, void * obj, Arguments... args);

		// Delete event by name
		void DestroyObjectEvent(const String & name);

	private:
		// Constructor
		EventManager();

		// Create new event
		template< typename... Arguments >
		Event< void(Arguments...) > * AddEvent(const Hash & hash, EventMap & events, Mutex & mutex);

		// Return null if not found
		template< typename... Arguments >
		Event< void(Arguments...) > * GetEvent(const Hash & hash, const EventMap & events, Mutex & mutex);

		// Clear all events
		void ClearEvents(EventMap & events);

		// Register Event Implementation
		template< typename... Arguments >
		void RegisterEventImpl(const String & name, EventMap & events, Mutex & mutex, const Delegate< void(Arguments...) > & d);

		// Unregister Event Implementation
		template< typename... Arguments >
		void UnregisterEventImpl(const String & name, EventMap & events, Mutex & mutex, const Delegate< void(Arguments...) > & d);

		// Raise event Implementation
		template< typename... Arguments, typename Function >
		void RaiseEventImpl(const String & name, EventMap & events, Mutex & mutex, Function & f);

		// Delete event Implementation
		void DestroyEventImpl(const String & name, EventMap & events, Mutex & mutex);

	private:
		EventMap mEvents;									//!< Events
		MUTEX_DECLARE(mEventsMutex);						//!< Mutex for thread safety

		EventMap mObjectEvents;								//!< Events for a specific object
		MUTEX_DECLARE(mObjectMutex);						//!< Mutex for thread safety

	};
}

#define EVENT_MANAGER\
	::BH::EventManager::Instance()

#include "Core/EventManager.inl"

#endif
