// Precompiled Header
#include "Platform/Stdafx.h"

#include "Platform/Camera.h"

namespace BH
{
	Camera::Camera()
	: mPosition( 0.0f, 0.0f, -30.0f )
	, mTarget( 0.0f, 0.0f, 0.0f )
	, mUp( 0.0f, 1.0f, 0.0f )
	, mUpdate( true )
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::SetPosition( const Vector3f & position )
	{
		mPosition = position;
		mUpdate = true;
	}

	void Camera::SetTarget( const Vector3f & lookat )
	{
		mTarget = lookat;
		mUpdate = true;
	}

	void Camera::SetUp( const Vector3f & up )
	{
		mUp = up;
		mUpdate = true;
	}

	Vector3f Camera::GetPosition() const
	{
		return mPosition;
	}

	Vector3f Camera::GetTarget() const
	{
		return mTarget;
	}

	Vector3f Camera::GetUp() const
	{
		return mUp;
	}

	Matrix4 Camera::GetView() const
	{
		return mView;
	}

	void Camera::UpdateView()
	{
		if ( !mUpdate )
			return;

		mView = Matrix4::CreateLookAt( mPosition, mTarget, mUp );
		mUpdate = false;
	}
}