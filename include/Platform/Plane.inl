namespace BH
{
	inline Plane::Plane() : DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f) {}

	inline Plane::Plane(const Vector3f & normal, f32 d) : DirectX::XMFLOAT4(normal.x, normal.y, normal.z, d) {}

	inline Plane::Plane(const Vector3f & point1, const Vector3f & point2, const Vector3f & point3)
	{
		DirectX::XMVECTOR P0 = DirectX::XMLoadFloat3(&point1);
		DirectX::XMVECTOR P1 = DirectX::XMLoadFloat3(&point2);
		DirectX::XMVECTOR P2 = DirectX::XMLoadFloat3(&point3);
		DirectX::XMStoreFloat4(this, DirectX::XMPlaneFromPoints(P0, P1, P2));
	}

	inline Plane::Plane(const Vector3f & point, const Vector3f & normal)
	{
		DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&point);
		DirectX::XMVECTOR N = DirectX::XMLoadFloat3(&normal);
		DirectX::XMStoreFloat4(this, DirectX::XMPlaneFromPointNormal(P, N));
	}

	inline Plane::Plane(const Plane & p) : DirectX::XMFLOAT4(p.x, p.y, p.z, p.w) {}

	inline f32 Plane::DotCoordinate(const Vector3f & point) const
	{
		DirectX::XMVECTOR p = DirectX::XMLoadFloat4(this);
		DirectX::XMVECTOR v0 = DirectX::XMLoadFloat3(&point);
		return DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(p, v0));
	}

	inline f32 Plane::DotNormal(const Vector3f & normal) const
	{
		DirectX::XMVECTOR p = DirectX::XMLoadFloat4(this);
		DirectX::XMVECTOR n0 = DirectX::XMLoadFloat3(&normal);
		return DirectX::XMVectorGetX(DirectX::XMPlaneDotNormal(p, n0));
	}

	inline Plane Plane::Transform(const Plane& plane, const Quaternion & rotation)
	{
		DirectX::XMVECTOR p = DirectX::XMLoadFloat4(&plane);
		DirectX::XMVECTOR q = DirectX::XMLoadFloat4(&rotation);
		DirectX::XMVECTOR X = DirectX::XMVector3Rotate(p, q);
		X = DirectX::XMVectorSelect(p, X, DirectX::g_XMSelect1110); // result.d = plane.d

		Plane result;
		DirectX::XMStoreFloat4(&result, X);
		return result;
	}
}