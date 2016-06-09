#ifndef BH_TIMEPOINT_H
#define BH_TIMEPOINT_H

#include "Core/CoreEssential.h"
#include "Core/Duration.h"

#include <chrono>

namespace BH
{

	class TimePoint
	{
	private:
		typedef std::chrono::system_clock::time_point TimePointType;

	public:
		TimePoint( );
		TimePoint( const TimePoint & rhs ) = default;
		TimePoint( const Duration & dur );
		TimePoint( const TimePointType & tp );

		static TimePoint Now( );

		TimePoint & operator += ( const Duration & dur );
		TimePoint & operator -= ( const Duration & dur );

		bool operator == ( const TimePoint & tp ) const;
		bool operator != ( const TimePoint & tp ) const;
		bool operator >= ( const TimePoint & tp ) const;
		bool operator >  ( const TimePoint & tp ) const;
		bool operator <= ( const TimePoint & tp ) const;
		bool operator <  ( const TimePoint & tp ) const;
		
		// refer to http://www.cplusplus.com/reference/ctime/strftime/ for format info
		String ToString( const String & format = "%a, %b %d %G, %r" ) const;

	private:
		TimePointType mTimePoint;
		
		friend const Duration  operator - ( const TimePoint & tp1, const TimePoint & tp2 );

	};

	const TimePoint operator + ( const TimePoint & tp, const Duration & dur );
	const TimePoint operator + ( const Duration & dur, const TimePoint & tp );
	const TimePoint operator - ( const TimePoint & tp, const Duration & dur );
	const Duration  operator - ( const TimePoint & tp1, const TimePoint & tp2 );

}

#endif