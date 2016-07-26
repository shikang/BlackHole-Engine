#ifndef BH_ANIMATION_H
#define BH_ANIMATION_H

#include "Platform/Typedefs.h"
#include "Platform/Vector3f.h"
#include "Platform/Vector4f.h"
#include "Platform/Quaternion.h"

#include <vector>

namespace BH
{
	/*
		Track data is used to help process the animation in this
		case it is the last keyframe the track was on
	*/
	struct TrackData
	{
		u32 mLastKey;
	};

	typedef std::vector<Matrix4>				MatrixBuffer;
	typedef std::vector<TrackData>				TrackBuffer;

	/*
		A Keyframe is a snapshot of the node at a point in time.
		When rendering the game will interpolate between
		keyframes to create smooth motion.
	*/
	struct KeyFrame
	{
		Quaternion mRotation;
		Vector3f mEuler;		// DEBUG
		Vector3f mTranslate;
		f32 mTime;	
	};

	/*
		A Track or Path is the set of keyframes that correspond to a particular node.
		Each track can have a different number of keyframe and the keyframes 
		do not have to be distributed on regular intervals.
	*/
	struct Track
	{
		std::vector< KeyFrame > mKeyFrames;
	};

	// An animation is a set of tracks
	struct Animation
	{
		std::vector< Track > mTracks;
		String mName;
		f32 mDuration;					//!< Duration of the Animation in Seconds

		void CalculateTransform( f32 animTime, u32 trackIndex, Vector3f & animT, Quaternion & animR, TrackData & data ) const;
	};
}

#endif