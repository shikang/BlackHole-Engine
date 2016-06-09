#ifndef BH_VECTOR2F_H
#define BH_VECTOR2F_H

#include "Platform/Typedefs.h"

#include <DirectXMath.h>

namespace BH
{
	/*
		This code doen't work!

		union
		{
			struct
			{
				f32 x;
				f32 y;
			};

			DirectX::XMFLOAT2 mVec2;
		};

		Because there is a constructor in DirectX::XMFLOAT2 structure, 
		therefore union fails.

		Only way to wrap the vector while allowing v.x and v.y is to
		inherit DirectX::XMFLOAT2. Because there won't be any vtable (extra memory)
		when there is no virtual function
	*/
	class Vector2f : public DirectX::XMFLOAT2
	{
	public:
		static const Vector2f ZERO;		//!< Zero vector
		static const Vector2f ONE;		//!< One vector
		static const Vector2f UNIT_X;	//!< Unit x vector
		static const Vector2f UNIT_Y;	//!< Unit y vector

	public:
		// Default Constructor (zero vector)
		Vector2f();

		// Fill Constructor (x and y has the same value)
		explicit Vector2f(f32 value);

		// Paramaterized Constructor
		Vector2f(f32 X, f32 Y);

		// Copy Constructor
		Vector2f(const Vector2f & v);

	public:
		// Assignment operators
		Vector2f & operator = (const Vector2f & rhs);

		// += operator
		Vector2f & operator += (const Vector2f & rhs);

		// -= operator
		Vector2f & operator -= (const Vector2f & rhs);

		// *= operator with scale
		Vector2f & operator *= (f32 rhs);

		// *= operator with vector multiply by component
		Vector2f & operator *= (const Vector2f & rhs);

		// /= operator with scale (exception not handled when rhs is zero)
		Vector2f & operator /= (f32 rhs);

		// Unary operators
		Vector2f operator - () const;

		// Equality operator
		bool operator == (const Vector2f & rhs) const;

		// Inequality operator
		bool operator != (const Vector2f & rhs) const;

	public:
		/*
			(V.x <= bounds.x && V.x >= -bounds.x) &&
			(V.y <= bounds.y && V.y >= -bounds.y)

			This uses SSE, for faster checking
		*/
		bool InBounds(const Vector2f & bounds) const;

		// Make vector unit
		void Normalize();

		// Get the length of vector
		f32 Length() const;

		// Get the square length of vector
		f32 LengthSq() const;

		// Dot product
		f32 Dot(const Vector2f & v) const;

		// Get the z component of the cross product
		f32 ScalarTripleProduct(const Vector2f & v) const;

		// Clamp
		void Clamp(const Vector2f & min, const Vector2f & max);

	public:
		// Static functions

		// Normalise
		static Vector2f Normalize(const Vector2f & v);

		// Clamp
		static Vector2f Clamp(const Vector2f & v, const Vector2f & min, const Vector2f & max);

		// Distance between 2 vectors
		static f32 Distance(const Vector2f & v1, const Vector2f & v2);

		// Squared distance between 2 vectors
		static f32 DistanceSquared(const Vector2f & v1, const Vector2f & v2);

		// Get Min (Instead of have templated min function, overload < and >opertors, we need to have this function for SSE)
		static Vector2f Min(const Vector2f & v1, const Vector2f & v2);

		// Get Max
		static Vector2f Max(const Vector2f & v1, const Vector2f & v2);

		// Linear interpolation
		static Vector2f Lerp(const Vector2f & v1, const Vector2f & v2, f32 t);

		// Scalar interpolation
		static Vector2f SmoothStep(const Vector2f & v1, const Vector2f & v2, f32 t);

		// Get Barycentric
		static Vector2f Barycentric(const Vector2f & v1, const Vector2f & v2, const Vector2f & v3, f32 f, f32 g);

		// Catmull-Rom interpolation (Spline)
		static Vector2f CatmullRom(const Vector2f & v1, const Vector2f & v2, const Vector2f & v3, const Vector2f & v4, f32 t);

		// Hermite interpolation
		static Vector2f Hermite(const Vector2f & v1, const Vector2f & t1, const Vector2f & v2, const Vector2f & t2, f32 t);

		// Reflect vector
		static Vector2f Reflect(const Vector2f & ivec, const Vector2f & nvec);

		// Refract vector
		static Vector2f Refract(const Vector2f & ivec, const Vector2f & nvec, f32 refractionIndex);
	};

	// Plus operator
	Vector2f operator + (const Vector2f & lhs, const Vector2f & rhs);

	// Minus operator
	Vector2f operator - (const Vector2f & lhs, const Vector2f & rhs);

	// Multiply operator
	Vector2f operator * (const Vector2f & lhs, const Vector2f & rhs);

	// Multiply operator
	Vector2f operator * (f32 lhs, const Vector2f & rhs);

	// Multiply operator
	Vector2f operator * (const Vector2f & lhs, f32 rhs);

	// Divide operator
	Vector2f operator / (const Vector2f & lhs, f32 rhs);
}

#include "Platform/Vector2f.inl"

#endif