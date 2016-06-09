namespace BH
{
#if 0
	template <typename T>
	void Stopwatch<T>::Start( bool result )
	{
		if ( mStarted == true )
			return;

		mStarted = true;
		mPoint1 = T::now( );
	}

	template <typename T>
	Real Stopwatch<T>::Stop( )
	{
		if ( mStarted == false )
			return 0.0f;

		mStarted = false;
		mPoint2 = T::now( );

		return std::chrono::duration_cast< std::chrono::duration < Real > >( mPoint2 - mPoint1 ).count( );
	}

	template <typename T>
	void Stopwatch<T>::Reset( )
	{
		mStarted = false;
	}
#endif

}