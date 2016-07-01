// Precompiled Header
#include "Core/Stdafx.h"

#include "Core/Model.h"

namespace BH
{
	Model::Model()
	{

	}

	Model::~Model()
	{
		
	}

	void Model::Shutdown()
	{
		mMesh.Shutdown();
		mSkeleton.Shutdown();
		mAnimations.clear();
	}
}