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
		A Keyframe is a snapshot of the node at a point in time.
		When rendering the game will interpolate between
		keyframes to create smooth motion.
	*/
	struct KeyFrame
	{
		Quaternion mRotation;
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
	};
}

#endif