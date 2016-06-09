namespace BH
{
	template< typename T >
	T * SystemManager::AddSystem( const Char * name )
	{
		// Compile time check for heiarchy
		BH_STATIC_ASSERT( ( std::is_base_of<System, T>::value == true ), "SystemManager::AddSystem - T is not a derived class of System!" );

		CName cname( name );

		// You do not use main as a name to add system (Main is reserved for main system)
		BH_ASSERT( cname.Hash != Hash( "Main" ).GetHash(), "SystemManager::AddSystem - 'Main' name is used!" );

		SystemMap::iterator it = mSystems.find( cname );
		// If name exist
		if ( mSystems.find( cname ) != mSystems.end() )
		{
			LOGERROR( name << " system has already been added!" );
			return dynamic_cast<T *>( it->second.first );
		}

		T * newSystem = new T( name );
		mSystems.insert( std::make_pair( name, std::make_pair( newSystem, new Thread() ) ) );
		return newSystem;
	}

	template< typename T >
	inline T * SystemManager::GetSystem( const Char * name )
	{
		// Compile time check for heiarchy
		BH_STATIC_ASSERT( ( std::is_base_of<System, T>::value == true ), "SystemManager::AddSystem - T is not a derived class of System!" );

		CName cname( name );
		// If get main system
		if ( cname.Hash == Hash( "Main" ).GetHash() )
			return dynamic_cast<T *>( mMainSystem );

		SystemMap::iterator it = mSystems.find( cname );
		return ( mSystems.find( cname ) != mSystems.end() ) ? dynamic_cast<T *>( it->second.first ) : AddSystem<T>( name );
	}

	template< typename GameComponentType >
	GameComponentType * SystemManager::AddGameComponentToSystem()
	{
		//LOGINFO( "SystemManager::AddGameComponentToSystem - have no assigned system!" );
		return mMainSystem->AddGameComponent<GameComponentType>( TypeName<GameComponentType>() );
	}

	template< typename GameComponentType >
	GameComponentType * SystemManager::GetGameComponentFromSystem()
	{
		return mMainSystem->GetGameComponent<GameComponentType>( TypeName<GameComponentType>() );
	}

	template< typename ObjectComponentType >
	void SystemManager::AddObjectComponentToSystem( ObjectComponent * objectcomponent )
	{
		//LOGINFO( "SystemManager::AddObjectComponentToSystem - have no assigned system!" );
		mMainSystem->AddObjectComponent( static_cast<ObjectComponentType *>( objectcomponent ) );
	}

	template< typename ObjectComponentType >
	System * SystemManager::GetAssignedSystem()
	{
		//LOGINFO( "SystemManager::GetAssignedSystem - have no assigned system!" );
		//return nullptr;
		return mMainSystem;
	}

	template< typename ObjectComponentType >
	void SystemManager::RemoveObjectComponentFromSystem( ObjectComponent * objectcomponent )
	{
		mMainSystem->RemoveObjectComponent( static_cast<ObjectComponentType *>( objectcomponent ) );
	}

	template< typename DependeeType, typename DependentType >
	void SystemManager::GameComponentDependency()
	{
		// Calling 
		DependencyFunctor<DependeeType, DependentType>()();
	}

	template< typename DependeeType, typename DependentType >
	void DependencyFunctor<DependeeType, DependentType>::operator()() const
	{
		//LOGINFO( "SystemManager::GameComponentDependency - have no assigned system!" );
		if( System * system = ::BH::Game::Instance().SystemManager.GetSystem<System>( "Main" ) )
			system->Dependency( TypeName<DependeeType>(), TypeName<DependentType>() );
	}
}