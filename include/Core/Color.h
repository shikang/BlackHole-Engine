#ifndef BH_COLOR_H
#define BH_COLOR_H

#include "Core/CoreEssential.h"

namespace BH
{
	class Color
	{
	public:
		static const Color CLEAR;
		static const Color BLACK;
		static const Color WHITE;
		static const Color BLUE;
		static const Color GREEN;
		static const Color RED;
		static const Color YELLOW;
		static const Color CYAN;
		static const Color MAGENTA;

	public:
		union
		{
			struct
			{
				Real r, g, b, a;
			};

			Real value[ 4 ];
		};

	public:
		Color( );
		Color( Real R, Real G, Real B, Real A );
		Color( const Vector4f & color );

	public:
		Color & operator += ( const Color & rhs );
		Color & operator -= ( const Color & rhs );
		Color & operator *= ( const Color & rhs );
		Color & operator *= ( Real rhs );
		Color & operator /= ( const Color & rhs );
		Color & operator /= ( Real rhs );

		operator Vector4f ( ) const;

	public:
		String ToString( ) const;

	public:
		static Color Lerp( const Color & from, const Color & to, const Real t );

	};


	const Color operator + ( const Color & lhs, const Color & rhs );
	const Color operator - ( const Color & lhs, const Color & rhs );
	
	const Color operator * ( const Color & lhs, const Color & rhs );
	const Color operator * ( const Color & lhs, Real rhs );
	const Color operator * ( Real lhs, const Color & rhs );
	
	const Color operator / ( const Color & lhs, const Color & rhs );
	const Color operator / ( const Color & lhs, Real rhs );
	const Color operator / ( Real lhs, const Color & rhs );

}



#endif