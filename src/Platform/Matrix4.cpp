#include "Platform/Stdafx.h"

#include "Platform/Matrix4.h"

namespace BH
{
	const Matrix4 Matrix4::IDENTITY = Matrix4( 1.0f, 0.0f, 0.0f, 0.0f,
											   0.0f, 1.0f, 0.0f, 0.0f,
											   0.0f, 0.0f, 1.0f, 0.0f,
											   0.0f, 0.0f, 0.0f, 1.0f );

	Matrix4::Matrix4( f32 m00, f32 m01, f32 m02, f32 m03,
					  f32 m10, f32 m11, f32 m12, f32 m13,
					  f32 m20, f32 m21, f32 m22, f32 m23,
					  f32 m30, f32 m31, f32 m32, f32 m33 )
	: XMFLOAT4X4( m00, m01, m02, m03,
				  m10, m11, m12, m13,
				  m20, m21, m22, m23,
				  m30, m31, m32, m33 )
	{
	}

	Matrix4 Matrix4::CreateReflection( const Plane & plane )
	{
		Matrix4 R;
		DirectX::XMVECTOR planev = DirectX::XMLoadFloat4( &plane );
		DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixReflect( planev ) );
		return R;
	}

	Matrix4 Matrix4::CreateShadow( const Vector3f & lightDir, const Plane & plane )
	{
		Matrix4 R;
		DirectX::XMVECTOR light = DirectX::XMLoadFloat3( &lightDir );
		DirectX::XMVECTOR planev = DirectX::XMLoadFloat4( &plane );
		DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixShadow( planev, light ) );
		return R;
	}

	Matrix4 Matrix4::Transform( const Matrix4 & m, const Quaternion& rotation )
	{
		DirectX::XMVECTOR quatv = DirectX::XMLoadFloat4( &rotation );

		DirectX::XMMATRIX M0 = DirectX::XMLoadFloat4x4( &m );
		DirectX::XMMATRIX M1 = DirectX::XMMatrixRotationQuaternion( quatv );

		Matrix4 result;
		DirectX::XMStoreFloat4x4( &result, DirectX::XMMatrixMultiply( M1, M0 ) );
		return result;
	}

	Matrix4 Matrix4::CreateFromQuaternion( const Quaternion& rotation )
	{
		Matrix4 R;
		DirectX::XMVECTOR quatv = DirectX::XMLoadFloat4( &rotation );
		DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixRotationQuaternion( quatv ) );
		return R;
	}

	bool Matrix4::Decompose( Vector3f & scale, Quaternion & rotation, Vector3f & translation )
	{
		DirectX::XMVECTOR s, r, t;

		DirectX::XMMATRIX m = DirectX::XMLoadFloat4x4( this );
		if ( !DirectX::XMMatrixDecompose( &s, &r, &t, m ) )
			return false;

		DirectX::XMStoreFloat3( &scale, s );
		DirectX::XMStoreFloat4( &rotation, r );
		DirectX::XMStoreFloat3( &translation, t );

		return true;
	}

	Matrix4 Matrix4::CreateBillboard( const Vector3f & object, const Vector3f & cameraPosition, const Vector3f & cameraUp, const Vector3f & cameraView )
	{
		DirectX::XMVECTOR O = DirectX::XMLoadFloat3( &object );
		DirectX::XMVECTOR C = DirectX::XMLoadFloat3( &cameraPosition );
		DirectX::XMVECTOR Z = DirectX::XMVectorSubtract( O, C );

		DirectX::XMVECTOR N = DirectX::XMVector3LengthSq( Z );
		// Distance between object is camera close to zero
		if ( DirectX::XMVector3Less( N, DirectX::g_XMEpsilon ) )
		{
			// Z = -view
			DirectX::XMVECTOR F = DirectX::XMLoadFloat3( &cameraView );
			Z = DirectX::XMVectorNegate( F );
		}
		else
		{
			// Z = vector from camera to object
			Z = DirectX::XMVector3Normalize( Z );
		}

		DirectX::XMVECTOR up = DirectX::XMLoadFloat3( &cameraUp );
		DirectX::XMVECTOR X = DirectX::XMVector3Cross( up, Z );
		X = DirectX::XMVector3Normalize( X );

		DirectX::XMVECTOR Y = DirectX::XMVector3Cross( Z, X );

		DirectX::XMMATRIX M;
		M.r[0] = X;
		M.r[1] = Y;
		M.r[2] = Z;
		M.r[3] = DirectX::XMVectorSetW( O, 1.f );

		Matrix4 R;
		DirectX::XMStoreFloat4x4( &R, M );
		return R;
	}

	Matrix4 Matrix4::CreateConstrainedBillboard( const Vector3f & object, const Vector3f & cameraPosition, const Vector3f & rotateAxis,
												 const Vector3f & cameraView, const Vector3f & objectForward )
	{
		static const DirectX::XMVECTORF32 s_minAngle = { 0.99825467075f, 0.99825467075f, 0.99825467075f, 0.99825467075f }; // 1.0 - XMConvertToRadians( 0.1f );

		DirectX::XMVECTOR O = DirectX::XMLoadFloat3( &object );
		DirectX::XMVECTOR C = DirectX::XMLoadFloat3( &cameraPosition );
		DirectX::XMVECTOR faceDir = DirectX::XMVectorSubtract( O, C );

		DirectX::XMVECTOR N = DirectX::XMVector3LengthSq( faceDir );
		if ( DirectX::XMVector3Less( N, DirectX::g_XMEpsilon ) )
		{
			DirectX::XMVECTOR F = DirectX::XMLoadFloat3( &cameraView );
			faceDir = DirectX::XMVectorNegate( F );
		}
		else
		{
			faceDir = DirectX::XMVector3Normalize( faceDir );
		}

		DirectX::XMVECTOR Y = DirectX::XMLoadFloat3( &rotateAxis );
		DirectX::XMVECTOR X, Z;

		DirectX::XMVECTOR dot = DirectX::XMVectorAbs( DirectX::XMVector3Dot( Y, faceDir ) );
		if ( DirectX::XMVector3Greater( dot, s_minAngle ) )
		{
			Z = DirectX::XMLoadFloat3( &objectForward );
			dot = DirectX::XMVectorAbs( DirectX::XMVector3Dot( Y, Z ) );

			if ( DirectX::XMVector3Greater( dot, s_minAngle ) )
			{
				dot = DirectX::XMVectorAbs( XMVector3Dot( Y, DirectX::g_XMNegIdentityR2 ) );
				Z = ( DirectX::XMVector3Greater( dot, s_minAngle ) ) ? DirectX::g_XMIdentityR0 : DirectX::g_XMNegIdentityR2;
			}

			X = DirectX::XMVector3Cross( Y, Z );
			X = DirectX::XMVector3Normalize( X );

			Z = DirectX::XMVector3Cross( X, Y );
			Z = DirectX::XMVector3Normalize( Z );
		}
		else
		{
			X = DirectX::XMVector3Cross( Y, faceDir );
			X = DirectX::XMVector3Normalize( X );

			Z = DirectX::XMVector3Cross( X, Y );
			Z = DirectX::XMVector3Normalize( Z );
		}

		DirectX::XMMATRIX M;
		M.r[0] = X;
		M.r[1] = Y;
		M.r[2] = Z;
		M.r[3] = DirectX::XMVectorSetW( O, 1.f );

		Matrix4 R;
		DirectX::XMStoreFloat4x4( &R, M );
		return R;
	}

	Matrix4 Matrix4::CreateLookAt( const Vector3f & position, const Vector3f & target, const Vector3f & up )
	{
		Matrix4 R;
		DirectX::XMVECTOR eyev = DirectX::XMLoadFloat3( &position );
		DirectX::XMVECTOR targetv = DirectX::XMLoadFloat3( &target );
		DirectX::XMVECTOR upv = DirectX::XMLoadFloat3( &up );
		//DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixLookAtRH( eyev, targetv, upv ) );
		DirectX::XMStoreFloat4x4( &R, DirectX::XMMatrixLookAtLH( eyev, targetv, upv ) );
		return R;
	}

	Matrix4 Matrix4::CreateWorld( const Vector3f & position, const Vector3f & forward, const Vector3f & up )
	{
		DirectX::XMVECTOR zaxis = DirectX::XMVector3Normalize( DirectX::XMVectorNegate( DirectX::XMLoadFloat3( &forward ) ) );
		DirectX::XMVECTOR yaxis = DirectX::XMLoadFloat3( &up );
		DirectX::XMVECTOR xaxis = DirectX::XMVector3Normalize( DirectX::XMVector3Cross( yaxis, zaxis ) );
		yaxis = DirectX::XMVector3Cross( zaxis, xaxis );

		Matrix4 R;
		DirectX::XMStoreFloat3( reinterpret_cast<DirectX::XMFLOAT3*>( &R._11 ), xaxis );
		DirectX::XMStoreFloat3( reinterpret_cast<DirectX::XMFLOAT3*>( &R._21 ), yaxis );
		DirectX::XMStoreFloat3( reinterpret_cast<DirectX::XMFLOAT3*>( &R._31 ), zaxis );
		R._14 = R._24 = R._34 = 0.0f;
		R._41 = position.x; R._42 = position.y; R._43 = position.z;
		R._44 = 1.f;
		return R;
	}

	Matrix4 Matrix4::Lerp( const Matrix4 & m1, const Matrix4 & m2, f32 t )
	{
		DirectX::XMVECTOR x1 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &m1._11 ) );
		DirectX::XMVECTOR x2 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &m1._21 ) );
		DirectX::XMVECTOR x3 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &m1._31 ) );
		DirectX::XMVECTOR x4 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &m1._41 ) );

		DirectX::XMVECTOR y1 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &m2._11 ) );
		DirectX::XMVECTOR y2 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &m2._21 ) );
		DirectX::XMVECTOR y3 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &m2._31 ) );
		DirectX::XMVECTOR y4 = DirectX::XMLoadFloat4( reinterpret_cast<const DirectX::XMFLOAT4*>( &m2._41 ) );

		x1 = DirectX::XMVectorLerp( x1, y1, t );
		x2 = DirectX::XMVectorLerp( x2, y2, t );
		x3 = DirectX::XMVectorLerp( x3, y3, t );
		x4 = DirectX::XMVectorLerp( x4, y4, t );

		Matrix4 result;
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &result._11 ), x1 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &result._21 ), x2 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &result._31 ), x3 );
		DirectX::XMStoreFloat4( reinterpret_cast<DirectX::XMFLOAT4*>( &result._41 ), x4 );
		return result;
	}
}