// Precompiled Headers
#include "Core/StdAfx.h"
#include "Core/Math.h"

#include <math.h>
#include <limits>

namespace BH
{
	namespace Math
	{
		const Real Epsilon = std::numeric_limits<Real>::epsilon( );
		const Real Pi = 3.14159265359f;
		const Real Infinity = std::numeric_limits<Real>::infinity( );
		const Real NegInfinity = -1 * Math::Infinity;
	}

}