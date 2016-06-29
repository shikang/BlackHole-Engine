// Precompiled Header
#include "Platform/Stdafx.h"

#include "Platform/Skeleton.h"

namespace BH
{
	Skeleton::Skeleton()
	{

	}

	Skeleton::~Skeleton()
	{

	}

	void Skeleton::Initialise( const std::vector<Bone> & bones )
	{
		mBones = bones;
		InitialiseBones();
	}

	void Skeleton::Initialise( std::vector<Bone> && bones )
	{
		mBones = std::move( bones );
		InitialiseBones();
	}

	void Skeleton::ShutDown()
	{
		mBones.clear();
		mRoots.clear();
	}

	void Skeleton::InitialiseBones()
	{
		s32 size = mBones.size();

		for ( s32 i = 0; i < size; ++i )
		{
			Bone & b = mBones[i];
			b.boneIndex = i;

			if ( b.parentIndex != -1 )
				mBones[b.parentIndex].children.push_back( &b );
			else
				mRoots.push_back( &b );

			b.modelToBoneSpace = Matrix4::CreateTranslation( b.modelToBoneTranslation ) * Matrix4::CreateFromQuaternion( b.modelToBoneRotation );
			b.bindTransform = Matrix4::CreateTranslation( b.bindTranslation ) * Matrix4::CreateFromQuaternion( b.bindRotation );
		}
	}
}