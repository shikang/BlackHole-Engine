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
		void ShutDown();

	private:
		// Initialise roots
		void InitialiseBones();

	private:
		std::vector<Bone> mBones;		//!< List of bones
		std::vector<Bone *> mRoots;		//!< List of root bones

	};
}

#endif