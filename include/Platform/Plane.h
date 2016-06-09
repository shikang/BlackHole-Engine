#ifndef BH_PLANE_H
#define BH_PLANE_H

#include "Platform/Typedefs.h"

#include <DirectXMath.h>

namespace BH
{
	// [nx ny nz n.P]
	class Plane : public DirectX::XMFLOAT4
	{
	public:
		// Default Constructor (XZ Plane)
		Plane();

		// Parameterized constructor
		Plane(const Vector3f & normal, f32 d);

		// Parameterized constructor (3 points that lies on the plane
		Plane(const Vector3f & point1, const Vector3f & point2, const Vector3f & point3);

		// Parameterized constructor
		Plane(const Vector3f & point, const Vector3f & normal);

		// Copy Constructor
		Plane(const Plane & p);

	public:
		// Get normal of the plance
		Vector3f Normal() const { return Vector3f(x, y, z); }

		// Set the normal of the plane
		void SetNormal(const Vector3f & normal) { x = normal.x; y = normal.y; z = normal.z; }

		// Get shortest distance of plane from origin
		f32 D() const { return w; }

		// Set shortest distance of plane from origin
		void SetD(f32 d) { w = d; }

	public:
		// Calculate shortest distance between point and plane
		f32 DotCoordinate(const Vector3f & point) const;

		// Calculate angle between normal and plance normal
		f32 DotNormal(const Vector3f & normal) const;

	public:
		// Transform plane (remember transformation matrix of plane is not just multiplying the matrix, Remember CS250!)
		static Plane Transform(const Plane& plane, const Matrix4 & m);

		// Transform plane (remember transformation matrix of plane is not just multiplying the matrix, Remember CS250!)
		static Plane Transform(const Plane& plane, const Quaternion & rotation);
	};
}

#include "Platform/Plane.inl"

#endif