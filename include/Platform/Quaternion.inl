namespace BH
{
	inline Quaternion::Quaternion() : XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) {}

	inline Quaternion::Quaternion( f32 X, f32 Y, f32 Z, f32 W ) : XMFLOAT4( X, Y, Z, W ) {}

	inline Quaternion::Quaternion( const Vector3f & v, f32 scalar ) : XMFLOAT4( v.x, v.y, v.z, scalar ) {}

	inline Quaternion::Quaternion( const Quaternion & q ) : XMFLOAT4( q.x, q.y, q.z, q.w ) {}

	inline Quaternion & Quaternion::operator = ( const Quaternion & rhs )
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;
		return *this;
	}

	inline Quaternion & Quaternion::operator += ( const Quaternion & rhs )
	{
		DirectX::XMVECTOR q1 = DirectX::XMLoadFloat4( this );
		DirectX::XMVECTOR q2 = DirectX::XMLoadFloat4( &rhs );
		DirectX::XMVECTOR X = DirectX::XMVectorAdd( q1, q2 );
		DirectX::XMStoreFloat4( this, X );
		return *this;
	}

	inline Quaternion & Quaternion::operator -= ( const Quaternion & rhs )
	{
		DirectX::XMVECTOR q1 = DirectX::XMLoadFloat4( this );
		DirectX::XMVECTOR q2 = DirectX::XMLoadFloat4( &rhs );
		DirectX::XMVECTOR X = DirectX::XMVectorSubtract( q1, q2 );
		DirectX::XMStoreFloat4( this, X );
		return *this;
	}

	inline Quaternion & Quaternion::operator *= ( f32 rhs )
	{
		DirectX::XMVECTOR q1 = DirectX::XMLoadFloat4( this );
		DirectX::XMVECTOR X = DirectX::XMVectorScale( q1, rhs );
		DirectX::XMStoreFloat4( this, X );
		return *this;
	}

	inline Quaternion & Quaternion::operator *= ( const Quaternion & rhs )
	{
		DirectX::XMVECTOR q1 = DirectX::XMLoadFloat4( this );
		DirectX::XMVECTOR q2 = DirectX::XMLoadFloat4( &rhs );
		DirectX::XMVECTOR X = DirectX::XMQuaternionMultiply( q1, q2 );
		DirectX::XMStoreFloat4( this, X );
		return *this;
	}

	inline Quaternion & Quaternion::operator /= ( const Quaternion & rhs )
	{
		DirectX::XMVECTOR q1 = DirectX::XMLoadFloat4( this );
		DirectX::XMVECTOR q2 = DirectX::XMLoadFloat4( &rhs );
		q2 = DirectX::XMQuaternionInverse( q2 );
		XMStoreFloat4( this, DirectX::XMQuaternionMultiply( q1, q2 ) );
		return *this;
	}

	inline Quaternion Quaternion::operator - () const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4( this );
		DirectX::XMVECTOR X = DirectX::XMVectorNegate( v1 );
		Quaternion R;
		DirectX::XMStoreFloat4( &R, X );
		return R;
	}

	inline bool Quaternion::operator == ( const Quaternion &rhs ) const
	{
		DirectX::XMVECTOR q1 = DirectX::XMLoadFloat4( this );
		DirectX::XMVECTOR q2 = DirectX::XMLoadFloat4( &rhs );
		return DirectX::XMQuaternionEqual( q1, q2 );
	}

	inline bool Quaternion::operator != ( const Quaternion &rhs ) const
	{
		DirectX::XMVECTOR q1 = DirectX::XMLoadFloat4( this );
		DirectX::XMVECTOR q2 = DirectX::XMLoadFloat4( &rhs );
		return DirectX::XMQuaternionNotEqual( q1, q2 );
	}

	inline Quaternion operator + ( const Quaternion & lhs, const Quaternion & rhs )
	{
		DirectX::XMVECTOR q1 = DirectX::XMLoadFloat4( &lhs );
		DirectX::XMVECTOR q2 = DirectX::XMLoadFloat4( &rhs );
		DirectX::XMVECTOR X = DirectX::XMVectorAdd( q1, q2 );
		Quaternion R;
		DirectX::XMStoreFloat4( &R, X );
		return R;
	}

	inline Quaternion operator - ( const Quaternion & lhs, const Quaternion & rhs )
	{
		DirectX::XMVECTOR q1 = DirectX::XMLoadFloat4( &lhs );
		DirectX::XMVECTOR q2 = DirectX::XMLoadFloat4( &rhs );
		DirectX::XMVECTOR X = DirectX::XMVectorSubtract( q1, q2 );
		Quaternion R;
		DirectX::XMStoreFloat4( &R, X );
		return R;
	}

	inline Quaternion operator * ( const Quaternion & lhs, const Quaternion & rhs )
	{
		DirectX::XMVECTOR q1 = DirectX::XMLoadFloat4( &lhs );
		DirectX::XMVECTOR q2 = DirectX::XMLoadFloat4( &rhs );
		DirectX::XMVECTOR X = DirectX::XMQuaternionMultiply( q1, q2 );
		Quaternion R;
		DirectX::XMStoreFloat4( &R, X );
		return R;
	}

	inline Vector3f operator * (const Quaternion & lhs, const Vector3f & rhs)
	{
		Quaternion vec( rhs, 0.0f );
		Quaternion con = lhs;
		con.Conjugate();
		Quaternion result = con * vec * lhs;
		return Vector3f( result.x, result.y, result.z );
	}

	inline Quaternion operator * ( const Quaternion & lhs, f32 rhs )
	{
		DirectX::XMVECTOR q1 = DirectX::XMLoadFloat4( &lhs );
		DirectX::XMVECTOR X = DirectX::XMVectorScale( q1, rhs );
		Quaternion R;
		DirectX::XMStoreFloat4( &R, X );
		return R;
	}

	inline Quaternion operator / ( const Quaternion & lhs, const Quaternion & rhs )
	{
		DirectX::XMVECTOR q1 = DirectX::XMLoadFloat4( &lhs );
		DirectX::XMVECTOR q2 = DirectX::XMLoadFloat4( &rhs );
		q2 = DirectX::XMQuaternionInverse( q2 );

		Quaternion R;
		DirectX::XMStoreFloat4( &R, DirectX::XMQuaternionMultiply( q1, q2 ) );
		return R;
	}

	inline Quaternion operator * ( f32 lhs, const Quaternion & rhs )
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4( &rhs );
		DirectX::XMVECTOR X = DirectX::XMVectorScale( v1, lhs );
		Quaternion R;
		DirectX::XMStoreFloat4( &R, X );
		return R;
	}

	inline f32 Quaternion::Length() const
	{
		DirectX::XMVECTOR q = DirectX::XMLoadFloat4( this );
		return DirectX::XMVectorGetX( DirectX::XMQuaternionLength( q ) );
	}

	inline f32 Quaternion::LengthSq() const
	{
		DirectX::XMVECTOR q = DirectX::XMLoadFloat4( this );
		return DirectX::XMVectorGetX( DirectX::XMQuaternionLengthSq( q ) );
	}

	inline void Quaternion::Normalize()
	{
		DirectX::XMVECTOR q = DirectX::XMLoadFloat4( this );
		DirectX::XMStoreFloat4( this, DirectX::XMQuaternionNormalize( q ) );
	}

	inline void Quaternion::Conjugate()
	{
		DirectX::XMVECTOR q = DirectX::XMLoadFloat4( this );
		DirectX::XMStoreFloat4( this, DirectX::XMQuaternionConjugate( q ) );
	}

	inline void Quaternion::Inverse()
	{
		DirectX::XMVECTOR q = DirectX::XMLoadFloat4( this );
		DirectX::XMStoreFloat4( this, DirectX::XMQuaternionInverse( q ) );
	}

	inline f32 Quaternion::Dot( const Quaternion& q ) const
	{
		DirectX::XMVECTOR q1 = DirectX::XMLoadFloat4( this );
		DirectX::XMVECTOR q2 = DirectX::XMLoadFloat4( &q );
		return DirectX::XMVectorGetX( DirectX::XMQuaternionDot( q1, q2 ) );
	}

	inline Quaternion Quaternion::CreateFromAxisAngle( const Vector3f & axis, f32 angle )
	{
		DirectX::XMVECTOR a = DirectX::XMLoadFloat3( &axis );

		Quaternion R;
		DirectX::XMStoreFloat4( &R, DirectX::XMQuaternionRotationAxis( a, angle ) );
		return R;
	}

	inline Quaternion Quaternion::CreateFromYawPitchRoll( f32 yaw, f32 pitch, f32 roll )
	{
		Quaternion R;
		DirectX::XMStoreFloat4( &R, DirectX::XMQuaternionRotationRollPitchYaw( pitch, yaw, roll ) );
		return R;
	}
}