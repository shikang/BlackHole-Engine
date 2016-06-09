#ifndef BH_VECTOR4F_H
#define BH_VECTOR4F_H

#include "Platform/Typedefs.h"

#include <DirectXMath.h>

namespace BH
{
	class Vector4f : public DirectX::XMFLOAT4
	{
	public:
		static const Vector4f ZERO;		//!< Zero vector
		static const Vector4f ONE;		//!< One vector
		static const Vector4f UNIT_X;	//!< Unit x vector
		static const Vector4f UNIT_Y;	//!< Unit y vector
		static const Vector4f UNIT_Z;	//!< Unit z vector
		static const Vector4f UNIT_W;	//!< Unit w vector

	public:
		// Default Constructor (zero vector)
		Vector4f();

		// Fill Constructor (x and y has the same value)
		explicit Vector4f(f32 value);

		// Paramaterized Constructor
		Vector4f(f32 X, f32 Y, f32 Z, f32 W);

		// Copy Constructor
		Vector4f(const Vector4f & v);

	public:
		// Assignment operators
		Vector4f & operator = (const Vector4f & rhs);

		// += operator
		Vector4f & operator += (const Vector4f & rhs);

		// -= operator
		Vector4f & operator -= (const Vector4f & rhs);

		// *= operator with scale
		Vector4f & operator *= (f32 rhs);

		// *= operator with vector multiply by component
		Vector4f & operator *= (const Vector4f & rhs);

		// /= operator with scale (exception not handled when rhs is zero)
		Vector4f & operator /= (f32 rhs);

		// Unary operators
		Vector4f operator - () const;

		// Equality operator
		bool operator == (const Vector4f & rhs) const;

		// Inequality operator
		bool operator != (const Vector4f & rhs) const;

	public:
		/*
			(V.x <= bounds.x && V.x >= -bounds.x) &&
			(V.y <= bounds.y && V.y >= -bounds.y)

			This uses SSE, for faster checking
		*/
		bool InBounds(const Vector4f & bounds) const;

		// Make vector unit
		void Normalize();

		// Get the length of vector
		f32 Length() const;

		// Get the square length of vector
		f32 LengthSq() const;

		// Dot product
		f32 Dot(const Vector4f & v) const;

		// Clamp
		void Clamp(const Vector4f & min, const Vector4f & max);

	public:
		// Static functions

		// Normalise
		static Vector4f Normalize(const Vector4f & v);

		// Clamp
		static Vector4f Clamp(const Vector4f & v, const Vector4f & min, const Vector4f & max);

		// Distance between 2 vectors
		static f32 Distance(const Vector4f & v1, const Vector4f & v2);

		// Squared distance between 2 vectors
		static f32 DistanceSquared(const Vector4f & v1, const Vector4f & v2);

		// Get Min (Instead of have templated min function, overload < and >opertors, we need to have this function for SSE)
		static Vector4f Min(const Vector4f & v1, const Vector4f & v2);

		// Get Max
		static Vector4f Max(const Vector4f & v1, const Vector4f & v2);

		// Linear interpolation
		static Vector4f Lerp(const Vector4f & v1, const Vector4f & v2, f32 t);

		// Scalar interpolation
		static Vector4f SmoothStep(const Vector4f & v1, const Vector4f & v2, f32 t);

		// Get Barycentric
		static Vector4f Barycentric(const Vector4f & v1, const Vector4f & v2, const Vector4f & v3, f32 f, f32 g);

		// Catmull-Rom interpolation (Spline)
		static Vector4f CatmullRom(const Vector4f & v1, const Vector4f & v2, const Vector4f & v3, const Vector4f & v4, f32 t);

		// Hermite interpolation
		static Vector4f Hermite(const Vector4f & v1, const Vector4f & t1, const Vector4f & v2, const Vector4f & t2, f32 t);

		// Reflect vector
		static Vector4f Reflect(const Vector4f & ivec, const Vector4f & nvec);

		// Refract vector
		static Vector4f Refract(const Vector4f & ivec, const Vector4f & nvec, f32 refractionIndex);
	};

	// Plus operator
	Vector4f operator + (const Vector4f & lhs, const Vector4f & rhs);

	// Minus operator
	Vector4f operator - (const Vector4f & lhs, const Vector4f & rhs);

	// Multiply operator
	Vector4f operator * (const Vector4f & lhs, const Vector4f & rhs);

	// Multiply operator
	Vector4f operator * (f32 lhs, const Vector4f & rhs);

	// Multiply operator
	Vector4f operator * (const Vector4f & lhs, f32 rhs);

	// Divide operator
	Vector4f operator / (const Vector4f & lhs, f32 rhs);
}

#include "Platform/Vector4f.inl"

#endif