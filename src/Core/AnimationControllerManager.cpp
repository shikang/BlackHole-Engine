// Precompiled Header
#include "Core/Stdafx.h"

#include "Platform/FileSystem.h"
#include "Platform/Texture.h"
#include "Platform/AnimationController.h"

#include "Core/AnimationControllerManager.h"

#include <utility>

namespace BH
{
	AnimationControllerManager::AnimationControllerManager()
	{
	}

	AnimationControllerManager::~AnimationControllerManager()
	{
	}

	void AnimationControllerManager::Initialise()
	{
		LOG("Initialising Animation Controller Manager...");
		
		LOG("Animation Controller Manager Initialised");
	}

	void AnimationControllerManager::Shutdown()
	{
		for( auto & i : mControllerList )
		{
			delete i;
		}

		mControllerList.clear();
	}

	AnimationController * AnimationControllerManager::CreateController()
	{
		AnimationController * ac = new AnimationController;
		mControllerList.insert( ac );

		return ac;
	}

	void AnimationControllerManager::DeleteController( AnimationController * controller )
	{
		AnimationControllerList::iterator it = mControllerList.find( controller );
		if ( it != mControllerList.end() )
		{
			mControllerList.erase( it );
			delete controller;
		}
	}
}