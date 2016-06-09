namespace BH
{
	template< typename GAMESTATE_TYPE >
	inline GAMESTATE_TYPE * GameStateManager::Push()
	{
		// Force a compile-time type test.
		ENTITY_TYPETEST( GameState, GAMESTATE_TYPE );
		return static_cast<GAMESTATE_TYPE *>( Push( TypeCName<GAMESTATE_TYPE>() ) );
	}

	template< typename GAMESTATE_TYPE >
	void GameStateManager::Add( const Hash& name )
	{
		// Force a compile-time type test.
		ENTITY_TYPETEST( GameState, GAMESTATE_TYPE );
		Add( TypeCName<GAMESTATE_TYPE>(), name );
	}

	template< typename STATE_TYPE >
	inline STATE_TYPE* GameStateManager::Get( const Hash& name ) const
	{
		return static_cast< STATE_TYPE* >( Get( name ) );
	}

	inline GameState* GameStateManager::Get( const Hash& name ) const
	{
		StateMap::const_iterator findit = mStates.find( name );
		return findit != mStates.end() ? findit->second : nullptr;
	}
}