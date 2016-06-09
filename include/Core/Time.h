#ifndef BH_TIME_H
#define BH_TIME_H

#include "Core/CoreEssential.h"
#include "Core/Duration.h"
#include "Core/TimePoint.h"

#include <chrono>

namespace BH
{

	class Time
	{
	public:
		static inline Real GetTime( );
		static inline Real GetFixedTime( );
		static inline Real GetUnscaledTime( );
		static inline Real GetDt( );
		static inline Real GetFixedDt( );
		static inline Real GetUnscaledDt( );
		static inline Real GetMaxDt( );

		static inline Real GetTimeScale( );
		static inline void SetTimeScale( Real t );

		static inline const TimePoint & GetGameStartTime( );
		static inline TimePoint GetCurrentGameTime( );

		static inline String ToString( Real seconds );

		// going to friend to class that is going to be incharge of the time
		friend class Game;

	private:
		// elapsed game time in seconds
		static Real mTime;
		// elapsed game time in seconds, fixed
		static Real mFixedTime;
		// elapsed unscale game time in seconds
		static Real mUnscaledTime;

		// dt
		static Real mDt;
		// fixed dt
		static Real mFixedDt;
		// unscaled dt
		static Real mUnscaledDt;

		// max Dt
		static Real mMaxDt;
		// time scale rate
		static Real mTimeScale;

		// start time
		static TimePoint mGameStartTime;
		
	};

}

#include "Core/Time.inl"

#endif