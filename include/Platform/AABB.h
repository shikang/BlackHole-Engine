#ifndef BH_AABB_H
#define BH_AABB_H

#include "Core/CoreEssential.h"

#include "Platform/Vector3f.h"

namespace BH
{
	struct AABB
	{
		Vector3f mMin;		//!< Min point
		Vector3f mMax;		//!< Max point

		// Constructor
		AABB() : mMin( Vector3f::ZERO ), mMax( Vector3f::ONE ) {}
		AABB( const Vector3f & min, const Vector3f & max ) : mMin ( min ), mMax( max ) {}
	};
}

#endif