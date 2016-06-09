// Precompiled Headers
#include "Core/StdAfx.h"

#include "Core/Time.h"
#include "Core/TimePoint.h"

namespace BH
{

	TimePoint::TimePoint( )
	{

	}

	TimePoint::TimePoint( const Duration & dur ) :
		TimePoint( Time::GetGameStartTime( ) + dur )
	{

	}

	TimePoint::TimePoint( const TimePointType & tp ) :
		mTimePoint( tp )
	{

	}

	TimePoint TimePoint::Now( )
	{
		return TimePoint( std::chrono::system_clock::now( ) );
	}

	TimePoint & TimePoint::operator += ( const Duration & dur )
	{
		mTimePoint += dur.mDuration;
		return *this;
	}

	TimePoint & TimePoint::operator -= ( const Duration & dur )
	{
		mTimePoint -= dur.mDuration;
		return *this;
	}

	bool TimePoint::operator == ( const TimePoint & tp ) const
	{
		return mTimePoint == tp.mTimePoint;
	}

	bool TimePoint::operator != ( const TimePoint & tp ) const
	{
		return mTimePoint != tp.mTimePoint;
	}

	bool TimePoint::operator >= ( const TimePoint & tp ) const
	{
		return mTimePoint >= tp.mTimePoint;
	}

	bool TimePoint::operator >  ( const TimePoint & tp ) const
	{
		return mTimePoint > tp.mTimePoint;
	}

	bool TimePoint::operator <= ( const TimePoint & tp ) const
	{
		return mTimePoint <= tp.mTimePoint;
	}

	bool TimePoint::operator <  ( const TimePoint & tp ) const
	{
		return mTimePoint < tp.mTimePoint;
	}

	String TimePoint::ToString( const String & format ) const
	{
		static Char buffer[ 255 ];

		std::time_t tt = std::chrono::system_clock::to_time_t( mTimePoint );
		struct tm * timeinfo = std::localtime( &tt );

		strftime( buffer, 255, format.c_str( ), timeinfo );
		return String( buffer );
	}

	const TimePoint operator + ( const TimePoint & tp, const Duration & dur )
	{
		TimePoint t( tp );
		return t += dur;
	}

	const TimePoint operator + ( const Duration & dur, const TimePoint & tp )
	{
		TimePoint t( tp );
		return t += dur;
	}

	const TimePoint operator - ( const TimePoint & tp, const Duration & dur )
	{
		TimePoint t( tp );
		return t -= dur;
	}

	const Duration  operator - ( const TimePoint & tp1, const TimePoint & tp2 )
	{
		Duration d( Real( ( tp1.mTimePoint - tp2.mTimePoint ).count() * 
						  TimePoint::TimePointType::duration::period::num / 
						  TimePoint::TimePointType::duration::period::den ) );
		return d;
	}
}