#ifndef BH_DURATION_H
#define BH_DURATION_H

#include "Core/CoreEssential.h"

#include <chrono>

namespace BH
{

	class Duration
	{
	private:
		friend class TimePoint;

	private:
		typedef Real DataType;
		typedef std::milli DurationType;

	public:
		static const Duration ZERO;

	public:
		Duration( );
		Duration( const Duration & dur ) = default;
		Duration( Real seconds );

		Duration & operator = ( const Duration & rhs ) = default;

		Duration & operator += ( const Duration & rhs );
		Duration & operator -= ( const Duration & rhs );

		bool operator == ( const Duration & rhs );
		bool operator != ( const Duration & rhs );
		bool operator <= ( const Duration & rhs );
		bool operator <  ( const Duration & rhs );
		bool operator >= ( const Duration & rhs );
		bool operator >  ( const Duration & rhs );

		String ToStringMinutes( ) const;
		String ToStringSeconds( ) const;
		String ToStringMilliseconds( ) const;

	private:
		std::chrono::system_clock::duration mDuration;
	};

	const Duration operator + ( const Duration & lhs, const Duration & rhs );
	const Duration operator - ( const Duration & lhs, const Duration & rhs );


}

#endif