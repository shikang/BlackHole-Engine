#include "Platform/Stdafx.h"

#include "Platform/Quaternion.h"
#include "Platform/Matrix4.h"
#include "Platform/Vector3f.h"

namespace BH
{
	const Quaternion Quaternion::IDENTITY = Quaternion( 0.0f, 0.0f, 0.0f, 1.0f );

	Quaternion Quaternion::CreateFromRotationMatrix( const Matrix4 & mat )
	{
		DirectX::XMMATRIX M = DirectX::XMLoadFloat4x4( &mat );

		Quaternion R;
		DirectX::XMStoreFloat4( &R, DirectX::XMQuaternionRotationMatrix( M ) );
		return R;
	}

	Quaternion Quaternion::Lerp( const Quaternion& q1, const Quaternion& q2, f32 t )
	{
		DirectX::XMVECTOR Q0 = DirectX::XMLoadFloat4(&q1);
		DirectX::XMVECTOR Q1 = DirectX::XMLoadFloat4(&q2);

		DirectX::XMVECTOR dot = DirectX::XMVector4Dot(Q0, Q1);

		DirectX::XMVECTOR R;
		if ( DirectX::XMVector4GreaterOrEqual( dot, DirectX::XMVectorZero() ) )
		{
			R = DirectX::XMVectorLerp( Q0, Q1, t );
		}
		else
		{
			DirectX::XMVECTOR tv = DirectX::XMVectorReplicate( t );
			DirectX::XMVECTOR t1v = DirectX::XMVectorReplicate( 1.f - t );
			DirectX::XMVECTOR X0 = DirectX::XMVectorMultiply( Q0, t1v );
			DirectX::XMVECTOR X1 = DirectX::XMVectorMultiply( Q1, tv );
			R = DirectX::XMVectorSubtract( X0, X1 );
		}

		Quaternion result;
		DirectX::XMStoreFloat4( &result, DirectX::XMQuaternionNormalize(R) );
		return result;
	}

	Quaternion Quaternion::Slerp( const Quaternion& q1, const Quaternion& q2, f32 t )
	{
		DirectX::XMVECTOR Q0 = DirectX::XMLoadFloat4( &q1 );
		DirectX::XMVECTOR Q1 = DirectX::XMLoadFloat4( &q2 );

		Quaternion result;
		DirectX::XMStoreFloat4( &result, DirectX::XMQuaternionSlerp( Q0, Q1, t ) );
		return result;
	}

	Quaternion Quaternion::Concatenate( const Quaternion& q1, const Quaternion& q2 )
	{
		DirectX::XMVECTOR Q0 = DirectX::XMLoadFloat4( &q1 );
		DirectX::XMVECTOR Q1 = DirectX::XMLoadFloat4( &q2 );

		Quaternion result;
		DirectX::XMStoreFloat4( &result, DirectX::XMQuaternionMultiply( Q1, Q0 ) );
		return result;
	}

	Quaternion Quaternion::Cross( const Quaternion & q1, const Quaternion & q2 )
	{
		// q1 X q2 = ( q1.w * q2.v + q2.w * q1.v + q1.v X q2.v, q1.w * q2.w - q1.v . q2.v )
		Vector3f v1( q1.x, q1.y, q1.z );
		Vector3f v2( q2.x, q2.y, q2.z );

		f32 angle = q1.w * q2.w - v1.Dot( v2 );
		Vector3f axis = q1.w * v2 + q2.w * v1 + v1.Cross( v2 );

		return Quaternion( axis, angle );
	}
}