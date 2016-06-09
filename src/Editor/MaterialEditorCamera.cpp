// Precompiled
#include "Core/Stdafx.h"

#include "Platform/Quaternion.h"

#include "Core/Delegate.h"
#include "Core/Renderer.h"
#include "Core/InputManager.h"
#include "Core/Math.h"
#include "Core/Picking.h"

#include "Editor/MaterialEditorCamera.h"

namespace BH
{
	MaterialEditorCamera::MaterialEditorCamera()
	: mActive( false )
	, mClicked( false )
	, mScrolled( false )
	, mPrevCamera( nullptr )
	{

	}

	MaterialEditorCamera::~MaterialEditorCamera()
	{

	}

	void MaterialEditorCamera::Initialise()
	{
		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddMouseEvent( Mouse::Left, Input::Triggered, 
																				  Delegate<void( s32, s32 )>::FromMethod<E_METHOD( MaterialEditorCamera, ClickTriggerClickCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddMouseEvent( Mouse::Left, Input::Released, 
																				  Delegate<void( s32, s32 )>::FromMethod<E_METHOD( MaterialEditorCamera, ClickReleaseClickCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddMouseWheelEvent( Delegate<void( s32 )>::FromMethod<E_METHOD( MaterialEditorCamera, ScrollCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddMouseMoveEvent( Delegate<void( s32, s32 )>::FromMethod<E_METHOD( MaterialEditorCamera, MouseMoveCallback )>( this ) );
	}

	void MaterialEditorCamera::Shutdown()
	{
		SetEditorCameraActive( false );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveMouseEvent( Mouse::Left, Input::Triggered, 
																					 Delegate<void( s32, s32 )>::FromMethod<E_METHOD( MaterialEditorCamera, ClickTriggerClickCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveMouseEvent( Mouse::Left, Input::Released, 
																					 Delegate<void( s32, s32 )>::FromMethod<E_METHOD( MaterialEditorCamera, ClickReleaseClickCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveMouseWheelEvent( Delegate<void( s32 )>::FromMethod<E_METHOD( MaterialEditorCamera, ScrollCallback )>( this ) );
		
		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveMouseMoveEvent( Delegate<void( s32, s32 )>::FromMethod<E_METHOD( MaterialEditorCamera, MouseMoveCallback )>( this ) );
	}

	void MaterialEditorCamera::Update( f32 )
	{
		if ( !mActive )
			return;
	}

	void MaterialEditorCamera::SetEditorCameraActive( bool active )
	{
		if ( mActive == active )
			return;

		mActive = active;
		Renderer * renderer = SYSTEM_MANAGER.GetGameComponentFromSystem<Renderer>();
		if ( mActive )
		{
			mPrevCamera = renderer->GetCamera();
			renderer->SetCamera( &mEditorCamera );
		}
		else if ( renderer->GetCamera() == &mEditorCamera )
		{
			renderer->SetCamera( mPrevCamera );
			mPrevCamera = nullptr;
		}
	}

	void MaterialEditorCamera::ClickTriggerClickCallback( s32 x, s32 y )
	{
		mClicked = true;
		mCurrentMousePos.x = static_cast<f32>( x );
		mCurrentMousePos.y = static_cast<f32>( y );
	}

	void MaterialEditorCamera::ClickReleaseClickCallback( s32, s32 )
	{
		mClicked = false;
	}

	void MaterialEditorCamera::ScrollCallback( s32 x )
	{
		if ( mClicked )
		{
			mScrolled = false;
			return;
		}

		Vector3f dir = mEditorCamera.GetTarget() - mEditorCamera.GetPosition();
		f32 length = dir.LengthSq();
		dir.Normalize();

		// If reached minimum or maximum
		if ( ( x < 0 && length >= 900.0f ) ||
			 ( x > 0 && length <= 25.0f ) )
			 return;

		mScrolled = x != 0;
		Vector3f move = dir * f32( x );

		//mEditorCamera.SetTarget( mEditorCamera.GetTarget() + move );
		mEditorCamera.SetPosition( mEditorCamera.GetPosition() + move );

		mScrolled = false;
	}

	void MaterialEditorCamera::MouseMoveCallback( s32 x, s32 y )
	{
		if ( !mActive || !mClicked  || mScrolled )
			return;

		mPreviousMousePos = mCurrentMousePos;
		mCurrentMousePos.x = static_cast<f32>( x );
		mCurrentMousePos.y = static_cast<f32>( y );

		if ( mClicked )
		{
			Vector2f diff = mCurrentMousePos - mPreviousMousePos;
			diff *= 0.25f * ( Math::Pi / 180.0f );
		
			// Gimbal lockless rotation
			Vector3f view = mEditorCamera.GetPosition() - mEditorCamera.GetTarget();

			const Matrix4 & viewMat = mEditorCamera.GetView();
			// Row major
			Vector3f side( viewMat.m[0][0], viewMat.m[1][0], viewMat.m[2][0] );
			Vector3f front( viewMat.m[0][2], viewMat.m[1][2], viewMat.m[2][2] );

			Quaternion pitch = Quaternion::CreateFromAxisAngle( side, diff.y );
			Quaternion heading = Quaternion::CreateFromAxisAngle( Vector3f::UNIT_Y, diff.x );
			Quaternion t = Quaternion::Cross( pitch, heading );

			Vector3f rotPos = t * view;
			mEditorCamera.SetPosition( rotPos + mEditorCamera.GetTarget() );

			f32 scale = front.Dot( Vector3f::UNIT_Y );
			if ( fabs( scale ) > 0.9f )
			{
				mEditorCamera.SetUp( t * mEditorCamera.GetUp() );
			}
			else
			{
				// Projection on xz plane
				Vector3f proj = front - ( scale * Vector3f::UNIT_Y );

				if ( proj.Cross( side ).Dot( Vector3f::UNIT_Y ) > 0.0f )
					mEditorCamera.SetUp( Vector3f::UNIT_Y );
				else
					mEditorCamera.SetUp( -Vector3f::UNIT_Y );
			}
		}
	}
}