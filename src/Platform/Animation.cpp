// Precompiled Header
#include "Platform/Stdafx.h"

#include "Platform/Animation.h"

namespace BH
{
	void Animation::CalculateTransform( f32 animTime, u32 trackIndex, Vector3f & animT, Quaternion & animR, TrackData & data ) const
	{
		const f32 DEG_TO_RAD = ( 3.14159265359f / 180.0f );
		/*
			Since keys are not spaced at regular intervals we need to search
			for the keyframes that will be interpolated between.  The track data is
			used to store what the last keyframe was to prevent searching the entire
			track.
		*/
		s32 currentKey = data.mLastKey;
		const Track & currentPath = mTracks[trackIndex];

		// Search Forward in the keyframes for the interval
		while ( currentKey != currentPath.mKeyFrames.size() - 1 &&
				currentPath.mKeyFrames[currentKey + 1].mTime < animTime )
			++currentKey;

		// Search Backward in the keyframes for the interval
		while ( currentKey != 0 && currentPath.mKeyFrames[currentKey].mTime > animTime )
			--currentKey;

		if ( currentKey == currentPath.mKeyFrames.size() - 1 )
		{
			/*
				Past the last keyframe for this path so use the last frame and the transform data
				so the animation is clamped to the last frame
			*/
			animR = currentPath.mKeyFrames[currentKey].mRotation;
			Quaternion b = animR;
			animR = Quaternion::CreateFromYawPitchRoll( DEG_TO_RAD * currentPath.mKeyFrames[currentKey].mEuler.y,
														DEG_TO_RAD * currentPath.mKeyFrames[currentKey].mEuler.x,
														DEG_TO_RAD * currentPath.mKeyFrames[currentKey].mEuler.z );
			animT = currentPath.mKeyFrames[currentKey].mTranslate;
		}
		else
		{
			// Generate transform data by interpolating between the two keyframes
			const KeyFrame & keyOne = currentPath.mKeyFrames[currentKey];
			const KeyFrame & keyTwo = currentPath.mKeyFrames[currentKey + 1];

			f32 t1 = keyOne.mTime;
			f32 t2 = keyTwo.mTime;

			// Normalize the distance between the two keyframes
			f32 segLen = t2 - t1;
			f32 segStart = animTime - t1;
			f32 segNormalizedT = segStart / segLen;

			// Interpolate between 2 frames
			animR = Quaternion::Slerp( keyOne.mRotation, keyTwo.mRotation, segNormalizedT );
			Quaternion b = animR;

			Quaternion q1 = Quaternion::CreateFromYawPitchRoll( DEG_TO_RAD * keyOne.mEuler.y,
																DEG_TO_RAD * keyOne.mEuler.x,
																DEG_TO_RAD * keyOne.mEuler.z );

			Quaternion q2 = Quaternion::CreateFromYawPitchRoll( DEG_TO_RAD * keyTwo.mEuler.y,
																DEG_TO_RAD * keyTwo.mEuler.x,
																DEG_TO_RAD * keyTwo.mEuler.z );

			animR = Quaternion::Slerp( q1, q2, segNormalizedT );

			// standard linear interpolation
			animT = ( 1.0f - segNormalizedT ) * keyOne.mTranslate + segNormalizedT *  keyTwo.mTranslate;
		}

		// Remember the last keyframe
		data.mLastKey = currentKey;
	}

}