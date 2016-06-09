namespace BH
{
	inline Matrix4::Matrix4()
	: XMFLOAT4X4( 1.0f, 0.0f, 0.0f, 0.0f,
				  0.0f, 1.0f, 0.0f, 0.0f,
				  0.0f, 0.0f, 1.0f, 0.0f,
				  0.0f, 0.0f, 0.0f, 1.0f )
	{
	}

	inline Matrix4::Matrix4( const Vector3f & x, const Vector3f & y, const Vector3f & z )
	: XMFLOAT4X4( x.x,  x.y,  x.z,  0.0f,
				  y.x,  y.y,  y.z,  0.0f,
				  z.x,  z.y,  z.z,  0.0f,
				  0.0f, 0.0f, 0.0f, 1.0f )
	{
	}

	inline Matrix4::Matrix4( const Vector4f & x, const Vector4f & y, const Vector4f & z, const Vector4f & w )
	: XMFLOAT4X4( x.x, x.y, x.z, x.w,
				  y.x, y.y, y.z, y.w,
				  z.x, z.y, z.z, z.w,
				  w.x, w.y, w.z, w.w )
	{
	}

	inline Matrix4::Matrix4( const Matrix4 & m )
	: XMFLOAT4X4( m.m[0][0], m.m[0][1], m.m[0][2], m.m[0][3],
				  m.m[1][0], m.m[1][1], m.m[1][2], m.m[1][3],
				  m.m[2][0], m.m[2][1], m.m[2][2], m.m[2][3],
				  m.m[3][0], m.m[3][1], m.m[3][2], m.m[3][3] )
	{
	}

	inline bool Matrix4::operator == ( const Matrix4 & rhs ) const
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &_11 ) );
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &_21 ) );
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &_31 ) );
		DirectX::XMVECTOR x4 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &_41 ) );
																								  
		DirectX::XMVECTOR y1 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._11 ) );
		DirectX::XMVECTOR y2 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._21 ) );
		DirectX::XMVECTOR y3 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._31 ) );
		DirectX::XMVECTOR y4 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._41 ) );

		return ( DirectX::XMVector4Equal( x1, y1 ) &&
				 DirectX::XMVector4Equal( x2, y2 ) &&
				 DirectX::XMVector4Equal( x3, y3 ) &&
				 DirectX::XMVector4Equal( x4, y4 ) ) != 0;
	}

	inline bool Matrix4::operator != ( const Matrix4 & rhs ) const
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &_11 ) );
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &_21 ) );
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &_31 ) );
		DirectX::XMVECTOR x4 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &_41 ) );
													  
		DirectX::XMVECTOR y1 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._11 ) );
		DirectX::XMVECTOR y2 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._21 ) );
		DirectX::XMVECTOR y3 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._31 ) );
		DirectX::XMVECTOR y4 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._41 ) );

		return ( DirectX::XMVector4NotEqual( x1, y1 ) ||
				 DirectX::XMVector4NotEqual( x2, y2 ) ||
				 DirectX::XMVector4NotEqual( x3, y3 ) ||
				 DirectX::XMVector4NotEqual( x4, y4 ) ) != 0;
	}

	inline Matrix4 & Matrix4::operator += ( const Matrix4 & rhs )
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_11 ) );
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_21 ) );
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_31 ) );
		DirectX::XMVECTOR x4 = DirectX::XMLoadFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_41 ) );

		DirectX::XMVECTOR y1 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._11 ) );
		DirectX::XMVECTOR y2 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._21 ) );
		DirectX::XMVECTOR y3 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._31 ) );
		DirectX::XMVECTOR y4 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._41 ) );

		x1 = DirectX::XMVectorAdd( x1, y1 );
		x2 = DirectX::XMVectorAdd( x2, y2 );
		x3 = DirectX::XMVectorAdd( x3, y3 );
		x4 = DirectX::XMVectorAdd( x4, y4 );

		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_11 ), x1 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_21 ), x2 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_31 ), x3 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_41 ), x4 );
		return *this;
	}

	inline Matrix4 & Matrix4::operator -= ( const Matrix4 & rhs )
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_11 ) );
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_21 ) );
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_31 ) );
		DirectX::XMVECTOR x4 = DirectX::XMLoadFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_41 ) );
													  
		DirectX::XMVECTOR y1 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._11 ) );
		DirectX::XMVECTOR y2 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._21 ) );
		DirectX::XMVECTOR y3 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._31 ) );
		DirectX::XMVECTOR y4 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._41 ) );

		x1 = DirectX::XMVectorSubtract( x1, y1 );
		x2 = DirectX::XMVectorSubtract( x2, y2 );
		x3 = DirectX::XMVectorSubtract( x3, y3 );
		x4 = DirectX::XMVectorSubtract( x4, y4 );

		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_11 ), x1 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_21 ), x2 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_31 ), x3 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_41 ), x4 );
		return *this;
	}

	inline Matrix4 & Matrix4::operator *= ( const Matrix4 & rhs )
	{
		DirectX::XMMATRIX M1 = DirectX::XMLoadFloat4x4( this );
		DirectX::XMMATRIX M2 = DirectX::XMLoadFloat4x4( &rhs );
		DirectX::XMMATRIX X = DirectX::XMMatrixMultiply( M2, M1 );
		DirectX::XMStoreFloat4x4( this, X );
		return *this;
	}

	inline Matrix4 & Matrix4::operator *= ( f32 rhs )
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_11 ) );
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_21 ) );
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_31 ) );
		DirectX::XMVECTOR x4 = DirectX::XMLoadFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_41 ) );

		x1 = DirectX::XMVectorScale( x1, rhs );
		x2 = DirectX::XMVectorScale( x2, rhs );
		x3 = DirectX::XMVectorScale( x3, rhs );
		x4 = DirectX::XMVectorScale( x4, rhs );

		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_11 ), x1 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_21 ), x2 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_31 ), x3 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_41 ), x4 );
		return *this;
	}

	inline Matrix4 & Matrix4::operator /= ( f32 rhs )
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_11 ) );
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_21 ) );
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_31 ) );
		DirectX::XMVECTOR x4 = DirectX::XMLoadFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_41 ) );

		f32 rs = 1.0f / rhs;

		x1 = DirectX::XMVectorScale( x1, rs );
		x2 = DirectX::XMVectorScale( x2, rs );
		x3 = DirectX::XMVectorScale( x3, rs );
		x4 = DirectX::XMVectorScale( x4, rs );

		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_11 ), x1 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_21 ), x2 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_31 ), x3 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_41 ), x4 );
		return *this;
	}

	inline Matrix4 & Matrix4::operator /= ( const Matrix4 & rhs )
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_11 ) );
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_21 ) );
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_31 ) );
		DirectX::XMVECTOR x4 = DirectX::XMLoadFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_41 ) );
													  
		DirectX::XMVECTOR y1 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._11 ) );
		DirectX::XMVECTOR y2 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._21 ) );
		DirectX::XMVECTOR y3 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._31 ) );
		DirectX::XMVECTOR y4 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._41 ) );

		x1 = DirectX::XMVectorDivide( x1, y1 );
		x2 = DirectX::XMVectorDivide( x2, y2 );
		x3 = DirectX::XMVectorDivide( x3, y3 );
		x4 = DirectX::XMVectorDivide( x4, y4 );

		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_11 ), x1 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_21 ), x2 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_31 ), x3 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &_41 ), x4 );
		return *this;
	}

	inline Matrix4 Matrix4::operator - () const
	{
		DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &_11 ) );
		DirectX::XMVECTOR v2 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &_21 ) );
		DirectX::XMVECTOR v3 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &_31 ) );
		DirectX::XMVECTOR v4 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &_41 ) );

		v1 = DirectX::XMVectorNegate( v1 );
		v2 = DirectX::XMVectorNegate( v2 );
		v3 = DirectX::XMVectorNegate( v3 );
		v4 = DirectX::XMVectorNegate( v4 );

		Matrix4 R;
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._11 ), v1 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._21 ), v2 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._31 ), v3 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._41 ), v4 );
		return R;
	}

	inline Matrix4 operator + ( const Matrix4 & lhs, const Matrix4 & rhs )
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &lhs._11 ) );
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &lhs._21 ) );
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &lhs._31 ) );
		DirectX::XMVECTOR x4 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &lhs._41 ) );
													  											  		   	 
		DirectX::XMVECTOR y1 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._11 ) );
		DirectX::XMVECTOR y2 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._21 ) );
		DirectX::XMVECTOR y3 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._31 ) );
		DirectX::XMVECTOR y4 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._41 ) );

		x1 = DirectX::XMVectorAdd( x1, y1 );
		x2 = DirectX::XMVectorAdd( x2, y2 );
		x3 = DirectX::XMVectorAdd( x3, y3 );
		x4 = DirectX::XMVectorAdd( x4, y4 );

		Matrix4 R;
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._11 ), x1 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._21 ), x2 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._31 ), x3 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._41 ), x4 );
		return R;
	}

	inline Matrix4 operator - ( const Matrix4 & lhs, const Matrix4 & rhs )
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &lhs._11 ) );
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &lhs._21 ) );
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &lhs._31 ) );
		DirectX::XMVECTOR x4 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &lhs._41 ) );
													  											  		   	 
		DirectX::XMVECTOR y1 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._11 ) );
		DirectX::XMVECTOR y2 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._21 ) );
		DirectX::XMVECTOR y3 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._31 ) );
		DirectX::XMVECTOR y4 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._41 ) );

		x1 = DirectX::XMVectorSubtract( x1, y1 );
		x2 = DirectX::XMVectorSubtract( x2, y2 );
		x3 = DirectX::XMVectorSubtract( x3, y3 );
		x4 = DirectX::XMVectorSubtract( x4, y4 );

		Matrix4 R;
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._11 ), x1 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._21 ), x2 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._31 ), x3 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._41 ), x4 );
		return R;
	}

	inline Matrix4 operator * ( const Matrix4 & lhs, const Matrix4 & rhs )
	{
		DirectX::XMMATRIX m1 = DirectX::XMLoadFloat4x4( &lhs );
		DirectX::XMMATRIX m2 = DirectX::XMLoadFloat4x4( &rhs );
		DirectX::XMMATRIX X = DirectX::XMMatrixMultiply( m2, m1 );

		Matrix4 R;
		DirectX::XMStoreFloat4x4( &R, X );
		return R;
	}

	inline Matrix4 operator * ( const Matrix4 & lhs, f32 rhs )
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &lhs._11 ) );
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &lhs._21 ) );
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &lhs._31 ) );
		DirectX::XMVECTOR x4 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &lhs._41 ) );

		x1 = DirectX::XMVectorScale( x1, rhs );
		x2 = DirectX::XMVectorScale( x2, rhs );
		x3 = DirectX::XMVectorScale( x3, rhs );
		x4 = DirectX::XMVectorScale( x4, rhs );

		Matrix4 R;
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._11 ), x1 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._21 ), x2 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._31 ), x3 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._41 ), x4 );
		return R;
	}

	inline Vector3f operator * ( const Matrix4 & lhs, const Vector3f & rhs )
	{
		DirectX::XMMATRIX m = DirectX::XMLoadFloat4x4( &lhs );
		DirectX::XMVECTOR v = DirectX::XMLoadFloat3( &rhs );
		DirectX::XMVECTOR X = DirectX::XMVector3Transform( v, m );

		Vector3f R;
		DirectX::XMStoreFloat3( &R, X );
		return R;
	}

	inline Vector4f operator * ( const Matrix4 & lhs, const Vector4f & rhs )
	{
		DirectX::XMMATRIX m = DirectX::XMLoadFloat4x4( &lhs );
		DirectX::XMVECTOR v = DirectX::XMLoadFloat4( &rhs );
		DirectX::XMVECTOR X = DirectX::XMVector4Transform( v, m );

		Vector4f R;
		DirectX::XMStoreFloat4( &R, X );
		return R;
	}

	inline Matrix4 operator / ( const Matrix4 & lhs, f32 rhs )
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &lhs._11 ) );
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &lhs._21 ) );
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &lhs._31 ) );
		DirectX::XMVECTOR x4 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &lhs._41 ) );

		f32 rs = 1.0f / rhs;

		x1 = DirectX::XMVectorScale( x1, rs );
		x2 = DirectX::XMVectorScale( x2, rs );
		x3 = DirectX::XMVectorScale( x3, rs );
		x4 = DirectX::XMVectorScale( x4, rs );

		Matrix4 R;
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._11 ), x1 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._21 ), x2 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._31 ), x3 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._41 ), x4 );
		return R;
	}

	inline Matrix4 operator / ( const Matrix4 & lhs, const Matrix4 & rhs )
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &lhs._11 ) );
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &lhs._21 ) );
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &lhs._31 ) );
		DirectX::XMVECTOR x4 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &lhs._41 ) );
													  											  		   	 
		DirectX::XMVECTOR y1 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._11 ) );
		DirectX::XMVECTOR y2 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._21 ) );
		DirectX::XMVECTOR y3 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._31 ) );
		DirectX::XMVECTOR y4 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._41 ) );

		x1 = DirectX::XMVectorDivide( x1, y1 );
		x2 = DirectX::XMVectorDivide( x2, y2 );
		x3 = DirectX::XMVectorDivide( x3, y3 );
		x4 = DirectX::XMVectorDivide( x4, y4 );

		Matrix4 R;
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._11 ), x1 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._21 ), x2 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._31 ), x3 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._41 ), x4 );
		return R;
	}

	inline Matrix4 operator * ( f32 lhs, const Matrix4 & rhs )
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._11 ) );
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._21 ) );
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._31 ) );
		DirectX::XMVECTOR x4 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &rhs._41 ) );

		x1 = DirectX::XMVectorScale( x1, lhs );
		x2 = DirectX::XMVectorScale( x2, lhs );
		x3 = DirectX::XMVectorScale( x3, lhs );
		x4 = DirectX::XMVectorScale( x4, lhs );

		Matrix4 R;
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._11 ), x1 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._21 ), x2 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._31 ), x3 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &R._41 ), x4 );
		return R;
	}

	inline Matrix4 Matrix4::Transpose() const
	{
		DirectX::XMMATRIX m = DirectX::XMLoadFloat4x4( this );
		Matrix4 R;
		DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixTranspose( m ) );
		return R;
	}

	inline Matrix4 Matrix4::Inverse() const
	{
		DirectX::XMMATRIX m = DirectX::XMLoadFloat4x4( this );
		Matrix4 R;
		DirectX::XMVECTOR det;
		DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixInverse( &det, m ) );
		return R;
	}

	inline f32 Matrix4::Determinant() const
	{
		DirectX::XMMATRIX rhs = DirectX::XMLoadFloat4x4( this );
		return DirectX::XMVectorGetX( DirectX::XMMatrixDeterminant( rhs ) );
	}

	inline Matrix4 Matrix4::CreateTranslation( const Vector3f & position )
	{
		Matrix4 R;
		DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixTranslation( position.x, position.y, position.z ) );
		return R;
	}

	inline Matrix4 Matrix4::CreateTranslation( f32 x, f32 y, f32 z )
	{
		Matrix4 R;
		DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixTranslation( x, y, z ) );
		return R;
	}

	inline Matrix4 Matrix4::CreateScale( const Vector3f & scales )
	{
		Matrix4 R;
		DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixScaling( scales.x, scales.y, scales.z ) );
		return R;
	}

	inline Matrix4 Matrix4::CreateScale( f32 xs, f32 ys, f32 zs )
	{
		Matrix4 R;
		DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixScaling( xs, ys, zs ) );
		return R;
	}

	inline Matrix4 Matrix4::CreateScale( f32 scale )
	{
		Matrix4 R;
		DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixScaling( scale, scale, scale ) );
		return R;
	}

	inline Matrix4 Matrix4::CreateRotationX( f32 radians )
	{
		Matrix4 R;
		DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixRotationX( radians ) );
		return R;
	}

	inline Matrix4 Matrix4::CreateRotationY( f32 radians )
	{
		Matrix4 R;
		DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixRotationY( radians ) );
		return R;
	}

	inline Matrix4 Matrix4::CreateRotationZ( f32 radians )
	{
		Matrix4 R;
		DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixRotationZ( radians ) );
		return R;
	}

	inline Matrix4 Matrix4::CreateFromAxisAngle( const Vector3f & axis, f32 angle )
	{
		Matrix4 R;
		DirectX::XMVECTOR a = DirectX::XMLoadFloat3( &axis );
		DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixRotationAxis( a, angle ) );
		return R;
	}

	inline Matrix4 Matrix4::CreatePerspectiveFieldOfView( f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane )
	{
		Matrix4 R;
		//DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixPerspectiveFovRH( fov, aspectRatio, nearPlane, farPlane ) );
		DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixPerspectiveFovLH( fov, aspectRatio, nearPlane, farPlane ) );
		return R;
	}

	inline Matrix4 Matrix4::CreatePerspective( f32 width, f32 height, f32 nearPlane, f32 farPlane )
	{
		Matrix4 R;
		//DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixPerspectiveRH( width, height, nearPlane, farPlane ) );
		DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixPerspectiveLH( width, height, nearPlane, farPlane ) );
		return R;
	}

	inline Matrix4 Matrix4::CreatePerspectiveOffCenter( f32 left, f32 right, f32 bottom, f32 top, f32 nearPlane, f32 farPlane )
	{
		Matrix4 R;
		//DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixPerspectiveOffCenterRH( left, right, bottom, top, nearPlane, farPlane ) );
		DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixPerspectiveOffCenterLH( left, right, bottom, top, nearPlane, farPlane ) );
		return R;
	}

	inline Matrix4 Matrix4::CreateOrthographic( f32 width, f32 height, f32 zNearPlane, f32 zFarPlane )
	{
		Matrix4 R;
		//DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixOrthographicRH( width, height, zNearPlane, zFarPlane ) );
		DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixOrthographicLH( width, height, zNearPlane, zFarPlane ) );
		return R;
	}

	inline Matrix4 Matrix4::CreateOrthographicOffCenter( f32 left, f32 right, f32 bottom, f32 top, f32 zNearPlane, f32 zFarPlane )
	{
		Matrix4 R;
		//DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixOrthographicOffCenterRH( left, right, bottom, top, zNearPlane, zFarPlane ) );
		DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixOrthographicOffCenterLH( left, right, bottom, top, zNearPlane, zFarPlane ) );
		return R;
	}

	inline Matrix4 Matrix4::CreateFromYawPitchRoll( f32 yaw, f32 pitch, f32 roll )
	{
		Matrix4 R;
		DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixRotationRollPitchYaw( pitch, yaw, roll ) );
		return R;
	}

}