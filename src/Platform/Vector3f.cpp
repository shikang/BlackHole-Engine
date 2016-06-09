#include "Platform/Stdafx.h"

#include "Platform/Vector3f.h"
#include "Platform/Matrix4.h"

namespace BH
{
	const Vector3f Vector3f::ZERO = Vector3f( 0.0f );
	const Vector3f Vector3f::ONE = Vector3f( 1.0f );
	const Vector3f Vector3f::UNIT_X = Vector3f( 1.0f, 0.0f, 0.0f );
	const Vector3f Vector3f::UNIT_Y = Vector3f( 0.0f, 1.0f, 0.0f );
	const Vector3f Vector3f::UNIT_Z = Vector3f( 0.0f, 0.0f, 1.0f );

	Vector3f Vector3f::Unproject( const Vector3f & vec, f32 viewportX, f32 viewportY, f32 viewportWidth, f32 viewportHeight, 
								  f32 viewportNear, f32 viewportFar, const Matrix4 & projection, const Matrix4 & view,
								  const Matrix4 & world )
	{
		//DirectX::XMVECTOR v = DirectX::XMLoadFloat3( &vec );
		DirectX::XMVECTOR v = DirectX::XMVectorSet( vec.x, vec.y, vec.z, 1.0f );

		Matrix4 dProjection = projection;
		Matrix4 dView = view;
		Matrix4 dWorld = world;

		DirectX::XMMATRIX p = DirectX::XMLoadFloat4x4( &dProjection );
		DirectX::XMMATRIX l = DirectX::XMLoadFloat4x4( &dView );
		DirectX::XMMATRIX w = DirectX::XMLoadFloat4x4( &dWorld );

		DirectX::XMVECTOR r = DirectX::XMVector3Unproject( v, viewportX, viewportY, viewportWidth, viewportHeight, 
														   viewportNear, viewportFar, p, l, w );

		Vector3f result;
		DirectX::XMStoreFloat3( &result, r );
		return result;
	}
}