// Precompiled Header
#include "Core/Stdafx.h"

#include "Platform/FileSystem.h"
#include "Platform/Texture.h"
#include "Platform/AnimationController.h"

#include "Core/Model.h"
#include "Core/AnimationControllerManager.h"

#include <utility>

namespace BH
{
	const AnimationControllerManager::AnimationControllerID AnimationControllerManager::INVALID_ID = 0;
	const AnimationControllerManager::AnimationControllerID AnimationControllerManager::STARTING_ID = 1;

	AnimationControllerManager::AnimationControllerManager()
	: mNewID( 1 )
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
		for ( auto & i : mControllerList )
		{
			delete i.second;
		}

		mControllerList.clear();
		mUnusedIDList.clear();
		mNewID = 1;
	}

	void AnimationControllerManager::Update( f32 dt )
	{
		for ( auto & i : mControllerList )
		{
			i.second->UpdateAnimation( dt );
		}
	}

	AnimationControllerManager::AnimationControllerID AnimationControllerManager::CreateController( Model * model )
	{
		AnimationController * ac = new AnimationController;
		AnimationControllerID newID = GetID();

		mControllerList.insert( std::make_pair( newID, ac ) );

		return newID;
	}

	void AnimationControllerManager::DeleteController( AnimationControllerID id )
	{
		AnimationControllerList::iterator it = mControllerList.find( id );
		if ( it != mControllerList.end() )
		{
			delete it->second;
			mControllerList.erase( it );
			mUnusedIDList.push_back( id );
		}
	}

	void AnimationControllerManager::SetAnimation( AnimationControllerID id, const String & animation )
	{
		AnimationControllerList::iterator it = mControllerList.find( id );
		if ( it != mControllerList.end() )
		{
			it->second->SetAnimation( animation );
		}
	}

	void AnimationControllerManager::SetAnimationSpeed( AnimationControllerID id, f32 speed )
	{
		AnimationControllerList::iterator it = mControllerList.find( id );
		if ( it != mControllerList.end() )
		{
			it->second->SetAnimationSpeed( speed );
		}
	}

	AnimationControllerManager::AnimationControllerID AnimationControllerManager::GetID()
	{
		if ( mUnusedIDList.empty() )
		{
			return mNewID++;
		}
		else
		{
			AnimationControllerID id = mUnusedIDList.back();
			mUnusedIDList.pop_back();
			return id;
		}
	}
}