#ifndef BH_QUATERNION_H
#define BH_QUATERNION_H

#include "Platform/Typedefs.h"
#include "Platform/Vector3f.h"

#include <DirectXMath.h>

namespace BH
{
	// Contains a vector and a angle
	// Angle to rotate about an axis
	class Quaternion : public DirectX::XMFLOAT4
	{
	public:
		static const Quaternion IDENTITY;		//!< Identity quaternion

	public:
		// Default Constructor (zero vector)
		Quaternion();

		// Paramaterized Constructor
		Quaternion( f32 X, f32 Y, f32 Z, f32 W );

		// Paramaterized Constructor
		Quaternion( const Vector3f & v, f32 scalar );

		// Copy Constructor
		Quaternion( const Quaternion & q );

	public:
		// Assignment operators
		Quaternion & operator = ( const Quaternion & rhs );

		// += operator
		Quaternion & operator += ( const Quaternion & rhs );

		// -= operator
		Quaternion & operator -= ( const Quaternion & rhs );

		// *= operator with scale
		Quaternion & operator *= ( f32 rhs );

		// *= operator with quaternion
		Quaternion & operator *= ( const Quaternion & rhs );

		// /= operator with quaternion
		Quaternion & operator /= ( const Quaternion & rhs );

		// Unary operators
		Quaternion operator - () const;

		// Equality operator
		bool operator == ( const Quaternion & rhs ) const;

		// Inequality operator
		bool operator != ( const Quaternion & rhs ) const;

	public:
		// Length
		f32 Length() const;

		// Squared Length
		f32 LengthSq() const;

		// Normalise
		void Normalize();

		// Conjugate (Negate vector part of quaternion)
		// Multiple by it's conjugate = [w^2 + v^2, ZERO] (Use to extract scalar and vector part)
		void Conjugate();

		// Inverse (Multiply by it's inverse, you get identity quaternion)
		void Inverse();

		// Dot product
		f32 Dot( const Quaternion & q ) const;

		// Cross product
		static Quaternion Cross( const Quaternion & q1, const Quaternion & q2 );

	public:
		// Static functions

		// Get quaternion from axis angle (radian)
		static Quaternion CreateFromAxisAngle( const Vector3f & axis, f32 angle );

		// Get quaternion from yaw, pitch, roll
		// yaw = angle of rotation around y axis, pitch = angle of rotation around x axis, roll = angle of rotation around z axis (radian)
		static Quaternion CreateFromYawPitchRoll( f32 yaw, f32 pitch, f32 roll );

		// Get quaternion from rotation matrix
		static Quaternion CreateFromRotationMatrix( const Matrix4& mat );

		// Linear interpolate
		static Quaternion Lerp( const Quaternion& q1, const Quaternion& q2, f32 t );

		// Spherical linear interpolate
		static Quaternion Slerp( const Quaternion& q1, const Quaternion& q2, f32 t );

		// Combine 2 rotation to 1
		static Quaternion Concatenate( const Quaternion& q1, const Quaternion& q2 );
	};

	// Plus operator
	Quaternion operator + ( const Quaternion & lhs, const Quaternion & rhs );

	// Minus operator
	Quaternion operator - ( const Quaternion & lhs, const Quaternion & rhs );

	// Multiply operator
	Quaternion operator * ( const Quaternion & lhs, const Quaternion & rhs );

	// Multiply operator
	Vector3f operator * ( const Quaternion & lhs, const Vector3f & rhs );

	// Multiply operator
	Quaternion operator * ( f32 lhs, const Quaternion & rhs );

	// Multiply operator
	Quaternion operator * ( const Quaternion & lhs, f32 rhs );

	// Divide operator
	Quaternion operator / ( const Quaternion & lhs, const Quaternion & rhs );
}

#include "Platform/Quaternion.inl"

#endif