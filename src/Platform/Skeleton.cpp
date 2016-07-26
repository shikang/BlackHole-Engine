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

	void Skeleton::Shutdown()
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

	void Skeleton::ProcessAnimationGraph( f32 time, MatrixBuffer & buffer,
										  const Animation & anim, TrackBuffer & trackData ) const
	{
		Matrix4 transform = Matrix4::IDENTITY;

		u32 size = mRoots.size();
		for ( u32 i = 0; i < size; ++i )
			RecursiveProcess( time, *mRoots[i], anim, buffer, trackData, transform );
	}

	void Skeleton::RecursiveProcess( f32 time, const Bone & bone, const Animation & anim, MatrixBuffer & buffer,
									 TrackBuffer & trackData, Matrix4 parentTransform ) const
	{
		Vector3f t;
		Quaternion r;

		time = 0.0f;
		//f32 angle = time / anim.mDuration * 6.28319f;

		anim.CalculateTransform( time, bone.boneIndex, t, r, trackData[bone.boneIndex] );

		//Matrix4 localTransform = bone.bindTransform;
		//Matrix4 localTransform = Matrix4::CreateTranslation(bone.bindTranslation) * Matrix4::CreateFromQuaternion(bone.bindRotation);
		//Matrix4 localTransform = Matrix4::IDENTITY;
		//Matrix4 localTransform = Matrix4::CreateTranslation(t) * Matrix4::CreateFromQuaternion(Quaternion::IDENTITY);
		Matrix4 localTransform = Matrix4::CreateTranslation(t) * Matrix4::CreateFromQuaternion(r);
		//Matrix4 localTransform = Matrix4::CreateTranslation(t) * Matrix4::CreateFromYawPitchRoll( angle, 0.0f, 0.0f );
		//Matrix4 localTransform = Matrix4::CreateTranslation(t) * Matrix4::CreateFromQuaternion(Quaternion::CreateFromAxisAngle(Vector3f::UNIT_Y, angle));
		//Matrix4 modelTransform = localTransform  * parentTransform;
		Matrix4 modelTransform = parentTransform * localTransform;

		buffer[bone.boneIndex] = modelTransform;

		u32 size = bone.children.size();
		for ( u32 i = 0; i < size; ++ i)
		{
			RecursiveProcess( time, *bone.children[i], anim, buffer, trackData, modelTransform );
		}
	}
}