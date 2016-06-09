namespace BH
{
	inline Vector2f::Vector2f() : XMFLOAT2(0.0f, 0.0f) {}

	inline Vector2f::Vector2f(f32 value) : XMFLOAT2(value, value) {}

	inline Vector2f::Vector2f(f32 X, f32 Y) : XMFLOAT2(X, Y) {}

	inline Vector2f::Vector2f(const Vector2f & v) : XMFLOAT2(v.x, v.y) {}

	inline Vector2f & Vector2f::operator = (const Vector2f &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	inline Vector2f & Vector2f::operator += (const Vector2f &rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat2(&rhs);
		DirectX::XMVECTOR X = DirectX::XMVectorAdd(v1, v2);
		DirectX::XMStoreFloat2(this, X);
		return *this;
	}

	inline Vector2f & Vector2f::operator -= (const Vector2f &rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat2(&rhs);
		DirectX::XMVECTOR X = DirectX::XMVectorSubtract(v1, v2);
		DirectX::XMStoreFloat2(this, X);
		return *this;
	}

	inline Vector2f & Vector2f::operator *= (f32 rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(this);
		DirectX::XMVECTOR X = DirectX::XMVectorScale(v1, rhs);
		DirectX::XMStoreFloat2(this, X);
		return *this;
	}

	inline Vector2f & Vector2f::operator *= (const Vector2f &rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat2(&rhs);
		DirectX::XMVECTOR X = DirectX::XMVectorMultiply(v1, v2);
		DirectX::XMStoreFloat2(this, X);
		return *this;
	}

	inline Vector2f & Vector2f::operator /= (f32 rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(this);
		DirectX::XMVECTOR X = DirectX::XMVectorScale(v1, 1.0f / rhs);
		DirectX::XMStoreFloat2(this, X);
		return *this;
	}

	inline Vector2f Vector2f::operator - () const
	{
		return Vector2f(-x, -y);
	}

	inline bool Vector2f::operator == (const Vector2f &rhs) const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat2(&rhs);
		return DirectX::XMVector2Equal(v1, v2);
	}

	inline bool Vector2f::operator != (const Vector2f &rhs) const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat2(&rhs);
		return DirectX::XMVector2NotEqual(v1, v2);
	}

	inline bool Vector2f::InBounds(const Vector2f & bounds) const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat2(&bounds);
		return DirectX::XMVector2InBounds(v1, v2);
	}

	inline void Vector2f::Normalize()
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(this);
		DirectX::XMVECTOR X = DirectX::XMVector2Normalize(v1);
		DirectX::XMStoreFloat2(this, X);
	}

	inline f32 Vector2f::Length() const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(this);
		DirectX::XMVECTOR X = DirectX::XMVector2Length(v1);
		return DirectX::XMVectorGetX(X);
	}

	inline f32 Vector2f::LengthSq() const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(this);
		DirectX::XMVECTOR X = DirectX::XMVector2LengthSq(v1);
		return DirectX::XMVectorGetX(X);
	}

	inline f32 Vector2f::Dot(const Vector2f& v) const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat2(&v);
		DirectX::XMVECTOR X = DirectX::XMVector2Dot(v1, v2);
		return DirectX::XMVectorGetX(X);
	}

	inline f32 Vector2f::ScalarTripleProduct(const Vector2f& v) const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat2(&v);
		DirectX::XMVECTOR X = DirectX::XMVector2Cross(v1, v2);
		return DirectX::XMVectorGetX(X);
	}

	inline void Vector2f::Clamp(const Vector2f & min, const Vector2f & max)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(this);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat2(&min);
		DirectX::XMVECTOR v3 = DirectX::XMLoadFloat2(&max);
		DirectX::XMVECTOR X = DirectX::XMVectorClamp(v1, v2, v3);
		DirectX::XMStoreFloat2(this, X);
	}

	inline Vector2f Vector2f::Normalize(const Vector2f & v)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(&v);
		DirectX::XMVECTOR X = DirectX::XMVector2Normalize(v1);
		
		Vector2f result;
		DirectX::XMStoreFloat2(&result, X);
		return result;
	}

	inline Vector2f Vector2f::Clamp(const Vector2f & v, const Vector2f & min, const Vector2f & max)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(&v);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat2(&min);
		DirectX::XMVECTOR v3 = DirectX::XMLoadFloat2(&max);
		DirectX::XMVECTOR X = DirectX::XMVectorClamp(v1, v2, v3);
		
		Vector2f result;
		DirectX::XMStoreFloat2(&result, X);
		return result;
	}

	inline f32 Vector2f::Distance(const Vector2f & v1, const Vector2f & v2)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat2(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat2(&v2);
		DirectX::XMVECTOR V = DirectX::XMVectorSubtract(x2, x1);
		DirectX::XMVECTOR X = DirectX::XMVector2Length(V);
		return DirectX::XMVectorGetX(X);
	}

	inline f32 Vector2f::DistanceSquared(const Vector2f & v1, const Vector2f & v2)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat2(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat2(&v2);
		DirectX::XMVECTOR V = DirectX::XMVectorSubtract(x2, x1);
		DirectX::XMVECTOR X = DirectX::XMVector2LengthSq(V);
		return DirectX::XMVectorGetX(X);
	}

	inline Vector2f Vector2f::Min(const Vector2f & v1, const Vector2f & v2)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat2(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat2(&v2);
		DirectX::XMVECTOR X = DirectX::XMVectorMin(x1, x2);

		Vector2f result;
		DirectX::XMStoreFloat2(&result, X);
		return result;
	}

	inline Vector2f Vector2f::Max(const Vector2f & v1, const Vector2f & v2)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat2(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat2(&v2);
		DirectX::XMVECTOR X = DirectX::XMVectorMax(x1, x2);

		Vector2f result;
		DirectX::XMStoreFloat2(&result, X);
		return result;
	}

	inline Vector2f Vector2f::Lerp(const Vector2f & v1, const Vector2f & v2, f32 t)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat2(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat2(&v2);
		DirectX::XMVECTOR X = DirectX::XMVectorLerp(x1, x2, t);

		Vector2f result;
		DirectX::XMStoreFloat2(&result, X);
		return result;
	}

	inline Vector2f Vector2f::SmoothStep(const Vector2f & v1, const Vector2f & v2, f32 t)
	{
		t = (t > 1.0f) ? 1.0f : ( (t < 0.0f) ? 0.0f : t );  // Clamp value to 0 to 1
		t = t * t * (3.0f - 2.0f * t);
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat2(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat2(&v2);
		DirectX::XMVECTOR X = DirectX::XMVectorLerp(x1, x2, t);

		Vector2f result;
		DirectX::XMStoreFloat2(&result, X);
		return result;
	}

	inline Vector2f Vector2f::Barycentric(const Vector2f & v1, const Vector2f & v2, const Vector2f & v3, f32 f, f32 g)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat2(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat2(&v2);
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat2(&v3);
		DirectX::XMVECTOR X = DirectX::XMVectorBaryCentric(x1, x2, x3, f, g);

		Vector2f result;
		DirectX::XMStoreFloat2(&result, X);
		return result;
	}

	inline Vector2f Vector2f::CatmullRom(const Vector2f & v1, const Vector2f & v2, const Vector2f & v3, const Vector2f & v4, f32 t)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat2(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat2(&v2);
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat2(&v3);
		DirectX::XMVECTOR x4 = DirectX::XMLoadFloat2(&v4);
		DirectX::XMVECTOR X = DirectX::XMVectorCatmullRom(x1, x2, x3, x4, t);

		Vector2f result;
		DirectX::XMStoreFloat2(&result, X);
		return result;
	}

	inline Vector2f Vector2f::Hermite(const Vector2f & v1, const Vector2f & t1, const Vector2f & v2, const Vector2f & t2, f32 t)
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat2(&v1);
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat2(&t1);
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat2(&v2);
		DirectX::XMVECTOR x4 = DirectX::XMLoadFloat2(&t2);
		DirectX::XMVECTOR X = DirectX::XMVectorHermite(x1, x2, x3, x4, t);

		Vector2f result;
		DirectX::XMStoreFloat2(&result, X);
		return result;
	}

	inline Vector2f Vector2f::Reflect(const Vector2f & ivec, const Vector2f & nvec)
	{
		DirectX::XMVECTOR i = DirectX::XMLoadFloat2(&ivec);
		DirectX::XMVECTOR n = DirectX::XMLoadFloat2(&nvec);
		DirectX::XMVECTOR X = DirectX::XMVector2Reflect(i, n);

		Vector2f result;
		DirectX::XMStoreFloat2(&result, X);
		return result;
	}

	inline Vector2f Vector2f::Refract(const Vector2f & ivec, const Vector2f & nvec, f32 refractionIndex)
	{
		DirectX::XMVECTOR i = DirectX::XMLoadFloat2(&ivec);
		DirectX::XMVECTOR n = DirectX::XMLoadFloat2(&nvec);
		DirectX::XMVECTOR X = DirectX::XMVector2Refract(i, n, refractionIndex);

		Vector2f result;
		DirectX::XMStoreFloat2(&result, X);
		return result;
	}

	inline Vector2f operator + (const Vector2f & lhs, const Vector2f & rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(&lhs);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat2(&rhs);
		DirectX::XMVECTOR X = DirectX::XMVectorAdd(v1, v2);
		Vector2f R;
		DirectX::XMStoreFloat2(&R, X);
		return R;
	}

	inline Vector2f operator - (const Vector2f & lhs, const Vector2f & rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(&lhs);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat2(&rhs);
		DirectX::XMVECTOR X = DirectX::XMVectorSubtract(v1, v2);
		Vector2f R;
		DirectX::XMStoreFloat2(&R, X);
		return R;
	}

	inline Vector2f operator * (const Vector2f & lhs, const Vector2f & rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(&lhs);
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat2(&rhs);
		DirectX::XMVECTOR X = DirectX::XMVectorMultiply(v1, v2);
		Vector2f R;
		DirectX::XMStoreFloat2(&R, X);
		return R;
	}

	inline Vector2f operator * (const Vector2f & lhs, f32 rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(&lhs);
		DirectX::XMVECTOR X = DirectX::XMVectorScale(v1, rhs);
		Vector2f R;
		DirectX::XMStoreFloat2(&R, X);
		return R;
	}

	inline Vector2f operator / (const Vector2f & lhs, f32 rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(&lhs);
		DirectX::XMVECTOR X = DirectX::XMVectorScale(v1, 1.0f / rhs);
		Vector2f R;
		DirectX::XMStoreFloat2(&R, X);
		return R;
	}

	inline Vector2f operator * (f32 lhs, const Vector2f & rhs)
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat2(&rhs);
		DirectX::XMVECTOR X = DirectX::XMVectorScale(v1, lhs);
		Vector2f R;
		DirectX::XMStoreFloat2(&R, X);
		return R;
	}
}