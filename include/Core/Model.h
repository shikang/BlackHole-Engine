#ifndef BH_MODEL_H
#define BH_MODEL_H

#include "Core/CoreEssential.h"

#include "Platform/Mesh.h"
#include "Platform/Animation.h"
#include "Platform/Skeleton.h"

namespace BH
{
	class Model
	{
	public:
		// Constructor
		Model();

		// Destructor
		~Model();

		// Shutdown
		void Shutdown();

		inline bool IsAnimatable() const { return !mAnimations.empty() && mSkeleton.HasBones(); };

	private:
		String mName;	// Debug purpose
		Mesh mMesh;
		Skeleton mSkeleton;
		std::vector< Animation > mAnimations;

		friend class ModelManager;
		friend class AnimationController;
		friend class Renderer;
	};
}

#endif