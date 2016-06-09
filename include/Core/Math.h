#ifndef BH_MATH_H
#define BH_MATH_H

#include "Core/CoreEssential.h"
#include "Core/Time.h"

namespace BH
{
	namespace Math
	{
		extern const Real Epsilon;
		extern const Real Pi;
		extern const Real Infinity;
		extern const Real NegInfinity;

		inline Real DegToRad( Real deg );
		inline Real RadToDeg( Real rad );

		inline bool Approximately( Real lhs, Real rhs );

		inline Real Abs( Real value );
		inline Real Sqrt( Real value );	

		inline s8 Sign( Real value );

		inline Real Sin( Real rad );
		inline Real Cos( Real rad );
		inline Real Tan( Real rad );

		inline Real ASin( Real f );
		inline Real ACos( Real f );
		inline Real ATan( Real f );

		template <typename T>
		inline T Max( const T & v1, const T & v2 );

		template <typename T, typename... Arguments >
		inline T Max( const T & v1, const T & v2, Arguments... args );

		template <typename T>
		inline T Min( const T & v1, const T & v2 );

		template <typename T, typename... Arguments >
		inline T Min( const T & v1, const T & v2, Arguments... args );

		inline Real Ceil( Real f );
		inline Real Floor( Real f );
		inline Real Round( Real f );

		template <typename T>
		inline T Clamp( const T & value, const T & low, const T & high );

		inline Real Lerp( Real from, Real to, Real t );

		inline Real DeltaAngle( Real currentDeg, Real targetDeg );

	};
}

#include "Core/Math.inl"

#endif