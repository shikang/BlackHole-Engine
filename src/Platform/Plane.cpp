#include "Platform/Stdafx.h"

#include "Platform/Plane.h"
#include "Platform/Matrix4.h"

namespace BH
{
	Plane Plane::Transform(const Plane& plane, const Matrix4 & m)
	{
		DirectX::XMVECTOR p = DirectX::XMLoadFloat4(&plane);
		DirectX::XMMATRIX m0 = DirectX::XMLoadFloat4x4(&m);

		Plane result;
		DirectX::XMStoreFloat4(&result, DirectX::XMPlaneTransform(p, m0));
		return result;
	}
}