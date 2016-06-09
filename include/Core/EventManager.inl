namespace BH
{
	template< typename... Arguments >
	void EventManager::RegisterEventImpl(const String & name, EventMap & events, Mutex & mutex, const Delegate< void(Arguments...) > & d)
	{
		Hash hash(name);
		Event< void(Arguments...) > * e = GetEvent<Arguments...>(hash, events, mutex);

		// Event does not exist
		if (!e)
			e = AddEvent<Arguments...>(hash, events, mutex);

		e->Register(d);
	}

	template< typename... Arguments >
	void EventManager::UnregisterEventImpl(const String & name, EventMap & events, Mutex & mutex, const Delegate< void(Arguments...) > & d)
	{
		Hash hash(name);
		Event< void(Arguments...) > * e = GetEvent<Arguments...>(hash, events, mutex);

		if (!e)
		{
			LOGERROR("Event " << name << " does not exist!");
			return;
		}

		e->Unregister(d);
	}

	template< typename... Arguments, typename Function >
	void EventManager::RaiseEventImpl(const String & name, EventMap & events, Mutex & mutex, Function & f)
	{
		Hash hash(name);
		Event< void(Arguments...) > * e = GetEvent<Arguments...>(hash, events, mutex);

		if (!e)
		{
			LOGERROR("Event " << name << " does not exist!");
			return;
		}

		f(e);
	}

	inline void EventManager::DestroyEventImpl(const String & name, EventMap & events, Mutex & mutex)
	{
		MUTEX_SCOPEDLOCK(mutex);

		Hash hash(name);
		EventMap::iterator it = events.find(hash);

		if (it == events.end())
		{
			LOGERROR("Event " << name << " does not exist!");
			return;
		}

		delete it->second;
		events.erase(it);
	}

	template< typename... Arguments >
	inline Event< void(Arguments...) > * EventManager::GetEvent(const Hash & hash, const EventMap & events, Mutex & mutex)
	{
		MUTEX_SCOPEDLOCK(mutex);
		EventMap::const_iterator it = events.find(hash);

		return (it == events.end()) ? nullptr : reinterpret_cast< Event< void(Arguments...) > * >(it->second);
	}

	template< typename... Arguments >
	Event< void(Arguments...) > * EventManager::AddEvent(const Hash & hash, EventMap & events, Mutex & mutex)
	{
		Event< void(Arguments...) > * new_event = new Event< void(Arguments...) >();
		MUTEX_SCOPEDLOCK(mutex);
		events.insert(std::make_pair(hash, new_event));
		return new_event;
	}

	template< typename... Arguments >
	inline void EventManager::RegisterEvent(const String & name, const Delegate< void(Arguments...) > & d)
	{
		RegisterEventImpl(name, mEvents, mEventsMutex, d);
	}

	template< typename... Arguments >
	inline void EventManager::UnregisterEvent(const String & name, const Delegate< void(Arguments...) > & d)
	{
		UnregisterEventImpl(name, mEvents, mEventsMutex, d);
	}

	template< typename... Arguments >
	inline void EventManager::RaiseEvent(const String & name, Arguments... args)
	{
		RaiseEventImpl<Arguments...>( name, mEvents, mEventsMutex, 
									  std::bind( &Event< void(Arguments...) >::Raise, std::placeholders::_1, args... ) );
	}

	inline void EventManager::DestroyEvent(const String & name)
	{
		DestroyEventImpl(name, mEvents, mEventsMutex);
	}

	template< typename... Arguments >
	inline void EventManager::RegisterObjectEvent(const String & name, const Delegate< void(Arguments...) > & d)
	{
		RegisterEventImpl(name, mObjectEvents, mObjectMutex, d);
	}

	template< typename... Arguments >
	inline void EventManager::UnregisterObjectEvent(const String & name, const Delegate< void(Arguments...) > & d)
	{
		UnregisterEventImpl(name, mObjectEvents, mObjectMutex, d);
	}

	template< typename... Arguments >
	inline void EventManager::RaiseObjectEvent(const String & name, void * obj, Arguments... args)
	{
		RaiseEventImpl<Arguments...>( name, mObjectEvents, mObjectMutex, 
									  std::bind( &Event< void(Arguments...) >::ObjectRaise, std::placeholders::_1, obj, args... ) );
	}

	inline void EventManager::DestroyObjectEvent(const String & name)
	{
		DestroyEventImpl(name, mObjectEvents, mObjectMutex);
	}
}