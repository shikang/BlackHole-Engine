// Precompiled
#include "Core/Stdafx.h"

#include "Platform/Quaternion.h"

#include "Core/Delegate.h"
#include "Core/Renderer.h"
#include "Core/InputManager.h"
#include "Core/Math.h"
#include "Core/Picking.h"

#include "Game/DebugCamera.h"

#include <algorithm>

namespace BH
{
	DebugCamera::DebugCamera()
	: mActive( false )
	, mClicked( false )
	, mScrolled( false )
	, mRight( false )
	, mPrevCamera( nullptr )
	, mW( false )
	, mA( false )
	, mS( false )
	, mD( false )
	, mPressedTime( 0.0f )
	{

	}

	DebugCamera::~DebugCamera()
	{

	}

	void DebugCamera::Initialise()
	{
		// @test
		//SYSTEM_MANAGER.GetGameComponentFromSystem<BH::DebugCamera>()->SetDebugCameraActive( true, true );
		SetDebugCameraActive( true, true );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddMouseEvent( Mouse::Right, Input::Triggered, 
																				  Delegate<void( s32, s32 )>::FromMethod<E_METHOD( DebugCamera, RightTriggerClickCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddMouseEvent( Mouse::Right, Input::Released, 
																				  Delegate<void( s32, s32 )>::FromMethod<E_METHOD( DebugCamera, RightReleaseClickCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddMouseWheelEvent( Delegate<void( s32 )>::FromMethod<E_METHOD( DebugCamera, ScrollCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddMouseMoveEvent( Delegate<void( s32, s32 )>::FromMethod<E_METHOD( DebugCamera, MouseMoveCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddMouseEvent( Mouse::Middle, Input::Triggered, 
																				  Delegate<void( s32, s32 )>::FromMethod<E_METHOD( DebugCamera, MiddleTriggerClickCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddMouseEvent( Mouse::Middle, Input::Released, 
																				  Delegate<void( s32, s32 )>::FromMethod<E_METHOD( DebugCamera, MiddleReleasedCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::W, Input::Triggered, 
																				Delegate<void()>::FromMethod<E_METHOD( DebugCamera, WTriggeredCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::W, Input::Released, 
																				Delegate<void()>::FromMethod<E_METHOD( DebugCamera, WReleaseCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::A, Input::Triggered, 
																				Delegate<void()>::FromMethod<E_METHOD( DebugCamera, ATriggeredCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::A, Input::Released, 
																				Delegate<void()>::FromMethod<E_METHOD( DebugCamera, AReleaseCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::S, Input::Triggered, 
																				Delegate<void()>::FromMethod<E_METHOD( DebugCamera, STriggeredCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::S, Input::Released, 
																				Delegate<void()>::FromMethod<E_METHOD( DebugCamera, SReleaseCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::D, Input::Triggered, 
																				Delegate<void()>::FromMethod<E_METHOD( DebugCamera, DTriggeredCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::D, Input::Released, 
																				Delegate<void()>::FromMethod<E_METHOD( DebugCamera, DReleaseCallback )>( this ) );

		mDebugCamera.SetPosition( Vector3f( 11.0f, 11.0f, -20.0f ) );
	}

	void DebugCamera::Shutdown()
	{
		SetDebugCameraActive( false, false );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveMouseEvent( Mouse::Right, Input::Triggered, 
																					 Delegate<void( s32, s32 )>::FromMethod<E_METHOD( DebugCamera, RightTriggerClickCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveMouseEvent( Mouse::Right, Input::Released, 
																					 Delegate<void( s32, s32 )>::FromMethod<E_METHOD( DebugCamera, RightReleaseClickCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveMouseWheelEvent( Delegate<void( s32 )>::FromMethod<E_METHOD( DebugCamera, ScrollCallback )>( this ) );
		
		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveMouseMoveEvent( Delegate<void( s32, s32 )>::FromMethod<E_METHOD( DebugCamera, MouseMoveCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveMouseEvent( Mouse::Middle, Input::Triggered, 
																				     Delegate<void( s32, s32 )>::FromMethod<E_METHOD( DebugCamera, MiddleTriggerClickCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveMouseEvent( Mouse::Middle, Input::Released, 
																				     Delegate<void( s32, s32 )>::FromMethod<E_METHOD( DebugCamera, MiddleReleasedCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::W, Input::Triggered, 
																				   Delegate<void()>::FromMethod<E_METHOD( DebugCamera, WTriggeredCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::W, Input::Released, 
																				   Delegate<void()>::FromMethod<E_METHOD( DebugCamera, WReleaseCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::A, Input::Triggered, 
																				   Delegate<void()>::FromMethod<E_METHOD( DebugCamera, ATriggeredCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::A, Input::Released, 
																				   Delegate<void()>::FromMethod<E_METHOD( DebugCamera, AReleaseCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::S, Input::Triggered, 
																				   Delegate<void()>::FromMethod<E_METHOD( DebugCamera, STriggeredCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::S, Input::Released, 
																				   Delegate<void()>::FromMethod<E_METHOD( DebugCamera, SReleaseCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::D, Input::Triggered, 
																				   Delegate<void()>::FromMethod<E_METHOD( DebugCamera, DTriggeredCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::D, Input::Released, 
																				   Delegate<void()>::FromMethod<E_METHOD( DebugCamera, DReleaseCallback )>( this ) );
	}

	void DebugCamera::Update( f32 dt )
	{
		if ( !mActive )
			return;

		Vector3f dir = Vector3f::ZERO;
		if ( mRight )
		{
			if ( mW )
				dir.z = 1.0f;
			else if ( mS )
				dir.z = -1.0f;

			if ( mA )
				dir.y = -1.0f;
			else if ( mD )
				dir.y = 1.0f;

			if ( dir.LengthSq() > FLT_EPSILON )
			{
				dir.Normalize();
				mPressedTime += 2.0f * dt;
			}

			const Matrix4 & viewMat = mDebugCamera.GetView();
			Vector3f side( viewMat.m[0][0], viewMat.m[1][0], viewMat.m[2][0] );
			Vector3f front( viewMat.m[0][2], viewMat.m[1][2], viewMat.m[2][2] );
			Vector3f move = std::min( std::exp( mPressedTime ), 50.0f ) * dt * ( dir.y * side + dir.z * front );

			mDebugCamera.SetTarget( mDebugCamera.GetTarget() + move );
			mDebugCamera.SetPosition( mDebugCamera.GetPosition() + move );
		}
	}

	void DebugCamera::SetDebugCameraActive( bool active, bool currentView )
	{
		if ( mActive == active )
			return;

		mActive = active;
		Renderer * renderer = SYSTEM_MANAGER.GetGameComponentFromSystem<Renderer>();
		if ( mActive )
		{
			mPrevCamera = renderer->GetCamera();
			if ( currentView )
			{
				Camera * currentCam = renderer->GetCamera();

				mDebugCamera.SetPosition( currentCam->GetPosition() );
				mDebugCamera.SetTarget( currentCam->GetTarget() );
				mDebugCamera.SetUp( currentCam->GetUp() );
			}

			renderer->SetCamera( &mDebugCamera );
		}
		else if ( renderer->GetCamera() == &mDebugCamera )
		{
			renderer->SetCamera( mPrevCamera );
			mPrevCamera = nullptr;
			if ( currentView )
			{
				Camera * currentCam = renderer->GetCamera();

				currentCam->SetPosition( mDebugCamera.GetPosition() );
				currentCam->SetTarget( mDebugCamera.GetTarget() );
				currentCam->SetUp( mDebugCamera.GetUp() );
			}
		}
	}

	void DebugCamera::RightTriggerClickCallback( s32 x, s32 y )
	{
		mRight = true;
		mCurrentMousePos.x = static_cast<f32>( x );
		mCurrentMousePos.y = static_cast<f32>( y );
	}

	void DebugCamera::RightReleaseClickCallback( s32, s32 )
	{
		mRight = false;
	}

	void DebugCamera::ScrollCallback( s32 x )
	{
		if ( mClicked || mRight )
		{
			mScrolled = false;
			return;
		}

		mScrolled = x != 0;
		Vector3f dir = mDebugCamera.GetTarget() - mDebugCamera.GetPosition();
		dir.Normalize();

		Vector3f move = dir * f32( x );

		mDebugCamera.SetTarget( mDebugCamera.GetTarget() + move );
		mDebugCamera.SetPosition( mDebugCamera.GetPosition() + move );

		mScrolled = false;
	}

	void DebugCamera::MouseMoveCallback( s32 x, s32 y )
	{
		if ( !mActive || ( !mClicked && !mRight ) || mScrolled )
			return;

		mPreviousMousePos = mCurrentMousePos;
		mCurrentMousePos.x = static_cast<f32>( x );
		mCurrentMousePos.y = static_cast<f32>( y );

		if ( mRight )
		{
			Vector2f diff = mCurrentMousePos - mPreviousMousePos;
			diff *= 0.25f * ( Math::Pi / 180.0f );
		
			// Gimbal lockless rotation
			Vector3f view = mDebugCamera.GetTarget() - mDebugCamera.GetPosition();

			const Matrix4 & viewMat = mDebugCamera.GetView();
			// Row major
			Vector3f side( viewMat.m[0][0], viewMat.m[1][0], viewMat.m[2][0] );
			Vector3f front( viewMat.m[0][2], viewMat.m[1][2], viewMat.m[2][2] );

			Quaternion pitch = Quaternion::CreateFromAxisAngle( side, diff.y );
			Quaternion heading = Quaternion::CreateFromAxisAngle( Vector3f::UNIT_Y, diff.x );
			Quaternion t = Quaternion::Cross( pitch, heading );

			Vector3f rotPos = t * view;
			//mDebugCamera.SetPosition( rotPos + mDebugCamera.GetTarget() );
			mDebugCamera.SetTarget( rotPos + mDebugCamera.GetPosition() );

			f32 scale = front.Dot( Vector3f::UNIT_Y );
			if ( fabs( scale ) > 0.9f )
			{
				mDebugCamera.SetUp( t * mDebugCamera.GetUp() );
			}
			else
			{
				// Projection on xz plane
				Vector3f proj = front - ( scale * Vector3f::UNIT_Y );

				if ( proj.Cross( side ).Dot( Vector3f::UNIT_Y ) > 0.0f )
					mDebugCamera.SetUp( Vector3f::UNIT_Y );
				else
					mDebugCamera.SetUp( -Vector3f::UNIT_Y );
			}
		}
		else
		{
			Renderer * renderer = SYSTEM_MANAGER.GetGameComponentFromSystem<Renderer>();

			const Matrix4 & world = renderer->GetPipeline()->GetWorld();
			const Matrix4 & viewMat = mDebugCamera.GetView();
			const Matrix4 & projMat = renderer->GetPipeline()->GetProjection();
			//Vector3f side( viewMat.m[0][0], viewMat.m[1][0], viewMat.m[2][0] );
			//Vector3f up( viewMat.m[0][1], viewMat.m[1][1], viewMat.m[2][1] );

			Vector2f screenRes( f32( GAME.Window.GetWidth() ), f32( GAME.Window.GetHeight() ) );
			Vector3f preNear, preFar, currNear, currFar;
			GetPickingInfo( world,
							viewMat,
							projMat,
							mPreviousMousePos,
							screenRes,
							preNear,
							preFar );

			GetPickingInfo( world,
							viewMat,
							projMat,
							mCurrentMousePos,
							screenRes,
							currNear,
							currFar);

			Vector3f move = -( currNear - preNear ) * Vector3f::Distance( mDebugCamera.GetTarget(), mDebugCamera.GetPosition() );

			mDebugCamera.SetTarget( mDebugCamera.GetTarget() + move );
			mDebugCamera.SetPosition( mDebugCamera.GetPosition() + move );
		}
	}

	void DebugCamera::MiddleTriggerClickCallback( s32 x, s32 y )
	{
		mClicked = true;
		mCurrentMousePos.x = static_cast<f32>( x );
		mCurrentMousePos.y = static_cast<f32>( y );
	}

	void DebugCamera::MiddleReleasedCallback( s32, s32 )
	{
		mClicked = false;
	}

	void DebugCamera::FocusOn( const Matrix4 & world, const AABB & aabb )
	{
		const Vector3f & minPoint = aabb.mMin;
		const Vector3f & maxPoint = aabb.mMax;

		Vector3f boundingPoints[8] = {
									     minPoint,											//!< 000
										 Vector3f( minPoint.x, minPoint.y, maxPoint.z ),	//!< 001 
										 Vector3f( minPoint.x, maxPoint.y, minPoint.z ),	//!< 010 
										 Vector3f( minPoint.x, maxPoint.y, maxPoint.z ),	//!< 011
										 Vector3f( maxPoint.x, minPoint.y, minPoint.z ),	//!< 100
										 Vector3f( maxPoint.x, minPoint.y, maxPoint.z ),	//!< 101
										 Vector3f( maxPoint.x, maxPoint.y, minPoint.z ),	//!< 110
										 maxPoint,											//!< 111
									 };

		Vector3f viewMin( FLT_MAX, FLT_MAX, FLT_MAX ), viewMax( -FLT_MAX, -FLT_MAX, -FLT_MAX );
		Vector3f worldMin( FLT_MAX, FLT_MAX, FLT_MAX ), worldMax( -FLT_MAX, -FLT_MAX, -FLT_MAX );
		Matrix4 view = mDebugCamera.GetView();
		for ( s32 i = 0; i < 8; ++i )
		{
			Vector3f p = world * boundingPoints[i];

			worldMin.x = std::min( worldMin.x, p.x );
			worldMin.y = std::min( worldMin.y, p.y );
			worldMin.z = std::min( worldMin.z, p.z );

			worldMax.x = std::max( worldMax.x, p.x );
			worldMax.y = std::max( worldMax.y, p.y );
			worldMax.z = std::max( worldMax.z, p.z );

			p = view * p;

			viewMin.x = std::min( viewMin.x, p.x );
			viewMin.y = std::min( viewMin.y, p.y );
			viewMin.z = std::min( viewMin.z, p.z );

			viewMax.x = std::max( viewMax.x, p.x );
			viewMax.y = std::max( viewMax.y, p.y );
			viewMax.z = std::max( viewMax.z, p.z );
		}

		f32 fov = SYSTEM_MANAGER.GetGameComponentFromSystem<Renderer>()->GetPipeline()->GetFieldOfView();
		f32 aspectRatio = SYSTEM_MANAGER.GetGameComponentFromSystem<Renderer>()->GetPipeline()->GetAspectRatio();
		f32 halfWidth = ( viewMax.x - viewMin.x );
		f32 halfHeight = ( viewMax.y - viewMin.y );

		f32 tanFov = tan( 0.5f * fov );
		f32 distance1 = ( halfWidth / aspectRatio ) / tanFov;
		f32 distance2 = ( halfHeight ) / tanFov;
		f32 distance = std::max( distance1, distance2 );

		Vector3f center = worldMin + 0.5f *( worldMax - worldMin );

		mDebugCamera.SetTarget( center );
		mDebugCamera.SetPosition( center - distance * Vector3f::UNIT_Z );
	}

	void DebugCamera::WTriggeredCallback()
	{
		mW = true;
	}

	void DebugCamera::WReleaseCallback()
	{
		mW = false;
		mPressedTime = 0.0f;
	}

	void DebugCamera::ATriggeredCallback()
	{
		mA = true;
	}

	void DebugCamera::AReleaseCallback()
	{
		mA = false;
		mPressedTime = 0.0f;
	}

	void DebugCamera::STriggeredCallback()
	{
		mS = true;
	}

	void DebugCamera::SReleaseCallback()
	{
		mS = false;
		mPressedTime = 0.0f;
	}

	void DebugCamera::DTriggeredCallback()
	{
		mD = true;
	}

	void DebugCamera::DReleaseCallback()
	{
		mD = false;
		mPressedTime = 0.0f;
	}
}