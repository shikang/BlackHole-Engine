#ifndef BH_MATRIX4_H
#define BH_MATRIX4_H

#include "Platform/Typedefs.h"
#include "Platform/Vector3f.h"
#include "Platform/Vector4f.h"
#include <DirectXMath.h>

namespace BH
{
	// Right hand coordinate system, Row major matrix for direct x
	class Matrix4 : public DirectX::XMFLOAT4X4
	{
	public:
		static const Matrix4 IDENTITY;	//!< Identity Matrix

	public:
		// Default Constructor (Zero Matrix)
		Matrix4();

		// Constructor (Row major)
		Matrix4(f32 m00, f32 m01, f32 m02, f32 m03,
				f32 m10, f32 m11, f32 m12, f32 m13,
				f32 m20, f32 m21, f32 m22, f32 m23,
				f32 m30, f32 m31, f32 m32, f32 m33);

		/*
			In column major: 	  In row major: 
			[x.x y.x z.x 0.0]	  [x.x x.y x.z 0.0]
			[x.y y.y z.y 0.0]	  [y.x y.y y.z 0.0]
			[x.z y.z z.z 0.0]	  [z.x z.y z.z 0.0]
			[0.0 0.0 0.0 1.0]	  [0.0 0.0 0.0 1.0]
		*/
		Matrix4(const Vector3f & x, const Vector3f & y, const Vector3f & z);

		/*
			In column major: 	  In row major:
			[x.x y.x z.x w.x]	  [x.x x.y x.z x.w]
			[x.y y.y z.y w.y]	  [y.x y.y y.z y.w]
			[x.z y.z z.z w.z]	  [z.x z.y z.z z.w]
			[x.w y.w z.w w.w]	  [w.x w.y w.z w.w]
		*/
		Matrix4(const Vector4f & x, const Vector4f & y, const Vector4f & z, const Vector4f & w);

		// Copy Constructor
		Matrix4(const Matrix4 & m);

	public:
		// Assignment operator
		Matrix4 & operator = (const Matrix4 & rhs) { memcpy(this, &rhs, sizeof(f32)* 16); return *this; }

		// += operator
		Matrix4 & operator += (const Matrix4 & rhs);

		// -= operator
		Matrix4 & operator -= (const Matrix4 & rhs);

		// *= operator
		Matrix4 & operator *= (const Matrix4 & rhs);

		// *= operator
		Matrix4 & operator *= (f32 rhs);

		// /= operator
		Matrix4 & operator /= (f32 rhs);

		// Divide by element
		Matrix4 & operator /= (const Matrix4 & rhs);

		// Negate operator
		Matrix4 operator - () const;

		// Equality operator
		bool operator == (const Matrix4 & rhs) const;

		// Inequality operator
		bool operator != (const Matrix4 & rhs) const;

	public:
		// Get the up vector part from matrix (y)
		inline Vector3f Up() const { return Vector3f(_21, _22, _23); }

		// Get the side vector part from matrix (x)
		inline Vector3f Side() const { return Vector3f(_11, _12, _13); }

		// Get the forward vector part from matrix (z)
		inline Vector3f Forward() const  { return Vector3f(-_31, -_32, -_33); }

		// Get translation part from matrix
		inline Vector3f Translation() const { return Vector3f(_41, _42, _43); }

		// Set the up vector part from matrix (y)
		inline void SetUp(const Vector3f & v) { _21 = v.x; _22 = v.y; _23 = v.z; }

		// Set the side vector part from matrix (x)
		inline void SetSide(const Vector3f & v) { _11 = v.x; _12 = v.y; _13 = v.z; }

		// Set the forward vector part from matrix (z)
		inline void SetForward(const Vector3f & v) { _31 = -v.x; _32 = -v.y; _33 = -v.z; }

		// Set translation part from matrix
		inline void SetTranslation(const Vector3f & v) { _41 = v.x; _42 = v.y; _43 = v.z; }

	public:
		// Get the scale, rotation and translation from the transform matrix
		bool Decompose(Vector3f & scale, Quaternion & rotation, Vector3f & translation);

		// Transpose matrix
		Matrix4 Transpose() const;

		// Inverse matrix
		Matrix4 Inverse() const;

		// Get determinant
		f32 Determinant() const;

	public:
		// Static Functions

		// Billboard
		static Matrix4 CreateBillboard(const Vector3f & object, const Vector3f & cameraPosition, const Vector3f & cameraUp, const Vector3f & cameraView);

		// Constrained Billboard
		static Matrix4 CreateConstrainedBillboard(const Vector3f & object, const Vector3f & cameraPosition, const Vector3f & rotateAxis,
												  const Vector3f & cameraView, const Vector3f & objectForward);

		// Translation Matrix
		static Matrix4 CreateTranslation(const Vector3f & position);

		// Translation Matrix
		static Matrix4 CreateTranslation(f32 x, f32 y, f32 z);

		// Scale Matrix
		static Matrix4 CreateScale(const Vector3f & scales);

		// Scale Matrix
		static Matrix4 CreateScale(f32 xs, f32 ys, f32 zs);

		// Scale Matrix
		static Matrix4 CreateScale(f32 scale);

		// Rotation Matrix about x axis
		static Matrix4 CreateRotationX(f32 radians);

		// Rotation Matrix about y axis
		static Matrix4 CreateRotationY(f32 radians);

		// Rotation Matrix about z axis
		static Matrix4 CreateRotationZ(f32 radians);

		// Rotation Matrix from axis angle
		static Matrix4 CreateFromAxisAngle(const Vector3f & axis, f32 angle);

		// Perspective matrix using fov
		static Matrix4 CreatePerspectiveFieldOfView(f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane);

		// Perspective matrix (width and height of near plane)
		static Matrix4 CreatePerspective(f32 width, f32 height, f32 nearPlane, f32 farPlane);

		// Asymmetric perspective
		static Matrix4 CreatePerspectiveOffCenter(f32 left, f32 right, f32 bottom, f32 top, f32 nearPlane, f32 farPlane);

		// Orthographic matrix
		static Matrix4 CreateOrthographic(f32 width, f32 height, f32 zNearPlane, f32 zFarPlane);

		// Asymmetrix orthograhic
		static Matrix4 CreateOrthographicOffCenter(f32 left, f32 right, f32 bottom, f32 top, f32 zNearPlane, f32 zFarPlane);

		// View matrix
		static Matrix4 CreateLookAt(const Vector3f & position, const Vector3f & target, const Vector3f & up);

		// World matrix
		static Matrix4 CreateWorld(const Vector3f & position, const Vector3f & forward, const Vector3f & up);

		// Rotation matrix from quaternion
		static Matrix4 CreateFromQuaternion(const Quaternion& rotation);

		// Rotation matrix from yaw (y axis angle), pitch (x axis angle) and roll (z axis angle)
		static Matrix4 CreateFromYawPitchRoll(f32 yaw, f32 pitch, f32 roll);

		// Planar shadow matrix
		static Matrix4 CreateShadow(const Vector3f & lightDir, const Plane & plane);

		// Reflection matrix
		static Matrix4 CreateReflection(const Plane & plane);

		// Linear interpolation
		static Matrix4 Lerp(const Matrix4 & m1, const Matrix4 & m2, f32 t);

		// Transform matrix
		static Matrix4 Transform(const Matrix4 & m, const Quaternion& rotation);

	};

	// Plus operator
	Matrix4 operator + (const Matrix4 & lhs, const Matrix4 & rhs);

	// Minus operator
	Matrix4 operator - (const Matrix4 & lhs, const Matrix4 & rhs);

	// Multiply by element operator
	Matrix4 operator * (const Matrix4 & lhs, const Matrix4 & rhs); 

	// Mulitply operator
	Matrix4 operator * (f32 lhs, const Matrix4 & rhs);

	// Multiply operator
	Matrix4 operator * (const Matrix4 & lhs, f32 rhs);

	// Transform vector
	Vector3f operator * (const Matrix4 & lhs, const Vector3f & rhs);

	// Transform vector
	Vector4f operator * (const Matrix4 & lhs, const Vector4f & rhs);

	// Divide operator
	Matrix4 operator / (const Matrix4 & lhs, f32 rhs);

	// Divide by element operator
	Matrix4 operator / (const Matrix4 & lhs, const Matrix4 & rhs);
}

#include "Platform/Matrix4.inl"

#endif