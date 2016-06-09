namespace BH
{
	inline Vector4f::Vector4f() : XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) {}

	inline Vector4f::Vector4f(f32 value) : XMFLOAT4(value, value, value, value) {}

	inline Vector4f::Vector4f(f32 X, f32 Y, f32 Z, f32 W) : XMFLOAT4(X, Y, Z, W) {}

	inline Vector4f::Vector4f(const Vector4f & v) : XMFLOAT4(v.x, v.y, v.z, v.w) {}

	inline Vector4f & Vector4f::operator = (const Vector4f &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
		return *this;
	}

	inline Vector4f & Vector4f::operator += (const Vector4f &rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat4(&rhs);
		DirectX::XMVECTOR X = DirectX::XMVectorAdd(v1, v2);
		DirectX::XMStoreFloat4(this, X);
		return *this;
	}

	inline Vector4f & Vector4f::operator -= (const Vector4f &rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat4(&rhs);
		DirectX::XMVECTOR X = DirectX::XMVectorSubtract(v1, v2);
		DirectX::XMStoreFloat4(this, X);
		return *this;
	}

	inline Vector4f & Vector4f::operator *= (f32 rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(this);
		DirectX::XMVECTOR X = DirectX::XMVectorScale(v1, rhs);
		DirectX::XMStoreFloat4(this, X);
		return *this;
	}

	inline Vector4f & Vector4f::operator *= (const Vector4f &rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat4(&rhs);
		DirectX::XMVECTOR X = DirectX::XMVectorMultiply(v1, v2);
		DirectX::XMStoreFloat4(this, X);
		return *this;
	}

	inline Vector4f & Vector4f::operator /= (f32 rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(this);
		DirectX::XMVECTOR X = DirectX::XMVectorScale(v1, 1.0f / rhs);
		DirectX::XMStoreFloat4(this, X);
		return *this;
	}

	inline Vector4f Vector4f::operator - () const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(this);
		DirectX::XMVECTOR X = DirectX::XMVectorNegate(v1);
		Vector4f R;
		DirectX::XMStoreFloat4(&R, X);
		return R;
	}

	inline bool Vector4f::operator == (const Vector4f &rhs) const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat4(&rhs);
		return DirectX::XMVector4Equal(v1, v2);
	}

	inline bool Vector4f::operator != (const Vector4f &rhs) const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat4(&rhs);
		return DirectX::XMVector4NotEqual(v1, v2);
	}

	inline bool Vector4f::InBounds(const Vector4f & bounds) const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat4(&bounds);
		return DirectX::XMVector4InBounds(v1, v2);
	}

	inline void Vector4f::Normalize()
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(this);
		DirectX::XMVECTOR X = DirectX::XMVector4Normalize(v1);
		DirectX::XMStoreFloat4(this, X);
	}

	inline f32 Vector4f::Length() const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(this);
		DirectX::XMVECTOR X = DirectX::XMVector4Length(v1);
		return DirectX::XMVectorGetX(X);
	}

	inline f32 Vector4f::LengthSq() const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(this);
		DirectX::XMVECTOR X = DirectX::XMVector4LengthSq(v1);
		return DirectX::XMVectorGetX(X);
	}

	inline f32 Vector4f::Dot(const Vector4f& v) const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat4(&v);
		DirectX::XMVECTOR X = DirectX::XMVector4Dot(v1, v2);
		return DirectX::XMVectorGetX(X);
	}

	inline void Vector4f::Clamp(const Vector4f & min, const Vector4f & max)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat4(&min);
		DirectX::XMVECTOR v3 = DirectX::XMLoadFloat4(&max);
		DirectX::XMVECTOR X = DirectX::XMVectorClamp(v1, v2, v3);
		DirectX::XMStoreFloat4(this, X);
	}

	inline Vector4f Vector4f::Normalize(const Vector4f & v)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(&v);
		DirectX::XMVECTOR X = DirectX::XMVector4Normalize(v1);

		Vector4f result;
		DirectX::XMStoreFloat4(&result, X);
		return result;
	}

	inline Vector4f Vector4f::Clamp(const Vector4f & v, const Vector4f & min, const Vector4f & max)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(&v);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat4(&min);
		DirectX::XMVECTOR v3 = DirectX::XMLoadFloat4(&max);
		DirectX::XMVECTOR X = DirectX::XMVectorClamp(v1, v2, v3);
		
		Vector4f result;
		DirectX::XMStoreFloat4(&result, X);
		return result;
	}

	inline f32 Vector4f::Distance(const Vector4f & v1, const Vector4f & v2)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4(&v2);
		DirectX::XMVECTOR V = DirectX::XMVectorSubtract(x2, x1);
		DirectX::XMVECTOR X = DirectX::XMVector4Length(V);
		return DirectX::XMVectorGetX(X);
	}

	inline f32 Vector4f::DistanceSquared(const Vector4f & v1, const Vector4f & v2)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4(&v2);
		DirectX::XMVECTOR V = DirectX::XMVectorSubtract(x2, x1);
		DirectX::XMVECTOR X = DirectX::XMVector4LengthSq(V);
		return DirectX::XMVectorGetX(X);
	}

	inline Vector4f Vector4f::Min(const Vector4f & v1, const Vector4f & v2)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4(&v2);
		DirectX::XMVECTOR X = DirectX::XMVectorMin(x1, x2);

		Vector4f result;
		DirectX::XMStoreFloat4(&result, X);
		return result;
	}

	inline Vector4f Vector4f::Max(const Vector4f & v1, const Vector4f & v2)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4(&v2);
		DirectX::XMVECTOR X = DirectX::XMVectorMax(x1, x2);

		Vector4f result;
		DirectX::XMStoreFloat4(&result, X);
		return result;
	}

	inline Vector4f Vector4f::Lerp(const Vector4f & v1, const Vector4f & v2, f32 t)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4(&v2);
		DirectX::XMVECTOR X = DirectX::XMVectorLerp(x1, x2, t);

		Vector4f result;
		DirectX::XMStoreFloat4(&result, X);
		return result;
	}

	inline Vector4f Vector4f::SmoothStep(const Vector4f & v1, const Vector4f & v2, f32 t)
	{
		t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
		t = t * t * (3.0f - 2.0f * t);
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4(&v2);
		DirectX::XMVECTOR X = DirectX::XMVectorLerp(x1, x2, t);

		Vector4f result;
		DirectX::XMStoreFloat4(&result, X);
		return result;
	}

	inline Vector4f Vector4f::Barycentric(const Vector4f & v1, const Vector4f & v2, const Vector4f & v3, f32 f, f32 g)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4(&v2);
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat4(&v3);
		DirectX::XMVECTOR X = DirectX::XMVectorBaryCentric(x1, x2, x3, f, g);

		Vector4f result;
		DirectX::XMStoreFloat4(&result, X);
		return result;
	}

	inline Vector4f Vector4f::CatmullRom(const Vector4f & v1, const Vector4f & v2, const Vector4f & v3, const Vector4f & v4, f32 t)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4(&v2);
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat4(&v3);
		DirectX::XMVECTOR x4 = DirectX::XMLoadFloat4(&v4);
		DirectX::XMVECTOR X = DirectX::XMVectorCatmullRom(x1, x2, x3, x4, t);

		Vector4f result;
		DirectX::XMStoreFloat4(&result, X);
		return result;
	}

	inline Vector4f Vector4f::Hermite(const Vector4f & v1, const Vector4f & t1, const Vector4f & v2, const Vector4f & t2, f32 t)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4(&t1);
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat4(&v2);
		DirectX::XMVECTOR x4 = DirectX::XMLoadFloat4(&t2);
		DirectX::XMVECTOR X = DirectX::XMVectorHermite(x1, x2, x3, x4, t);

		Vector4f result;
		DirectX::XMStoreFloat4(&result, X);
		return result;
	}

	inline Vector4f Vector4f::Reflect(const Vector4f & ivec, const Vector4f & nvec)
	{
		DirectX::XMVECTOR i = DirectX::XMLoadFloat4(&ivec);
		DirectX::XMVECTOR n = DirectX::XMLoadFloat4(&nvec);
		DirectX::XMVECTOR X = DirectX::XMVector4Reflect(i, n);

		Vector4f result;
		DirectX::XMStoreFloat4(&result, X);
		return result;
	}

	inline Vector4f Vector4f::Refract(const Vector4f & ivec, const Vector4f & nvec, f32 refractionIndex)
	{
		DirectX::XMVECTOR i = DirectX::XMLoadFloat4(&ivec);
		DirectX::XMVECTOR n = DirectX::XMLoadFloat4(&nvec);
		DirectX::XMVECTOR X = DirectX::XMVector4Refract(i, n, refractionIndex);

		Vector4f result;
		DirectX::XMStoreFloat4(&result, X);
		return result;
	}

	inline Vector4f operator + (const Vector4f & lhs, const Vector4f & rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(&lhs);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat4(&rhs);
		DirectX::XMVECTOR X = DirectX::XMVectorAdd(v1, v2);
		Vector4f R;
		DirectX::XMStoreFloat4(&R, X);
		return R;
	}

	inline Vector4f operator - (const Vector4f & lhs, const Vector4f & rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(&lhs);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat4(&rhs);
		DirectX::XMVECTOR X = DirectX::XMVectorSubtract(v1, v2);
		Vector4f R;
		DirectX::XMStoreFloat4(&R, X);
		return R;
	}

	inline Vector4f operator * (const Vector4f & lhs, const Vector4f & rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(&lhs);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat4(&rhs);
		DirectX::XMVECTOR X = DirectX::XMVectorMultiply(v1, v2);
		Vector4f R;
		DirectX::XMStoreFloat4(&R, X);
		return R;
	}

	inline Vector4f operator * (const Vector4f & lhs, f32 rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(&lhs);
		DirectX::XMVECTOR X = DirectX::XMVectorScale(v1, rhs);
		Vector4f R;
		DirectX::XMStoreFloat4(&R, X);
		return R;
	}

	inline Vector4f operator / (const Vector4f & lhs, f32 rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(&lhs);
		DirectX::XMVECTOR X = DirectX::XMVectorScale(v1, 1.0f / rhs);
		Vector4f R;
		DirectX::XMStoreFloat4(&R, X);
		return R;
	}

	inline Vector4f operator * (f32 lhs, const Vector4f & rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(&rhs);
		DirectX::XMVECTOR X = DirectX::XMVectorScale(v1, lhs);
		Vector4f R;
		DirectX::XMStoreFloat4(&R, X);
		return R;
	}
}