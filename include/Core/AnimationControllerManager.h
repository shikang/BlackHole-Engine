#ifndef BH_ANIMATION_CONTROLLER_MANAGER_H
#define BH_ANIMATION_CONTROLLER_MANAGER_H

#include "Core/CoreEssential.h"
#include "Core/FunctionsFramework.h"
#include "Core/GameComponent.h"
#include "Core/SystemManager.h"
#include "Core/IUpdatable.h"

namespace BH
{
	class AnimationControllerManager : public GameComponent, public IUpdatable
	{
	public:
		typedef u32 AnimationControllerID;
		typedef std::unordered_map<AnimationControllerID, AnimationController *> AnimationControllerList;
		typedef std::vector<AnimationControllerID> UnusedIDList;

		static const AnimationControllerID INVALID_ID;

		GETTER_AUTO_CREF( AnimationControllerList, ControllerList );

	private:
		static const AnimationControllerID STARTING_ID;

	public:
		// Constructor
		AnimationControllerManager();

		// Destructor
		~AnimationControllerManager();

		// Initialise
		void Initialise() override;

		// Update
		void Update( f32 dt ) override;

		// Shutdown
		void Shutdown() override;

	public:
		// Create Animation Controller for model, return id
		AnimationControllerID CreateController( Model * model );

		// Delete Animation Controller by id
		void DeleteController( AnimationControllerID id );

		// Set animation
		void SetAnimation( AnimationControllerID id, const String & animation );

		// Set animation speed
		void SetAnimationSpeed( AnimationControllerID id, f32 speed );

	private:
		AnimationControllerID GetID();

	private:
		AnimationControllerList mControllerList;
		AnimationControllerID mNewID;
		UnusedIDList mUnusedIDList;

	};
}

#include "Core/Typing.h"
DECLARE_TYPE(::BH::AnimationControllerManager, AnimationControllerManager);

#endif