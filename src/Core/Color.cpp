// Precompiled Headers
#include "Core/StdAfx.h"

#include "Core/Color.h"
#include "Core/Math.h"

namespace BH
{

	const Color Color::CLEAR = Color( 0.0f, 0.0f, 0.0f, 0.0f );
	const Color Color::BLACK = Color( 1.0f, 1.0f, 1.0f, 1.0f );
	const Color Color::WHITE = Color( 0.0f, 0.0f, 0.0f, 1.0f );

	const Color Color::BLUE = Color( 0.0f, 0.0f, 1.0f, 1.0f );
	const Color Color::GREEN = Color( 0.0f, 1.0f, 0.0f, 1.0f );
	const Color Color::RED = Color( 1.0f, 0.0f, 0.0f, 1.0f );

	const Color Color::YELLOW = Color( 1.0f, 0.92f, 0.016f, 1.0f );
	const Color Color::CYAN = Color( 0.0f, 1.0f, 1.0f, 1.0f );
	const Color Color::MAGENTA = Color( 1.0f, 0.0f, 1.0f, 1.0f );

	Color::Color( )
	{

	}

	Color::Color( Real R, Real G, Real B, Real A ) :
		r( R ), g( G ), b( B ), a( A )
	{

	}

	Color::Color( const Vector4f & color ) :
		Color( color.x, color.y, color.z, color.w )
	{

	}

	Color & Color::operator+= ( const Color & rhs )
	{
		r += std::max( 1.0f, rhs.r );
		g += std::max( 1.0f, rhs.g );
		b += std::max( 1.0f, rhs.b );
		a += std::max( 1.0f, rhs.a );

		return *this;
	}

	Color & Color::operator-= ( const Color & rhs )
	{
		r -= std::min( 0.0f, rhs.r );
		g -= std::min( 0.0f, rhs.g );
		b -= std::min( 0.0f, rhs.b );
		a -= std::min( 0.0f, rhs.a );

		return *this;
	}


	Color & Color::operator*= ( const Color & rhs )
	{
		r *= rhs.r;
		g *= rhs.g;
		b *= rhs.b;
		a *= rhs.a;

		return *this;
	}

	Color & Color::operator *= ( Real rhs )
	{
		r *= rhs;
		g *= rhs;
		b *= rhs;
		a *= rhs;

		return *this;
	}

	Color & Color::operator/= ( const Color & rhs )
	{
		r /= rhs.r;
		g /= rhs.g;
		b /= rhs.b;
		a /= rhs.a;

		return *this;
	}

	Color & Color::operator /= ( Real rhs )
	{
		r /= rhs;
		g /= rhs;
		b /= rhs;
		a /= rhs;
		
		return *this;
	}

	Color::operator Vector4f ( ) const
	{
		return Vector4f( r, g, b, a );
	}

	String Color::ToString( ) const
	{
		Ostringstream oss;
		oss << "r: " << r << ", g: " << g << ", b: " << b << ", a: " << a;
		return String( oss.str( ) );
	}

	Color Color::Lerp( const Color & from, const Color & to, const Real t )
	{
		return Color ( Math::Lerp( from.r, to.r, t ), 
					   Math::Lerp( from.g, to.g, t ), 
					   Math::Lerp( from.b, to.b, t ), 
					   Math::Lerp( from.a, to.a, t ) );
	}

	const Color operator + ( const Color & lhs, const Color & rhs )
	{
		Color r = lhs;
		return r += rhs;
	}

	const Color operator - ( const Color & lhs, const Color & rhs )
	{
		Color r = lhs;
		return r -= rhs;
	}

	const Color operator * ( const Color & lhs, const Color & rhs )
	{
		Color r = lhs;
		return r *= rhs;
	}

	const Color operator * ( const Color & lhs, Real rhs )
	{
		Color r = lhs;
		return r *= rhs;
	}

	const Color operator * ( Real lhs, const Color & rhs )
	{
		Color r = rhs;
		return r *= lhs;
	}

	const Color operator / ( const Color & lhs, const Color & rhs )
	{
		Color r = lhs;
		return r /= rhs;
	}

	const Color operator / ( const Color & lhs, Real rhs )
	{
		Color r = lhs;
		return r /= rhs;
	}

	const Color operator / ( Real lhs, const Color & rhs )
	{
		Color r = rhs;
		return r /= lhs;
	}

}