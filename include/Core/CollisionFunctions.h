#ifndef BH_COLLISION_FUNCTIONS_H
#define BH_COLLISION_FUNCTIONS_H

#include "Core/CoreEssential.h"
#include "Core/Contact.h"
#include "Core/OBB.h"

namespace BH
{
	bool OBBToOBB( Manifold & manifold, const OBB & lhs, const OBB & rhs, bool passable );
}

#endif