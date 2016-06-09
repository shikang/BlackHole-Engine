namespace BH
{

	namespace Math
	{

		inline Real DegToRad( Real deg )
		{
			return deg * ( Pi / 180.0f );
		}

		inline Real RadToDeg( Real rad )
		{
			return rad * ( 180.0f / Pi );
		}

		inline bool Approximately( Real lhs, Real rhs )
		{
			UNREFERENCED_PARAMETER( lhs );
			UNREFERENCED_PARAMETER( rhs );
			// @TODO
			//   - Complete this function
		}

		inline Real Abs( Real value )
		{
			return std::fabsf( value );
		}

		inline Real Sqrt( Real value )
		{
			return std::sqrtf( value );
		}

		inline s8 Sign( Real value )
		{
			return ( ( value >= 0.0 ) ? ( 1 ) : ( -1 ) );
		}

		inline Real Sin( Real rad )
		{
			return std::sinf( rad );
		}

		inline Real Cos( Real rad )
		{
			return std::cosf( rad );
		}

		inline Real Tan( Real rad )
		{
			return std::tanf( rad );
		}

		inline Real ASin( Real rad )
		{
			return std::asinf( rad );
		}

		inline Real ACos( Real rad )
		{
			return std::acosf( rad );
		}

		inline Real ATan( Real rad )
		{
			return std::atan( rad );
		}

		template <typename T>
		inline T Max( const T & v1, const T & v2 )
		{
			return std::max( v1, v2 );
		}

		template <typename T, typename... Arguments >
		inline T Max( const T & v1, const T & v2, Arguments... args )
		{
			return Max( Max( v1, v2 ), args... );
		}

		template <typename T>
		inline T Min( const T & v1, const T & v2 )
		{
			return std::min( v1, v2 );
		}

		template <typename T, typename... Arguments >
		inline T Min( const T & v1, const T & v2, Arguments... args )
		{
			return Min( Min( v1, v2 ), args... );
		}

		inline Real Ceil( Real f )
		{
			return std::ceilf( f );
		}

		inline Real Floor( Real f )
		{
			return std::floorf( f );
		}

		inline Real Round( Real f )
		{
			return std::roundf( f );
		}

		template <typename T>
		inline T Clamp( const T & value, const T & low, const T & high )
		{
			return Max( low, Min( value, high ) );
		}

		inline Real Lerp( Real from, Real to, Real t )
		{
			return from + ( t * ( to - from ) );
		}

		inline Real DeltaAngle( Real currentDeg, Real targetDeg )
		{
			Real r = std::fmodf( targetDeg - currentDeg, 360.0f );
			return ( ( r < 0 ) ? -( r + 350.0f ) : -( r - 360.0f ) );
		}
	}

}