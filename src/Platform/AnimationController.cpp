// Precompiled Header
#include "Platform/Stdafx.h"

#include "Platform/AnimationController.h"
#include "Platform/Skeleton.h"

namespace BH
{
	AnimationController::AnimationController()
	: mElapsedTime( 0.0f )
	, mAnimationSpeed( 1.0f )
	, mCurrentAnimation( nullptr )
	{

	}

	AnimationController::~AnimationController()
	{

	}

	void AnimationController::UpdateAnimation( f32 dt )
	{
		if ( !mCurrentAnimation || !mSkeleton )
			return;

		mElapsedTime += mAnimationSpeed * dt;
		if ( mElapsedTime > mCurrentAnimation->mDuration )
		{
			//mElapsedTime -= mCurrentAnimation->mDuration;
			mElapsedTime = 0;
			ClearTrackData();
		}

		mSkeleton->ProcessAnimationGraph( mElapsedTime, mBoneMatrixBuffer, *mCurrentAnimation, mAnimationTrackBuffer );
	}

	void AnimationController::AddAnimation( const Animation * animation )
	{
		if ( animation )
		{
			// Set default animation
			if ( !mCurrentAnimation )
			{
				mCurrentAnimation = animation;
			}

			mAnimations.insert( std::make_pair( animation->mName, animation ) );
		}
	}

	void AnimationController::SetAnimation( const String & animationName )
	{
		AnimationList::iterator it = mAnimations.find( animationName );
		if ( it != mAnimations.end() )
		{
			mCurrentAnimation = it->second;
		}
	}

	void AnimationController::SetSkeleton( const Skeleton * skeleton )
	{
		if ( skeleton )
		{
			mSkeleton = skeleton;
			mBoneMatrixBuffer.resize( skeleton->mBones.size() );
			mAnimationTrackBuffer.resize( skeleton->mBones.size() );
		}
		
	}

	void AnimationController::ClearTrackData()
	{
		for ( TrackData & t : mAnimationTrackBuffer )
			t.mLastKey = 0;
	}
}