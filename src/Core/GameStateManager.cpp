// Precompiled Header
#include "Core/Stdafx.h"

#include "Core/GameStateManager.h"
#include "Core/LogManager.h"

namespace BH
{
	GameStateManager::GameStateManager()
	: IUpdatable( UINT_MAX )
	{
	}

	GameStateManager::~GameStateManager()
	{
	}

	GameState * GameStateManager::Push( const CName & type )
	{
		if( EntityPtr newEnt = reinterpret_cast< EntityPtr >( ENTITY_MANAGER.New( type ) ) ) 
		{
			GameState* state = static_cast< GameState * >( newEnt );

			// Pause the current state on the top of the stack.
			if( !mStateStack.empty() )
				mStateActions.push_back( StateAction( mStateStack.back(), PauseState ) );

			// Push it to the stack.
			mStateStack.push_back( state );

			mStateActions.push_back( StateAction( state, ActivateState ) );
			mStateActions.push_back( StateAction( state, ResumeState ) );
			
			return state;
		}

		return nullptr;
	}

	void GameStateManager::Pop()
	{
		if( mStateStack.empty() ) 
		{
			LOGERROR( "State stack is empty. Unable to pop." );
			return;
		}

		GameState * state = mStateStack.back();
		mStateStack.pop_back();

		// Pause and deactivate the state we just popped.
		mStateActions.push_back( StateAction( state, PauseState ) );
		mStateActions.push_back( StateAction( state, DeactivateState ) );

		// Resume the state on the top of the stack.
		if( !mStateStack.empty() )
			mStateActions.push_back( StateAction( mStateStack.back(), ResumeState ) );

		// Set the popped state to be deleted.
		mToDelete.push_back( state );
	}

	void GameStateManager::Add( const CName& type, const Hash& name )
	{
		if( mStates.find( name ) != mStates.end() ) 
		{
			LOGERROR( "State (" << name << ") already exists." );
			return;
		}

		if( EntityPtr newEnt = reinterpret_cast< EntityPtr >( ENTITY_MANAGER.New( type ) ) ) 
		{
			GameState* state = static_cast<GameState*>( newEnt );
			// Add it to the map.
			mStates.insert( std::make_pair( name, state ) );
			mStateActions.push_back( StateAction( state, ActivateState ) );
			mStateActions.push_back( StateAction( state, ResumeState ) );
		}
	}

	void GameStateManager::Remove( const Hash& name )
	{
		StateMap::iterator findit = mStates.find( name );
		if( findit == mStates.end() ) 
		{
			LOGERROR( "State (" << name.GetText() << ") does not exist." );
			return;
		}

		GameState* state = findit->second;
		mStates.erase( findit );

		// Pause and deactivate the state we just removed.
		mStateActions.push_back( StateAction( state, PauseState ) );
		mStateActions.push_back( StateAction( state, DeactivateState ) );

		// Set the popped state to be deleted.
		mToDelete.push_back( state );
	}

	void GameStateManager::Clear( void )
	{
		// Clear all states on stack. Start from the back.
		for( size_t i = mStateStack.size(); i > 0; --i ) 
		{
			mStateActions.push_back( StateAction( mStateStack[i - 1], PauseState ) );
			mStateActions.push_back( StateAction( mStateStack[i - 1], DeactivateState ) );
			mToDelete.push_back( mStateStack[i - 1] );
		}

		for ( auto & i : mStates )
		{
			mStateActions.push_back( StateAction( i.second, PauseState ) );
			mStateActions.push_back( StateAction( i.second, DeactivateState ) );
			mToDelete.push_back( i.second );
		}

		mStateStack.clear();
		mStates.clear();
	}

	void GameStateManager::Initialise() {}

	void GameStateManager::Shutdown() 
	{ 
		Clear();
		Update( 0.0f ); 
	}

	void GameStateManager::Update( f32 )
	{
		for( size_t i = 0; i < mStateActions.size(); ++i )
			mStateActions[i].second( mStateActions[i].first );

		mStateActions.clear();

		for( size_t i = 0; i < mToDelete.size(); ++i )
			ENTITY_MANAGER.Delete( mToDelete[i] );

		mToDelete.clear();
	}

	void GameStateManager::ActivateState( GameState* state )
	{
		if( !state->mActive )
		{
			state->Activate();
			state->mActive = true;
		}
	}

	void GameStateManager::DeactivateState( GameState* state )
	{
		if( state->mActive )
		{
			state->mActive = false;
			state->Deactivate();
		}
	}

	void GameStateManager::ResumeState( GameState* state )
	{
		if( state->mActive && state->mPaused )
		{
			state->Resume();
			state->mPaused = false;
		}
	}

	void GameStateManager::PauseState( GameState* state )
	{
		if( state->mActive && !state->mPaused )
		{
			state->mPaused = true;
			state->Pause();
		}
	}
}