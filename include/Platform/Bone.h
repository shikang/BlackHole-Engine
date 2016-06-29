#ifndef BH_BONE_H
#define BH_BONE_H

#include "Platform/Typedefs.h"
#include "Platform/Vector3f.h"
#include "Platform/Vector4f.h"
#include "Platform/Matrix4.h"
#include "Platform/Quaternion.h"

#include <vector>

namespace BH
{
	struct Bone
	{
		String name;
		s32 parentIndex;
		s32 boneIndex;

		Vector3f bindTranslation;			//!< Translation of the bone to bind position
		Quaternion bindRotation;			//!< Rotation of the bone to bind position
		Matrix4 bindTransform;				//!< Transform of the bone to bind position

		Matrix4 modelToBoneSpace;			//!< Transform from model space to bone space (Inverse of bindTransform)
		Vector3f modelToBoneTranslation;	//!< Translate from model space to bone space (Inverse of bindTranslation)
		Quaternion modelToBoneRotation;		//!< Rotate from model space to bone space (Inverse of bindRotation)

		std::vector< Bone * > children;
	};
}

#endif