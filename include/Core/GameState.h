#ifndef BH_GAMESTATE_H
#define BH_GAMESTATE_H

#include "Core/CoreEssential.h"
#include "Core/Singleton.h"
#include "Core/Uncopyable.h"
#include "Core/Entity.h"

namespace BH
{
	// Abstract game state class.
	class GameState : public Entity,
					  private Uncopyable
	{
	public:
		GETTER_AUTO( bool, Active );
		GETTER_AUTO( bool, Paused );

	public:
		// Constructor.
		GameState();

		// Destructor.
		virtual ~GameState();

		// Called when state is added/pushed.
		virtual void Activate() {}

		// Called when state is removed/popped.
		virtual void Deactivate() {}

		// Called when a state is popped from the stack and this state is the new state on top.
		virtual void Resume() {}

		// Called when a state is pushed on to the stack and was the previous state on top.
		virtual void Pause() {}

		// Called just before the internal representation of the entity changes. (Before deserialisation)
		virtual Entity::OPTIONS OnPreChanged();

		// Called when the internal representation of the entity changes. (After deserialisation)
		virtual void OnChanged();

		// Called when the entity is about to be saved. (Before serialisation)
		virtual Entity::OPTIONS OnPreSaved() const;

		// Called after the entity is saved. (After serialisation)
		virtual void OnSaved() const;
		
	private:
		bool			mActive;
		bool			mPaused;
		friend class	GameStateManager;
		friend class	TypeDB;

	};
}

#define BH_GAMESTATE_FRIENDS\
	friend class	GameStateManager;\
	friend class	TypeDB

#include "Core/Typing.h"
DECLARE_TYPE( ::BH::GameState, GameState );

#endif