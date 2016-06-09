#ifndef BH_LEVELSTATE_H
#define BH_LEVELSTATE_H

#include "Core/CoreEssential.h"
#include "Core/GameState.h"
#include "Core/FunctionsFramework.h"

namespace BH
{
	// Abstract game state class.
	class LevelState : public GameState
	{
	public:
		// Set level
		GETSET_AUTO_CREF( String, Level );

	public:
		// Constructor.
		LevelState();

		// Destructor.
		virtual ~LevelState();

		// Called when state is added/pushed.
		virtual void Activate();

		// Called when state is removed/popped.
		virtual void Deactivate();

		// Called when a state is popped from the stack and this state is the new state on top.
		virtual void Resume();

		// Called when a state is pushed on to the stack and was the previous state on top.
		virtual void Pause();

		// Called just before the internal representation of the entity changes. (Before deserialisation)
		virtual Entity::OPTIONS OnPreChanged();

		// Called when the internal representation of the entity changes. (After deserialisation)
		virtual void OnChanged();

		// Called when the entity is about to be saved. (Before serialisation)
		virtual Entity::OPTIONS OnPreSaved() const;

		// Called after the entity is saved. (After serialisation)
		virtual void OnSaved() const;

	private:
		String mLevel;

		BH_GAMESTATE_FRIENDS;

	};
}

#include "Core/Typing.h"
DECLARE_TYPE( ::BH::LevelState, LevelState );

#endif