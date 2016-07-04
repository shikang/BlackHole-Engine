#ifndef BH_ANIMATION_CONTROLLER_H
#define BH_ANIMATION_CONTROLLER_H

#include "Platform/Typedefs.h"
#include "Platform/Animation.h"
#include "Platform/Matrix4.h"

#include "Core/FunctionsFramework.h"

#include <unordered_map>

namespace BH
{
	class AnimationController
	{
	public:
		typedef std::unordered_map<String, const Animation *> AnimationList;

	public:
		// Constructor
		AnimationController();

		// Destructor
		~AnimationController();

		// Update Animation
		void UpdateAnimation( f32 dt );

	public:
		inline void SetAnimationSpeed( f32 speed ) { mAnimationSpeed = speed; }

		// Add animation
		void AddAnimation( const Animation * animation );

		// Set current animation
		void SetAnimation( const String & animationName );

		// Set skeleton
		void SetSkeleton( const Skeleton * skeleton );

		// Clear Track data
		void ClearTrackData();

		// Get Bone Matrix Buffer
		GETTER_AUTO_CREF( MatrixBuffer, BoneMatrixBuffer );

	private:
		f32 mElapsedTime;
		f32 mAnimationSpeed;

		AnimationList mAnimations;
		const Animation * mCurrentAnimation;

		const Skeleton * mSkeleton;

		MatrixBuffer mBoneMatrixBuffer;
		TrackBuffer mAnimationTrackBuffer;
	};
}

#endif