namespace BH
{
	// -------------------------------------------------------------------------------------------
	template < typename T >
	struct AddUpdatableImpl< T, true >
	{
		static inline void AddUpdatable( System * system, T * t )
		{
			system->AddUpdatable( t );
		}
	};

	template < typename T >
	struct AddUpdatableImpl< T, false >
	{
		static inline void AddUpdatable( System *, T * ) {}
	};

	template < typename T >
	struct RemoveUpdatableImpl< T, true >
	{
		static inline void RemoveUpdatable( System * system, T * t )
		{
			system->RemoveUpdatable( t );
		}
	};

	template < typename T >
	struct RemoveUpdatableImpl< T, false >
	{
		static inline void RemoveUpdatable( System *, T * ) {}
	};

	template < typename T >
	struct AddDrawableImpl< T, true >
	{
		static inline void AddDrawable( System * system, T * t )
		{
			system->AddDrawable( t );
		}
	};

	template < typename T >
	struct AddDrawableImpl< T, false >
	{
		static inline void AddDrawable( System *, T * ) {}
	};

	template < typename T >
	struct RemoveDrawableImpl< T, true >
	{
		static inline void RemoveDrawable( System * system, T * t )
		{
			system->RemoveDrawable( t );
		}
	};

	template < typename T >
	struct RemoveDrawableImpl< T, false >
	{
		static inline void RemoveDrawable( System *, T * ) {}
	};

	template < typename T >
	struct SortUpdatableImpl< T, true >
	{
		static inline void SortUpdatable( System * system )
		{
			system->SortUpdateList();
		}
	};

	template < typename T >
	struct SortUpdatableImpl< T, false >
	{
		static inline void SortUpdatable( System * ) {}
	};

	template < typename T >
	struct SortDrawableImpl< T, true >
	{
		static inline void SortDrawable( System * system )
		{
			system->SortDrawableList();
		}
	};

	template < typename T >
	struct SortDrawableImpl< T, false >
	{
		static inline void SortDrawable( System * ) {}
	};

	template < typename T >
	struct FastInsertUpdatableImpl< T, true >
	{
		static inline void FastInsertUpdatable( System * system, IUpdatable * t )
		{
			system->FastInsertUpdateInsertList( t );
		}
	};

	template < typename T >
	struct FastInsertUpdatableImpl< T, false >
	{
		static inline void FastInsertUpdatable( System *, IUpdatable * ) {}
	};

	template < typename T >
	struct FastInsertDrawableImpl< T, true >
	{
		static inline void FastInsertDrawable( System * system, IDrawable * t )
		{
			system->FastInsertDrawableInsertList( t );
		}
	};

	template < typename T >
	struct FastInsertDrawableImpl< T, false >
	{
		static inline void FastInsertDrawable( System *, IDrawable * ) {}
	};

	// -------------------------------------------------------------------------------------------
	inline bool System::IsIdling() const
	{
		return !( mJobQueue.JobAvailable() );
	}

	template< typename T >
	T * System::AddGameComponent( const Char * name )
	{
		// Compile time check for hierachy
		BH_STATIC_ASSERT( ( std::is_base_of<GameComponent, T>::value == true ), "System::AddGameComponent - T is not a derived class of GameComponent!" );
		//static_assert( std::is_base_of<GameComponent, T>::value == true, "System::AddGameComponent - T is not a derived class of GameComponent!" );

		CName cname( name );

		// If name exist
		if ( mGameComponentMap.find( cname ) != mGameComponentMap.end() )
		{
			LOGERROR( name << " existed in system!" );
			return nullptr;
		}

		T * t = new T();
		GameComponent * component = static_cast<T *>( t );
		mGameComponentMap.insert( std::make_pair( cname, 
												  component ) );

		if ( mGameComponentProcessMap.find( cname ) == mGameComponentProcessMap.end() )
			mGameComponentProcessMap.insert( std::make_pair( cname, 0 ) );

		component->mSystem = this;

		// If updatable, add
		//AddUpdatable( t );
		AddUpdatableImpl< T, std::is_base_of<IUpdatable, T>::value >::AddUpdatable( this, t );

		// If drawable, add
		//AddDrawable( t );
		AddDrawableImpl< T, std::is_base_of<IDrawable, T>::value >::AddDrawable( this, t );

		return t;
	}

	template< typename T >
	T * System::GetGameComponent(const Char * name)
	{
		// Compile time check for hierachy
		BH_STATIC_ASSERT( ( std::is_base_of<GameComponent, T>::value == true ), "System::GetGameComponent - T is not a derived class of GameComponent!" );

		CName cname( name );
		GameComponentMap::iterator it = mGameComponentMap.find( cname );
		if ( it == mGameComponentMap.end() )
		{
			LOGERROR( name << " does not exist in system!" );
			return nullptr;
		}
		else
		{
			T * component = dynamic_cast<T *>( it->second );
			//BH_ASSERT( component, "System::GetGameComponent - component type incorrect" );
			return component;
		}
	}

	template< typename T >
	void System::AddObjectComponent( T * component )
	{
		// Compile time check for hierachy
		BH_STATIC_ASSERT( ( std::is_base_of<ObjectComponent, T>::value == true ), "System::AddObjectComponent - T is not a derived class of ObjectComponent!" );

		ObjectComponent * objcomponent = static_cast<ObjectComponent *>( component );
		ObjectMap::mapped_type & complist = mObjectMap[ objcomponent->mObjectID ];

		//BH_ASSERT( complist.find( objcomponent->GetType().Name ) == mObjectMap[objcomponent->mObjectID].end(), 
		//		   "System::AddObjectComponent - repeated [" + String( objcomponent->GetType().Name.Text ) + "] added onto same object" );

		ObjectMap::mapped_type::iterator it = complist.find( objcomponent->GetType().Name );

		// Add object component to system
		if ( it != mObjectMap[objcomponent->mObjectID].end() )
			complist.insert( std::make_pair( objcomponent->GetType().Name, ObjectMap::mapped_type::mapped_type() ) );

		complist[objcomponent->GetType().Name].push_back( objcomponent );

		objcomponent->mSystem = this;

		// If updatable, add
		//AddUpdatable( component );
		AddUpdatableImpl< T, std::is_base_of<IUpdatable, T>::value >::AddUpdatable( this, component );

		// If drawable, add
		//AddDrawable( component );
		AddDrawableImpl< T, std::is_base_of<IDrawable, T>::value >::AddDrawable( this, component );
	}

	template< typename T >
	inline bool System::ObjectComponentExist( ObjectID id )
	{
		ObjectMap::iterator it = mObjectMap.find( id );
		// return object and component exist
		return ( it != mObjectMap.end() && 
				 it->second.find( ENTITY_MANAGER.Types.Get<T>()->Name ) != it->second.end() );
	}

	template< typename T >
	void System::RemoveObjectComponent( T * component )
	{
		// Compile time check for hierachy
		BH_STATIC_ASSERT( ( std::is_base_of<ObjectComponent, T>::value == true ), "System::RemoveObjectComponent - T is not a derived class of ObjectComponent!" );

		ObjectComponent * objcomponent = static_cast<ObjectComponent *>( component );
		ObjectMap::iterator it = mObjectMap.find( objcomponent->mObjectID );

		// If object not found
		if ( it == mObjectMap.end() )
		{
			LOGERROR( "Object ID: [" << objcomponent->mObjectID << "] does not exist in system!" );
			return;
		}
		// If component not found, ignore
		else if ( it->second.find( objcomponent->GetType().Name ) == mObjectMap[objcomponent->mObjectID].end() )
		{
			LOGERROR( "[" << objcomponent->GetType().Name.Text << "] does not exist in the object or system!" );
			return;
		}

		// Remove component
		ObjectMap::mapped_type::iterator typeMap = it->second.find( objcomponent->GetType().Name );
		ObjectMap::mapped_type::mapped_type::iterator cit = std::find( typeMap->second.begin(), typeMap->second.end(), objcomponent );
		typeMap->second.erase( cit );

		// Remove component list if none exist
		if ( typeMap->second.empty() )
			it->second.erase( objcomponent->GetType().Name );

		objcomponent->mSystem = nullptr;

		// Remove object, if no more component left
		if( it->second.empty() )
			mObjectMap.erase( it );

		// if updatable, remove
		//RemoveUpdatable( component );
		RemoveUpdatableImpl< T, std::is_base_of<IUpdatable, T>::value >::RemoveUpdatable( this, component );

		// if drawable, remove
		//RemoveDrawable( component );
		RemoveDrawableImpl< T, std::is_base_of<IDrawable, T>::value >::RemoveDrawable( this, component );
	}

	template< typename T >
	void System::UpdatableAcquire( T * component )
	{
		IUpdatable * updatable = dynamic_cast<IUpdatable *>( component );
		while ( updatable->GetFlip() != mLastFlag )
			Thread::ThreadYield();
	}

	template< typename T >
	void System::AddUpdatable( T * t )
	{
		//std::cout << ENTITY_MANAGER.Types.Get<T>()->Name.Text << " - base of IUpdatable: " << std::is_base_of<IUpdatable, T>::value << std::endl;
		//if ( std::is_base_of<IUpdatable, T>::value == true )
		{
			IUpdatable * updatable = dynamic_cast<IUpdatable *>( t );
			mUpdatableList.push_back( updatable );

			// If game component is added (before initialise)
			//if ( std::is_base_of<GameComponent, T>::value == true )
			//{
			//	SortUpdateList();
			//}
			SortUpdatableImpl< T, std::is_base_of<GameComponent, T>::value >::SortUpdatable( this );

			// If object component, have not been added to run list yet
			//if ( std::is_base_of<ObjectComponent, T>::value == true && updatable->GetEnabled() )
			//{
			//	mUpdatableInsertList.push_back( updatable );
			//}
			FastInsertUpdatableImpl< T, std::is_base_of<ObjectComponent, T>::value >::FastInsertUpdatable( this, updatable );
		}
	}

	template< typename T >
	void System::RemoveUpdatable( T * t )
	{
		//if ( std::is_base_of<IUpdatable, T>::value == true )
		{
			IUpdatable * updatable = dynamic_cast<IUpdatable *>( t );
			UpdateList::iterator it = std::find( mUpdatableList.begin(), mUpdatableList.end(), updatable );

			if ( it != mUpdatableList.end() )
			{
				mUpdatableList.erase( it );

				// If enabled, possibly it can be found in add list
				if( updatable->GetEnabled() )
				{
					it = std::find( mUpdatableInsertList.begin(), mUpdatableInsertList.end(), updatable );
					if ( it != mUpdatableInsertList.end() )
						mUpdatableInsertList.erase( it );
				}
				// If disable, possibly it can be found in remove list
				else
				{
					it = std::find( mUpdatableRemoveList.begin(), mUpdatableRemoveList.end(), updatable );
					if ( it != mUpdatableRemoveList.end() )
						mUpdatableRemoveList.erase( it );
				}

				UpdateList::iterator rit = std::find( mUpdatableRunList.begin(), mUpdatableRunList.end(), updatable );
				if ( rit != mUpdatableRunList.end() )
				{
					mUpdatableRunList.erase( rit );
				}

				UpdateList::iterator pit = std::find( mPausingList.begin(), mPausingList.end(), updatable );
				if ( pit != mPausingList.end() )
				{
					mPausingList.erase( pit );
				}
			}
		}
	}

	template< typename T >
	void System::AddDrawable( T * t )
	{
		//if ( std::is_base_of<IDrawable, T>::value == true )
		{
			IDrawable * drawable = dynamic_cast<IDrawable *>( t );
			mDrawableList.push_back( drawable );

			// If game component is added (before initialise)
			//if ( std::is_base_of<GameComponent, T>::value == true )
			//{
			//	SortDrawableList();
			//}
			SortDrawableImpl< T, std::is_base_of<GameComponent, T>::value >::SortDrawable( this );

			// If object component, have not been added to run list yet
			//if ( std::is_base_of<ObjectComponent, T>::value == true && drawable->GetVisible() )
			//{
			//	mDrawableInsertList.push_back( drawable );
			//}
			FastInsertDrawableImpl< T, std::is_base_of<ObjectComponent, T>::value >::FastInsertDrawable( this, drawable );
		}
	}

	template< typename T >
	void System::RemoveDrawable( T * t )
	{
		//if ( std::is_base_of<IDrawable, T>::value == true )
		{
			IDrawable * drawable = dynamic_cast<IDrawable *>( t );
			DrawList::iterator it = std::find( mDrawableList.begin(), mDrawableList.end(), drawable );

			if ( it != mDrawableList.end() )
			{
				mDrawableList.erase( it );

				// If enabled, possibly it can be found in add list
				if( drawable->GetVisible() )
				{
					it = std::find( mDrawableInsertList.begin(), mDrawableInsertList.end(), drawable );
					if ( it != mDrawableInsertList.end() )
						mDrawableInsertList.erase( it );
				}
				// If disable, possibly it can be found in remove list
				else
				{
					it = std::find( mDrawableRemoveList.begin(), mDrawableRemoveList.end(), drawable );
					if ( it != mDrawableRemoveList.end() )
						mDrawableRemoveList.erase( it );
				}

				DrawList::iterator rit = std::find( mDrawableRunList.begin(), mDrawableRunList.end(), drawable );
				if ( rit != mDrawableRunList.end() )
				{
					mDrawableRunList.erase( rit );
				}
			}
		}
	}

	void System::SortUpdateList()
	{
		std::sort( mUpdatableList.begin(), mUpdatableList.end(), 
				 	[]( const IUpdatable * lhs, const IUpdatable * rhs )
				 	{
				 		return *lhs < *rhs;
				 	} 
				 );
	}

	void System::SortDrawableList()
	{
		std::sort( mDrawableList.begin(), mDrawableList.end(),
				   	[]( const IDrawable * lhs, const IDrawable * rhs )
				   	{
				   		return *lhs < *rhs;
				   	}
				 );
	}

	void System::FastInsertUpdateInsertList( IUpdatable * updatable )
	{
		if ( updatable->GetEnabled() )
		{
			mUpdatableInsertList.push_back( updatable );
		}
	}
	
	void System::FastInsertDrawableInsertList( IDrawable * drawable )
	{
		if ( drawable->GetVisible() )
		{
			mDrawableInsertList.push_back( drawable );
		}
	}

	template< typename ReturnType, typename... Arguments >
	void System::AddJob( u32 priority, const Delegate< ReturnType( Arguments... ) > & delegate, Arguments... args )
	{
		Message msg;
		// Add delegate as last params
		detail::PackParams( msg, args..., delegate );
		//msg.AddMsg< Delegate< ReturnType( Arguments... ) > >( delegate );

		// Add to job queue
		mJobQueue.AddJob( priority, JobQueue::Job::FromFunction< &detail::WorkImpl< Arguments..., Delegate< ReturnType(Arguments...) > > >(), msg );
	}

	template< typename... Arguments, typename ComponentType, typename ReturnType >
	ReturnType System::RequestJob( ObjectID id, ReturnType( ComponentType::*MemFun )( Arguments... ) const, Arguments... args )
	{
		// Compile time check for hierachy
		BH_STATIC_ASSERT( ( std::is_base_of<ObjectComponent, ComponentType>::value == true ), "System::RequestData - T is not a derived class of ObjectComponent!" );

		ObjectMap::iterator oit = mObjectMap.find( id );
		BH_ASSERT( oit != mObjectMap.end(), "System::RequestData - Object does not exist in this system!" );

		// If component type did not inherit from updatable, no need to block
		ObjectComponentMap::iterator cit = oit->second.find( ENTITY_MANAGER.Types.Get<ComponentType>()->Name );
		BH_ASSERT( cit != oit->second.end(), "System::RequestData - Component does not exist in this object in this system!" );

		// Get the first component from list
		ComponentType * component = static_cast<ComponentType *>( cit->second.front() );

		// Not updatable, not need to synchro
		if ( std::is_base_of<IUpdatable, ComponentType>::value == false )
		{
			return ( component->*MemFun )( args... );
		}
		else
		{
			// Blocking function to ensure flag is the same
			UpdatableAcquire( component );
			return ( component->*MemFun )( args... );
		}
	}

	template< typename... Arguments, typename ComponentType, typename ReturnType >
	ReturnType System::RequestJob( ObjectID id, ReturnType( ComponentType::*MemFun )( Arguments... ), Arguments... args )
	{
		// Compile time check for hierachy
		BH_STATIC_ASSERT( ( std::is_base_of<ObjectComponent, ComponentType>::value == true ), "System::RequestData - T is not a derived class of ObjectComponent!" );

		ObjectMap::iterator oit = mObjectMap.find( id );
		BH_ASSERT( oit != mObjectMap.end(), "System::RequestData - Object does not exist in this system!" );

		// If component type did not inherit from updatable, no need to block
		ObjectComponentMap::iterator cit = oit->second.find( ENTITY_MANAGER.Types.Get<ComponentType>()->Name );
		BH_ASSERT( cit != oit->second.end(), "System::RequestData - Component does not exist in this object in this system!" );

		// Get the first component from list
		ComponentType * component = static_cast<ComponentType *>( cit->second.front() );

		// Not updatable, not need to synchro
		if ( std::is_base_of<IUpdatable, ComponentType>::value == false )
		{
			return ( component->*MemFun )( args... );
		}
		else
		{
			// Blocking function to ensure flag is the same
			UpdatableAcquire( component );
			return ( component->*MemFun )( args... );
		}
	}
}