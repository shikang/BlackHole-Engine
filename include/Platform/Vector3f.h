#ifndef BH_VECTOR3F_H
#define BH_VECTOR3F_H

#include "Platform/Typedefs.h"

#include <DirectXMath.h>

namespace BH
{
	class Matrix4;

	class Vector3f : public DirectX::XMFLOAT3
	{
	public:
		static const Vector3f ZERO;		//!< Zero vector
		static const Vector3f ONE;		//!< One vector
		static const Vector3f UNIT_X;	//!< Unit x vector
		static const Vector3f UNIT_Y;	//!< Unit y vector
		static const Vector3f UNIT_Z;	//!< Unit z vector

	public:
		// Default Constructor (zero vector)
		Vector3f();

		// Fill Constructor (x and y has the same value)
		explicit Vector3f( f32 value );

		// Paramaterized Constructor
		Vector3f( f32 X, f32 Y, f32 Z );

		// Copy Constructor
		Vector3f( const Vector3f & v );

	public:
		// Assignment operators
		Vector3f & operator = ( const Vector3f & rhs );

		// += operator
		Vector3f & operator += ( const Vector3f & rhs );

		// -= operator
		Vector3f & operator -= ( const Vector3f & rhs );

		// *= operator with scale
		Vector3f & operator *= ( f32 rhs );

		// *= operator with vector multiply by component
		Vector3f & operator *= ( const Vector3f & rhs );

		// /= operator with scale (exception not handled when rhs is zero)
		Vector3f & operator /= ( f32 rhs );

		// Unary operators
		Vector3f operator - () const;

		f32 operator [] ( u32 index ) const;

		// Equality operator
		bool operator == ( const Vector3f & rhs ) const;

		// Inequality operator
		bool operator != ( const Vector3f & rhs ) const;

	public:
		/*
			(V.x <= bounds.x && V.x >= -bounds.x) &&
			(V.y <= bounds.y && V.y >= -bounds.y)

			This uses SSE, for faster checking
		*/
		bool InBounds( const Vector3f & bounds ) const;

		// Make vector unit
		void Normalize();

		// Get the length of vector
		f32 Length() const;

		// Get the square length of vector
		f32 LengthSq() const;

		// Dot product
		f32 Dot( const Vector3f & v ) const;

		// Get the z component of the cross product
		Vector3f Cross( const Vector3f & v ) const;

		// Clamp
		void Clamp( const Vector3f & min, const Vector3f & max );

	public:
		// Static functions

		// Normalise
		static Vector3f Normalize( const Vector3f & v );

		// Clamp
		static Vector3f Clamp( const Vector3f & v, const Vector3f & min, const Vector3f & max );

		// Distance between 2 vectors
		static f32 Distance( const Vector3f & v1, const Vector3f & v2 );

		// Squared distance between 2 vectors
		static f32 DistanceSquared( const Vector3f & v1, const Vector3f & v2 );

		// Get Min (Instead of have templated min function, overload < and >opertors, we need to have this function for SSE)
		static Vector3f Min( const Vector3f & v1, const Vector3f & v2 );

		// Get Max
		static Vector3f Max( const Vector3f & v1, const Vector3f & v2 );

		// Linear interpolation
		static Vector3f Lerp( const Vector3f & v1, const Vector3f & v2, f32 t );

		// Scalar interpolation
		static Vector3f SmoothStep( const Vector3f & v1, const Vector3f & v2, f32 t );

		// Get Barycentric
		static Vector3f Barycentric( const Vector3f & v1, const Vector3f & v2, const Vector3f & v3, f32 f, f32 g );

		// Catmull-Rom interpolation (Spline)
		static Vector3f CatmullRom( const Vector3f & v1, const Vector3f & v2, const Vector3f & v3, const Vector3f & v4, f32 t );

		// Hermite interpolation
		static Vector3f Hermite( const Vector3f & v1, const Vector3f & t1, const Vector3f & v2, const Vector3f & t2, f32 t );

		// Reflect vector
		static Vector3f Reflect( const Vector3f & ivec, const Vector3f & nvec );

		// Refract vector
		static Vector3f Refract( const Vector3f & ivec, const Vector3f & nvec, f32 refractionIndex );

		// Unproject vector from screen to world
		static Vector3f Unproject( const Vector3f & vec, f32 viewportX, f32 viewportY, f32 viewportWidth, f32 viewportHeight, 
								   f32 viewportNear, f32 viewportFar, const Matrix4 & projection, const Matrix4 & view,
								   const Matrix4 & world );
	};

	// Plus operator
	Vector3f operator + ( const Vector3f & lhs, const Vector3f & rhs );

	// Minus operator
	Vector3f operator - ( const Vector3f & lhs, const Vector3f & rhs );

	// Multiply operator
	Vector3f operator * ( const Vector3f & lhs, const Vector3f & rhs );

	// Multiply operator
	Vector3f operator * ( f32 lhs, const Vector3f & rhs );

	// Multiply operator
	Vector3f operator * ( const Vector3f & lhs, f32 rhs );

	// Divide operator
	Vector3f operator / ( const Vector3f & lhs, f32 rhs );
}

#include "Platform/Vector3f.inl"

#endif