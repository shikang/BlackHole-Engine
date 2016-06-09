#include <cassert>

namespace BH
{
	inline Vector3f::Vector3f() : XMFLOAT3(0.0f, 0.0f, 0.0f) {}

	inline Vector3f::Vector3f(f32 value) : XMFLOAT3(value, value, value) {}

	inline Vector3f::Vector3f(f32 X, f32 Y, f32 Z) : XMFLOAT3(X, Y, Z) {}

	inline Vector3f::Vector3f(const Vector3f & v) : XMFLOAT3(v.x, v.y, v.z) {}

	inline Vector3f & Vector3f::operator = (const Vector3f &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}

	inline Vector3f & Vector3f::operator += (const Vector3f &rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&rhs);
		DirectX::XMVECTOR X = DirectX::XMVectorAdd(v1, v2);
		DirectX::XMStoreFloat3(this, X);
		return *this;
	}

	inline Vector3f & Vector3f::operator -= (const Vector3f &rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&rhs);
		DirectX::XMVECTOR X = DirectX::XMVectorSubtract(v1, v2);
		DirectX::XMStoreFloat3(this, X);
		return *this;
	}

	inline Vector3f & Vector3f::operator *= (f32 rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(this);
		DirectX::XMVECTOR X = DirectX::XMVectorScale(v1, rhs);
		DirectX::XMStoreFloat3(this, X);
		return *this;
	}

	inline Vector3f & Vector3f::operator *= (const Vector3f &rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&rhs);
		DirectX::XMVECTOR X = DirectX::XMVectorMultiply(v1, v2);
		DirectX::XMStoreFloat3(this, X);
		return *this;
	}

	inline Vector3f & Vector3f::operator /= (f32 rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(this);
		DirectX::XMVECTOR X = DirectX::XMVectorScale(v1, 1.0f / rhs);
		DirectX::XMStoreFloat3(this, X);
		return *this;
	}

	inline Vector3f Vector3f::operator - () const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(this);
		DirectX::XMVECTOR X = DirectX::XMVectorNegate(v1);
		Vector3f R;
		DirectX::XMStoreFloat3(&R, X);
		return R;
	}

	inline f32 Vector3f::operator [] ( u32 index ) const
	{
		assert( index < 3 );

		return ( index == 0 ) ? x : ( ( index == 1 ) ? y : z );
	}

	inline bool Vector3f::operator == (const Vector3f &rhs) const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&rhs);
		return DirectX::XMVector3Equal(v1, v2);
	}

	inline bool Vector3f::operator != (const Vector3f &rhs) const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&rhs);
		return DirectX::XMVector3NotEqual(v1, v2);
	}

	inline bool Vector3f::InBounds(const Vector3f & bounds) const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&bounds);
		return DirectX::XMVector3InBounds(v1, v2);
	}

	inline void Vector3f::Normalize()
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(this);
		DirectX::XMVECTOR X = DirectX::XMVector3Normalize(v1);
		DirectX::XMStoreFloat3(this, X);
	}

	inline f32 Vector3f::Length() const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(this);
		DirectX::XMVECTOR X = DirectX::XMVector3Length(v1);
		return DirectX::XMVectorGetX(X);
	}

	inline f32 Vector3f::LengthSq() const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(this);
		DirectX::XMVECTOR X = DirectX::XMVector3LengthSq(v1);
		return DirectX::XMVectorGetX(X);
	}

	inline f32 Vector3f::Dot(const Vector3f& v) const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&v);
		DirectX::XMVECTOR X = DirectX::XMVector3Dot(v1, v2);
		return DirectX::XMVectorGetX(X);
	}

	inline Vector3f Vector3f::Cross(const Vector3f& v) const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&v);
		DirectX::XMVECTOR R = DirectX::XMVector3Cross(v1, v2);

		Vector3f result;
		XMStoreFloat3(&result, R);
		return result;
	}

	inline void Vector3f::Clamp(const Vector3f & min, const Vector3f & max)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&min);
		DirectX::XMVECTOR v3 = DirectX::XMLoadFloat3(&max);
		DirectX::XMVECTOR X = DirectX::XMVectorClamp(v1, v2, v3);
		DirectX::XMStoreFloat3(this, X);
	}

	inline Vector3f Vector3f::Normalize(const Vector3f & v)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(&v);
		DirectX::XMVECTOR X = DirectX::XMVector3Normalize(v1);
		
		Vector3f result;
		XMStoreFloat3(&result, X);
		return result;
	}

	inline Vector3f Vector3f::Clamp(const Vector3f & v, const Vector3f & min, const Vector3f & max)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(&v);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&min);
		DirectX::XMVECTOR v3 = DirectX::XMLoadFloat3(&max);
		DirectX::XMVECTOR X = DirectX::XMVectorClamp(v1, v2, v3);
		
		Vector3f result;
		XMStoreFloat3(&result, X);
		return result;
	}

	inline f32 Vector3f::Distance(const Vector3f & v1, const Vector3f & v2)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat3(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat3(&v2);
		DirectX::XMVECTOR V = DirectX::XMVectorSubtract(x2, x1);
		DirectX::XMVECTOR X = DirectX::XMVector3Length(V);
		return DirectX::XMVectorGetX(X);
	}

	inline f32 Vector3f::DistanceSquared(const Vector3f & v1, const Vector3f & v2)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat3(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat3(&v2);
		DirectX::XMVECTOR V = DirectX::XMVectorSubtract(x2, x1);
		DirectX::XMVECTOR X = DirectX::XMVector3LengthSq(V);
		return DirectX::XMVectorGetX(X);
	}

	inline Vector3f Vector3f::Min(const Vector3f & v1, const Vector3f & v2)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat3(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat3(&v2);
		DirectX::XMVECTOR X = DirectX::XMVectorMin(x1, x2);

		Vector3f result;
		DirectX::XMStoreFloat3(&result, X);
		return result;
	}

	inline Vector3f Vector3f::Max(const Vector3f & v1, const Vector3f & v2)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat3(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat3(&v2);
		DirectX::XMVECTOR X = DirectX::XMVectorMax(x1, x2);

		Vector3f result;
		DirectX::XMStoreFloat3(&result, X);
		return result;
	}

	inline Vector3f Vector3f::Lerp(const Vector3f & v1, const Vector3f & v2, f32 t)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat3(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat3(&v2);
		DirectX::XMVECTOR X = DirectX::XMVectorLerp(x1, x2, t);

		Vector3f result;
		DirectX::XMStoreFloat3(&result, X);
		return result;
	}

	inline Vector3f Vector3f::SmoothStep(const Vector3f & v1, const Vector3f & v2, f32 t)
	{
		t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
		t = t * t * (3.0f - 2.0f * t);
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat3(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat3(&v2);
		DirectX::XMVECTOR X = DirectX::XMVectorLerp(x1, x2, t);

		Vector3f result;
		DirectX::XMStoreFloat3(&result, X);
		return result;
	}

	inline Vector3f Vector3f::Barycentric(const Vector3f & v1, const Vector3f & v2, const Vector3f & v3, f32 f, f32 g)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat3(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat3(&v2);
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat3(&v3);
		DirectX::XMVECTOR X = DirectX::XMVectorBaryCentric(x1, x2, x3, f, g);

		Vector3f result;
		DirectX::XMStoreFloat3(&result, X);
		return result;
	}

	inline Vector3f Vector3f::CatmullRom(const Vector3f & v1, const Vector3f & v2, const Vector3f & v3, const Vector3f & v4, f32 t)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat3(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat3(&v2);
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat3(&v3);
		DirectX::XMVECTOR x4 = DirectX::XMLoadFloat3(&v4);
		DirectX::XMVECTOR X = DirectX::XMVectorCatmullRom(x1, x2, x3, x4, t);

		Vector3f result;
		DirectX::XMStoreFloat3(&result, X);
		return result;
	}

	inline Vector3f Vector3f::Hermite(const Vector3f & v1, const Vector3f & t1, const Vector3f & v2, const Vector3f & t2, f32 t)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat3(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat3(&t1);
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat3(&v2);
		DirectX::XMVECTOR x4 = DirectX::XMLoadFloat3(&t2);
		DirectX::XMVECTOR X = DirectX::XMVectorHermite(x1, x2, x3, x4, t);

		Vector3f result;
		DirectX::XMStoreFloat3(&result, X);
		return result;
	}

	inline Vector3f Vector3f::Reflect(const Vector3f & ivec, const Vector3f & nvec)
	{
		DirectX::XMVECTOR i = DirectX::XMLoadFloat3(&ivec);
		DirectX::XMVECTOR n = DirectX::XMLoadFloat3(&nvec);
		DirectX::XMVECTOR X = DirectX::XMVector3Reflect(i, n);

		Vector3f result;
		DirectX::XMStoreFloat3(&result, X);
		return result;
	}

	inline Vector3f Vector3f::Refract(const Vector3f & ivec, const Vector3f & nvec, f32 refractionIndex)
	{
		DirectX::XMVECTOR i = DirectX::XMLoadFloat3(&ivec);
		DirectX::XMVECTOR n = DirectX::XMLoadFloat3(&nvec);
		DirectX::XMVECTOR X = DirectX::XMVector3Refract(i, n, refractionIndex);

		Vector3f result;
		DirectX::XMStoreFloat3(&result, X);
		return result;
	}

	inline Vector3f operator + (const Vector3f & lhs, const Vector3f & rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(&lhs);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&rhs);
		DirectX::XMVECTOR X = DirectX::XMVectorAdd(v1, v2);
		Vector3f R;
		DirectX::XMStoreFloat3(&R, X);
		return R;
	}

	inline Vector3f operator - (const Vector3f & lhs, const Vector3f & rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(&lhs);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&rhs);
		DirectX::XMVECTOR X = DirectX::XMVectorSubtract(v1, v2);
		Vector3f R;
		DirectX::XMStoreFloat3(&R, X);
		return R;
	}

	inline Vector3f operator * (const Vector3f & lhs, const Vector3f & rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(&lhs);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat3(&rhs);
		DirectX::XMVECTOR X = DirectX::XMVectorMultiply(v1, v2);
		Vector3f R;
		DirectX::XMStoreFloat3(&R, X);
		return R;
	}

	inline Vector3f operator * (const Vector3f & lhs, f32 rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(&lhs);
		DirectX::XMVECTOR X = DirectX::XMVectorScale(v1, rhs);
		Vector3f R;
		DirectX::XMStoreFloat3(&R, X);
		return R;
	}

	inline Vector3f operator / (const Vector3f & lhs, f32 rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(&lhs);
		DirectX::XMVECTOR X = DirectX::XMVectorScale(v1, 1.0f / rhs);
		Vector3f R;
		DirectX::XMStoreFloat3(&R, X);
		return R;
	}

	inline Vector3f operator * (f32 lhs, const Vector3f & rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat3(&rhs);
		DirectX::XMVECTOR X = DirectX::XMVectorScale(v1, lhs);
		Vector3f R;
		DirectX::XMStoreFloat3(&R, X);
		return R;
	}
}