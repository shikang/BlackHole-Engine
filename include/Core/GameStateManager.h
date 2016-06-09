#ifndef BH_GAMESTATE_MANAGER_H
#define BH_GAMESTATE_MANAGER_H

#include "Core/CoreEssential.h"
#include "Core/GameState.h"
#include "Core/Uncopyable.h"

namespace BH
{
	// Game state manager class.
	class GameStateManager : public GameComponent, public IUpdatable
	{
	public:
		// Constructor.
		GameStateManager();

		// Destructor.
		~GameStateManager();

		// Push state to stack.
		template< typename STATE_TYPE >
		STATE_TYPE * Push();

		// Push state to stack
		GameState * Push( const CName & type );

		// Pop state from stack.
		void Pop();

		// Add persistent state.
		template< typename STATE_TYPE >
		void Add( const Hash & name );

		// Add persistent state.
		void Add( const CName & type, const Hash & name );

		// Remove persistent state.
		void Remove( const Hash & name );

		// Get persistent state by name.
		template< typename STATE_TYPE >
		STATE_TYPE * Get( const Hash & name ) const;

		// Get persistent state by name.
		GameState * Get( const Hash & name ) const;

		// Initialise
		void Initialise() override;

		// Shutdown
		void Shutdown() override;

		// Initialise and purge states.
		void Update( f32 dt ) override;

		// Clear all stack and persistent states.
		void Clear();
		
	private:
		static void ActivateState( GameState * state );
		static void DeactivateState( GameState * state );
		static void ResumeState( GameState * state );
		static void PauseState( GameState * state );

		typedef void ( * StateFunction )( GameState * );
		typedef std::pair< GameState *, StateFunction >		StateAction;

		typedef std::unordered_map< Hash, GameState * >		StateMap;
		typedef std::vector< GameState * >					StateArray;
		typedef std::vector < StateAction >					StateActions;
		
	private:
		StateActions		mStateActions;
		StateArray			mStateStack;	//!< Current state stack.
		StateArray			mToDelete;		//!< States to delete.
		StateMap			mStates;		//!< Existing persistent states.

		friend class		Game;

	};
}

#include "Core/GameStateManager.inl"

#include "Core/Typing.h"
DECLARE_TYPE(::BH::GameStateManager, GameStateManager);

#endif