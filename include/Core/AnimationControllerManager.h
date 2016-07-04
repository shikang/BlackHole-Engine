#ifndef BH_ANIMATION_CONTROLLER_MANAGER_H
#define BH_ANIMATION_CONTROLLER_MANAGER_H

#include "Core/CoreEssential.h"
#include "Core/FunctionsFramework.h"
#include "Core/GameComponent.h"
#include "Core/SystemManager.h"

namespace BH
{
	class AnimationControllerManager : public GameComponent
	{
	public:
		typedef std::set< AnimationController * > AnimationControllerList;

		GETTER_AUTO_CREF( AnimationControllerList, ControllerList );

	public:
		// Constructor
		AnimationControllerManager();

		// Destructor
		~AnimationControllerManager();

		// Initialise
		void Initialise() override;

		// Shutdown
		void Shutdown() override;

	public:
		// Create Animation Controller
		AnimationController * CreateController();

		// Delete Animation Controller
		void DeleteController( AnimationController * controller );

	private:
		AnimationControllerList mControllerList;

	};
}

#include "Core/Typing.h"
DECLARE_TYPE(::BH::AnimationControllerManager, AnimationControllerManager);

#endif