#ifndef BH_SKELETON_H
#define BH_SKELETON_H

#include "Platform/Typedefs.h"
#include "Platform/Bone.h"

namespace BH
{
	class Skeleton
	{
	public:
		// Constructor
		Skeleton();

		// Destructor
		~Skeleton();

	public:
		// Initialise skeleton
		void Initialise( const std::vector<Bone> & bones );

		void Initialise( std::vector<Bone> && bones );

		// Destory skeleton
		void Shutdown();

		inline bool HasBones() const { return !mBones.empty(); }

	private:
		// Initialise roots
		void InitialiseBones();

		// Process animation graph
		void ProcessAnimationGraph( f32 time, MatrixBuffer & buffer, 
									const Animation & anim, TrackBuffer & trackData ) const;

		// Recursive process
		void RecursiveProcess( f32 time, Bone & bone, const Animation & anim, MatrixBuffer & buffer, 
							   TrackBuffer & trackData, Matrix4 parentTransform ) const;

	private:
		std::vector<Bone> mBones;		//!< List of bones
		std::vector<Bone *> mRoots;		//!< List of root bones

		friend class AnimationController;
	};
}

#endif