#ifndef BH_OBB_H
#define BH_OBB_H

#include "Core/CoreEssential.h"

#include "Platform/Vector3f.h"

namespace BH
{
	struct OBB
	{
		Vector3f mAxis[3];		//!< Orientation
		Vector3f mPosition;		//!< Position
		Vector3f mHalfWidth;	//!< Axis Width

		OBB()
		: mPosition( Vector3f::ZERO )
		, mHalfWidth( Vector3f::ONE ) 
		{
			mAxis[0] = Vector3f::UNIT_X;
			mAxis[1] = Vector3f::UNIT_Y;
			mAxis[2] = Vector3f::UNIT_Z;
		}

		OBB( Vector3f axis[3], const Vector3f & position, const Vector3f & widths )
		: mPosition( position )
		, mHalfWidth( widths ) 
		{
			mAxis[0] = axis[0];
			mAxis[1] = axis[1];
			mAxis[2] = axis[2];
		}
	};
}

#endif