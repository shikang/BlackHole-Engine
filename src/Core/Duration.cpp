// Precompiled Headers
#include "Core/StdAfx.h"

#include "Core/Duration.h"

namespace BH
{
	const Duration Duration::ZERO = Duration( 0.0f );

	Duration::Duration( )
	{

	}

	Duration::Duration( Real seconds ) :
		mDuration( std::chrono::duration_cast< std::chrono::system_clock::duration >(
		std::chrono::duration<Real>( seconds ) ) )
	{

	}

	Duration & Duration::operator+= ( const Duration & rhs )
	{
		mDuration += rhs.mDuration;
		return *this;
	}

	Duration & Duration::operator-= ( const Duration & rhs )
	{
		mDuration -= rhs.mDuration;
		return *this;
	}

	bool Duration::operator == ( const Duration & rhs )
	{
		return mDuration == rhs.mDuration;
	}

	bool Duration::operator != ( const Duration & rhs )
	{
		return mDuration != rhs.mDuration;
	}

	bool Duration::operator <= ( const Duration & rhs )
	{
		return mDuration <= rhs.mDuration;
	}

	bool Duration::operator <  ( const Duration & rhs )
	{
		return mDuration < rhs.mDuration;
	}

	bool Duration::operator >= ( const Duration & rhs )
	{
		return mDuration >= rhs.mDuration;
	}

	bool Duration::operator >( const Duration & rhs )
	{
		return mDuration > rhs.mDuration;
	}

	String Duration::ToStringMinutes( ) const
	{
		return std::to_string( std::chrono::duration<Real, std::ratio< 60 > >( mDuration ).count( ) ) + "min";
	}

	String Duration::ToStringSeconds( ) const
	{
		return std::to_string( std::chrono::duration<Real>( mDuration ).count( ) ) + "s";
	}

	String Duration::ToStringMilliseconds( ) const
	{
		return std::to_string( mDuration.count( ) ) + "ms";
	}

	const Duration operator + ( const Duration & lhs, const Duration & rhs )
	{
		Duration r = lhs;
		return r += rhs;
	}

	const Duration operator - ( const Duration & lhs, const Duration & rhs )
	{
		Duration r = lhs;
		return r -= rhs;
	}
}