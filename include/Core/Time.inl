namespace BH
{ 
	inline Real Time::GetTime( )
	{
		return mTime;
	}

	inline Real Time::GetFixedTime( )
	{
		return mFixedTime;
	}

	inline Real Time::GetUnscaledTime( )
	{
		return mUnscaledTime;
	}
	inline Real Time::GetDt( )
	{
		return mDt;
	}

	inline Real Time::GetFixedDt( )
	{
		return mFixedDt;
	}

	inline Real Time::GetUnscaledDt( )
	{
		return mUnscaledDt;
	}

	inline Real Time::GetMaxDt( )
	{
		return mMaxDt;
	}

	inline Real Time::GetTimeScale( )
	{
		return mTimeScale;
	}

	inline void Time::SetTimeScale( Real t )
	{
		mTimeScale = t;
	}

	inline const TimePoint & Time::GetGameStartTime( )
	{
		return mGameStartTime;
	}

	inline TimePoint Time::GetCurrentGameTime( )
	{
		Duration d( GetTime( ) );
		return GetGameStartTime( ) + d;
	}
	
}