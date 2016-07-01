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

	private:
		Mesh mMesh;
		Skeleton mSkeleton;
		std::vector< Animation > mAnimations;

		friend class ModelManager;
		friend class Renderer;
	};
}

#endif