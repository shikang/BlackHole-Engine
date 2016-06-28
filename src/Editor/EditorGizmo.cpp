#include "Editor/EditorGizmo.h"

#include "Platform/MeshLoader.h"
#include "Platform/Matrix4.h"
#include "Platform/Vector3f.h"

#include "Core/Math.h"
#include "Core/CoreEssential.h"
#include "Core/Hash.h"
#include "Core/Game.h"
#include "Core/SystemManager.h"
#include "Core/Renderer.h"
#include "Core/System.h"
#include "Core/InputManager.h"
#include "Core/Picking.h"

#include <algorithm>

#if GAME_EDITOR_MODE == BH_ENABLED

namespace BH
{
	const f32 EditorGizmo::GIZMO_DEFAULT_SCALE = 8.0f;
	BH_CONSTEXPR f32 GIZMO_MAX_SCALE = 15.0f;
	BH_CONSTEXPR f32 GIZMO_MIN_SCALE = 1.0f;

	EditorGizmo::EditorGizmo()
	: mParent( nullptr )
	, mAxisPicked( AXIS_TOTAL )
	, mMode( GIZMO_MOVE )
	, mGizScale( GIZMO_DEFAULT_SCALE, GIZMO_DEFAULT_SCALE, GIZMO_DEFAULT_SCALE )
	, mSnapValue( 0.2f )
	, mIsSnap( true )
	{
	}

	EditorGizmo::~EditorGizmo()
	{

	}

	void EditorGizmo::CreateGizmoMesh()
	{
		mColors[AXIS_X] = Vector4f::UNIT_X + Vector4f::UNIT_W;
		mColors[AXIS_Y] = Vector4f::UNIT_Y + Vector4f::UNIT_W;
		mColors[AXIS_Z] = Vector4f::UNIT_Z + Vector4f::UNIT_W;

		MeshLoader::CreateXLine( mAxis );
		MeshLoader::CreateLineCircle( mCircle );

		// Arrow head creation
		std::vector< Vertex > aVertexBuffer {
			Vertex{ Vector3f( 1.0f,  0.0f,  0.0f ),	Vector4f::UNIT_W, Vector2f::ZERO },		// Left Bottom Back
			Vertex{ Vector3f( 0.9f,  0.1f,  0.0f ),	Vector4f::UNIT_W, Vector2f::ZERO },		// Left Bottom Front
			Vertex{ Vector3f( 0.9f, -0.1f,  0.0f ),	Vector4f::UNIT_W, Vector2f::ZERO },		// Left Top Back
			Vertex{ Vector3f( 0.9f,  0.0f,  0.1f ),	Vector4f::UNIT_W, Vector2f::ZERO },		// Left Top Front
			Vertex{ Vector3f( 0.9f,  0.0f, -0.1f ),	Vector4f::UNIT_W, Vector2f::ZERO }		// Right Bottom Bac
		};

		std::vector< s32 > aIndexBuffer{
			0, 1, 
			0, 2,
			0, 3,
			0, 4
		};

		mArrowHead.InitialiseBuffers( aVertexBuffer, aIndexBuffer, true );

		std::vector< Vertex > bVertexBuffer {
			Vertex{ Vector3f( 0.9f, -0.05f,  0.05f ),	Vector4f::UNIT_W, Vector2f::ZERO },		// LBF
			Vertex{ Vector3f( 1.0f, -0.05f,  0.05f ),	Vector4f::UNIT_W, Vector2f::ZERO },		// RBF
			Vertex{ Vector3f( 1.0f,  0.05f,  0.05f ),	Vector4f::UNIT_W, Vector2f::ZERO },		// RTF
			Vertex{ Vector3f( 0.9f,  0.05f,  0.05f ),	Vector4f::UNIT_W, Vector2f::ZERO },		// LTF
			Vertex{ Vector3f( 0.9f,  0.05f, -0.05f ),	Vector4f::UNIT_W, Vector2f::ZERO },		// LTB
			Vertex{ Vector3f( 1.0f,  0.05f, -0.05f ),	Vector4f::UNIT_W, Vector2f::ZERO },		// RTB
			Vertex{ Vector3f( 0.9f, -0.05f, -0.05f ),	Vector4f::UNIT_W, Vector2f::ZERO },		// LBB
			Vertex{ Vector3f( 1.0f, -0.05f, -0.05f ),	Vector4f::UNIT_W, Vector2f::ZERO }		// RBB
		};

		std::vector< s32 > bIndexBuffer{
			0, 1, 
			1, 2,
			2, 3,
			3, 0,
			3, 4,
			4, 5,
			2, 5,
			4, 6,
			0, 6,
			5, 7,
			1, 7,
			6, 7
		};
		mBoxHead.InitialiseBuffers( bVertexBuffer, bIndexBuffer, true );

		// Rotations
		mRotations[AXIS_X] = Matrix4::IDENTITY;
		mRotations[AXIS_Y] = Matrix4::CreateRotationZ( Math::DegToRad( 90.0f ) );
		mRotations[AXIS_Z] = Matrix4::CreateRotationY( Math::DegToRad( -90.0f ) );

		mCircleRotations[AXIS_X] = Matrix4::CreateRotationY( Math::DegToRad( 90.0f ) );
		mCircleRotations[AXIS_Y] = Matrix4::CreateRotationX( Math::DegToRad( 90.0f ) );
		mCircleRotations[AXIS_Z] = Matrix4::IDENTITY;
	}

	void EditorGizmo::Draw( f32 )
	{
		if ( !mParent )
			return;

		Vector3f parentScale( *mParent->mScaleX, *mParent->mScaleY, *mParent->mScaleZ );
		Vector3f parentPos( *mParent->mPositionX, *mParent->mPositionY, *mParent->mPositionZ );
		Matrix4 trans = Matrix4::CreateTranslation( parentPos );
		Matrix4 scale;

		if ( mAxisPicked != AXIS_TOTAL && mMode == GIZMO_SCALE )
		{
			Vector3f scale_disp = parentScale - mOldValue;
			scale = Matrix4::CreateScale( mGizScale + scale_disp );
		}
		else
		{
			scale = Matrix4::CreateScale( mGizScale );
		}

		// Draw
		if ( mMode == GIZMO_ROTATE )
		{
			for (s32 i = 0; i < AXIS_TOTAL; ++i)
			{
				Matrix4 transform = trans * scale * mCircleRotations[i];
				Vector4f color = (mAxisPicked != i) ? mColors[i] : Vector4f( 1.0f, 1.0f, 0.0f, 1.0f );
				SYSTEM_MANAGER.GetGameComponentFromSystem<Renderer>()->DrawLines( transform, color, &mCircle, true );
			}
		}
		else
		{
			for ( s32 i = 0; i < AXIS_TOTAL; ++i )
			{
				Matrix4 transform = trans * scale * mRotations[i];
				Vector4f color = ( mAxisPicked != i ) ? Vector4f::ONE : Vector4f(1.0f, 1.0f, 0.0f, 1.0f);
				SYSTEM_MANAGER.GetGameComponentFromSystem<Renderer>()->DrawLines( transform, color, &mAxis, true );

				if ( mMode == GIZMO_MOVE )
					SYSTEM_MANAGER.GetGameComponentFromSystem<Renderer>()->DrawLines( transform, mColors[i], &mArrowHead, true );
				else
					SYSTEM_MANAGER.GetGameComponentFromSystem<Renderer>()->DrawLines( transform, mColors[i], &mBoxHead, true );
			}
		}

		// Logic
		if ( mAxisPicked != AXIS_TOTAL )
		{
			mCurrClickPoint = GetCurrentClickPoint( mAxisPicked == AXIS_X, mAxisPicked == AXIS_Y, mAxisPicked == AXIS_Z );

			// logic
			Vector3f disp( Vector3f::ZERO );

			switch ( mAxisPicked )
			{
			case AXIS_X:
				disp.x = mCurrClickPoint.x - mPrevClickPoint.x;
				break;

			case AXIS_Y:
				disp.y = mCurrClickPoint.y - mPrevClickPoint.y;
				break;

			case AXIS_Z:
				disp.z = mCurrClickPoint.z - mPrevClickPoint.z;
				break;

			default:
				break;

			};

			if ( mMode == GIZMO_MOVE )
			{
				Vector3f final_pos = mOldValue + disp;
				if ( mIsSnap )
				{
					final_pos.x = std::round( final_pos.x / mSnapValue ) * mSnapValue;
					final_pos.y = std::round( final_pos.y / mSnapValue ) * mSnapValue;
					final_pos.z = std::round( final_pos.z / mSnapValue ) * mSnapValue;
				}

				*mParent->mPositionX = final_pos.x;
				*mParent->mPositionY = final_pos.y;
				*mParent->mPositionZ = final_pos.z;
			}
			else if ( mMode == GIZMO_SCALE )
			{
				Vector3f final_scale = mOldValue + disp;
				if ( mIsSnap )
				{
					final_scale.x = std::round( final_scale.x / mSnapValue ) * mSnapValue;
					final_scale.y = std::round( final_scale.y / mSnapValue ) * mSnapValue;
					final_scale.z = std::round( final_scale.z / mSnapValue ) * mSnapValue;
				}

				*mParent->mScaleX = final_scale.x;
				*mParent->mScaleY = final_scale.y;
				*mParent->mScaleZ = final_scale.z;
			}
			else
			{
				disp *= 0.25f;	// Sensitivity
				Vector3f final_rot = mOldValue + disp;

				if ( mIsSnap )
				{
					final_rot.x = std::round( final_rot.x / mSnapValue ) * mSnapValue;
					final_rot.y = std::round( final_rot.y / mSnapValue ) * mSnapValue;
					final_rot.z = std::round( final_rot.z / mSnapValue ) * mSnapValue;
				}
				
				*mParent->mRotationX = final_rot.x;
				*mParent->mRotationY = final_rot.y;
				*mParent->mRotationZ = final_rot.z;
			}
		}
	}

	bool EditorGizmo::GetScenePosFromScreen( const Vector2f & screenPos, Vector2f & scenePos ) const
	{
		Renderer * renderer = SYSTEM_MANAGER.GetGameComponentFromSystem<Renderer>();

		Vector2f offset = renderer->GetSceneOffset();
		Vector2f dim = renderer->GetSceneDimension();
		Vector2f maxBorder = offset + dim;

		f32 mX = screenPos.x / static_cast< f32 >( GAME.Window.GetWidth() );
		f32 mY = 1.0f - screenPos.y / static_cast< f32 >( GAME.Window.GetHeight() );

		//std::cout << " Giz Screen: " << mX << ", " << mY << std::endl;

		// If mouse position is outside scene
		if ( mX < offset.x || mX > maxBorder.x ||
			 mY < offset.y || mY > maxBorder.y )
		{
			//std::cout << "--------------------------------------------------------------------------------" << std::endl;
			return false;
		}

		mX = ( mX - offset.x ) / dim.x * static_cast< f32 >( GAME.Window.GetWidth() );
		mY = ( 1.0f - ( mY - offset.y ) / dim.y ) * static_cast< f32 >( GAME.Window.GetHeight() );

		//std::cout << "Giz Scene screen space: " << mX << ", " << mY << std::endl;
		//std::cout << "--------------------------------------------------------------------------------" << std::endl;
		
		scenePos.x = mX;
		scenePos.y = mY;
		return true;
	}

	bool EditorGizmo::GetClickInfo( Vector3f & clickNear, Vector3f & clickFar )
	{
		Renderer * renderer = SYSTEM_MANAGER.GetGameComponentFromSystem<Renderer>();
		InputManager * input = SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>();

		Vector2f scenePos;

		if( GetScenePosFromScreen( input->GetMousePosition(), scenePos ) )
		{
			GetPickingInfo( renderer->GetPipeline()->GetWorld(),
							renderer->GetCamera()->GetView(),
							renderer->GetPipeline()->GetProjection(),
							scenePos,
							Vector2f( f32( GAME.Window.GetWidth() ), f32( GAME.Window.GetHeight() ) ),
							clickNear, clickFar );

			return true;
		}

		return false;
	}

	bool EditorGizmo::GetViewClickInfo( Vector3f & clickView )
	{
		Renderer * renderer = SYSTEM_MANAGER.GetGameComponentFromSystem<Renderer>();
		InputManager * input = SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>();

		Vector2f scenePos;

		if( GetScenePosFromScreen( input->GetMousePosition(), scenePos ) )
		{
			// Get NDC click point
			f32 viewport_width = static_cast< f32 >( GAME.Window.GetWidth() * 0.5f );
			f32 viewport_height = static_cast< f32 >( GAME.Window.GetHeight() * 0.5f );

			Vector2f m_pos = scenePos;
			Vector3f p( m_pos.x / viewport_width + 0.5f / viewport_width - 1.0f,
					    m_pos.y / -viewport_height - 0.5f / viewport_height + 1.0f,
						renderer->Near );	// DIrectX is looking toward positive Z

			// Get view frame of click points.
			p.x /= renderer->GetPipeline()->GetProjection()._11;
			p.y /= renderer->GetPipeline()->GetProjection()._22;
			clickView = p;

			return true;
		}

		return false;
	}

	Vector3f EditorGizmo::GetCurrentClickPoint( bool alongX, bool alongY, bool alongZ )
	{
		if ( mMode == GIZMO_ROTATE )
		{
			Matrix4 view = SYSTEM_MANAGER.GetGameComponentFromSystem<Renderer>()->GetCamera()->GetView();
			Vector2f mPos = SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->GetMousePosition();

			if ( alongX )
			{
				Vector4f viewX = view * Vector4f::UNIT_X;
				
				// If Y dominant
				if ( fabs( viewX.y ) >= fabs( viewX.x ) && fabs( viewX.y ) >= fabs( viewX.z ) )
				{
					return ( signbit( viewX.y ) ? 1.0f : -1.0f ) * Vector3f::UNIT_X * mPos.x;
				}
				// X/Z dominant
				else
				{
					float sign;
					if ( fabs( viewX.x ) >= fabs( viewX.z ) )
						sign = signbit( viewX.x ) ? 1.0f : -1.0f;
					else
						sign = signbit( viewX.z ) ? 1.0f : -1.0f;

					return sign * Vector3f::UNIT_X * mPos.y;
				}
			}
			else if ( alongY )
			{
				Vector4f viewY = view * Vector4f::UNIT_Y;

				// If Y dominant
				if ( fabs( viewY.y ) >= fabs( viewY.x ) && fabs( viewY.y ) >= fabs( viewY.z ) )
				{
					return ( signbit( viewY.y ) ? 1.0f : -1.0f ) * Vector3f::UNIT_Y * mPos.x;
				}
				// X/Z dominant
				else
				{
					float sign;
					if ( fabs( viewY.x ) >= fabs( viewY.z ) )
						sign = signbit( viewY.x ) ? 1.0f : -1.0f;
					else
						sign = signbit( viewY.z ) ? 1.0f : -1.0f;

					return sign * Vector3f::UNIT_Y * mPos.y;
				}
			}
			else
			{
				Vector4f viewZ = view * Vector4f::UNIT_Z;

				// If Y dominant
				if ( fabs( viewZ.y ) >= fabs( viewZ.x ) && fabs( viewZ.y ) >= fabs( viewZ.z ) )
				{
					return ( signbit( viewZ.y ) ? 1.0f : -1.0f ) * Vector3f::UNIT_Z * mPos.x;
				}
				// X/Z dominant
				else
				{
					float sign;
					if ( fabs( viewZ.x ) >= fabs( viewZ.z ) )
						sign = signbit( viewZ.x ) ? 1.0f : -1.0f;
					else
						sign = signbit( viewZ.z ) ? 1.0f : -1.0f;

					return sign * Vector3f::UNIT_Z * mPos.y;
				}
			}
		}

		Vector3f world_near, world_far;

		if( !GetClickInfo( world_near, world_far ) )
			return mCurrClickPoint;

		Vector3f v = world_far - world_near;

		Vector3f diff( std::fabs( v.x ), std::fabs( v.y ), std::fabs( v.z ) );

		// Not elegant but Im just trying it out
		//if ( mMode == GIZMO_ROTATE )
		//{
		//	if ( alongX )
		//		v *= ( *mParent->mPositionX - world_near.x ) / ( world_far.x - world_near.x );
		//	else if ( alongY )
		//		v *= ( *mParent->mPositionY - world_near.y ) / ( world_far.y - world_near.y );
		//	else
		//		v *= ( *mParent->mPositionZ - world_near.z ) / ( world_far.z - world_near.z );
		//}
		//else
		//{
			if ( alongX )
			{
				if ( alongY )
					v *= ( *mParent->mPositionZ - world_near.z ) / ( world_far.z - world_near.z );
				else if ( alongZ )
					v *= ( *mParent->mPositionY - world_near.y ) / ( world_far.y - world_near.y );
				else if ( diff.y >= diff.z )
					v *= ( *mParent->mPositionY - world_near.y ) / ( world_far.y - world_near.y );
				else
					v *= ( *mParent->mPositionZ - world_near.z ) / ( world_far.z - world_near.z );
			}
			else if ( alongY )
			{
				if ( alongZ )
					v *= ( *mParent->mPositionX - world_near.x ) / ( world_far.x - world_near.x );
				else if ( diff.x >= diff.z )
					v *= ( *mParent->mPositionX - world_near.x ) / ( world_far.x - world_near.x );
				else
					v *= ( *mParent->mPositionZ - world_near.z ) / ( world_far.z - world_near.z );
			}
			else if ( alongZ )
			{
				if ( diff.x >= diff.y )
					v *= ( *mParent->mPositionX - world_near.x ) / ( world_far.x - world_near.x );
				else
					v *= ( *mParent->mPositionY - world_near.y ) / ( world_far.y - world_near.y );
			}
		//}

		return world_near + v;
	}

	void EditorGizmo::SetParent( SelectedInfo * parent )
	{
		mParent = parent;
	}

	s32 EditorGizmo::RotGizmoPick3D()
	{
		Vector3f click_near, click_far, ray;
		if( !GetClickInfo( click_near, click_far ) )
			return AXIS_TOTAL;

		ray = click_far - click_near;
		ray.Normalize();

		// in world frame
		Vector3f scale = mGizScale;
		f32 radius = std::max( scale.x, std::max( scale.y, scale.z ) );

		Vector3f pos = GetPosition();

		Vector3f axis[AXIS_TOTAL];
		axis[AXIS_X] = Vector3f( 1.0f, 0.0f, 0.0f );
		axis[AXIS_Y] = Vector3f( 0.0f, 1.0f, 0.0f );
		axis[AXIS_Z] = Vector3f( 0.0f, 0.0f, 1.0f );

		f32 min_time = FLT_MAX;
		s32 min_index = AXIS_TOTAL;

		for ( s32 i = 0; i < AXIS_TOTAL; ++i )
		{
			f32 t = RayPlaneEdgeIntersection( ray, click_near, pos, radius, axis[i] );

			if ( t >= 0.0f )
			{
				if ( min_index == AXIS_TOTAL || t < min_time )
				{
					min_time = t;
					min_index = i;
				}
			}
		}

		return min_index;
	}

	s32 EditorGizmo::GizmoPick3D()
	{
#if 0
		Vector3f v( 0.0f, 0.0f, 0.0f ), p;
		if( !GetViewClickInfo( p ) )
			return AXIS_TOTAL;

		// in view frame
		Vector3f scale = mGizScale;

		Vector3f pos = GetPosition();
		Vector3f aPos[AXIS_TOTAL];

		Camera * camera = SYSTEM_MANAGER.GetGameComponentFromSystem<Renderer>()->GetCamera();
		aPos[AXIS_X] = camera->GetView() * ( pos + Vector3f( 0.9f * scale.x, 0.0f, 0.0f ) );
		aPos[AXIS_Y] = camera->GetView() * ( pos + Vector3f( 0.0f, 0.9f * scale.y, 0.0f ) );
		aPos[AXIS_Z] = camera->GetView() * ( pos + Vector3f( 0.0f, 0.0f, 0.9f * scale.z ) );

		// hard code here because this code does not need to be maintain
		f32 radius = GetPickingRadius( scale * 0.4f, camera->GetView() );

		f32 min_time = FLT_MAX;
		s32 min_index = AXIS_TOTAL;

		for ( s32 i = 0; i < AXIS_TOTAL; ++i )
		{
			f32 t = RaySphereIntersection( v, p, aPos[i], radius );

			if ( t >= 0.0f )
			{
				if ( min_index == AXIS_TOTAL || t < min_time )
				{
					min_time = t;
					min_index = i;
				}
			}
		}

		return min_index;
#else
		Vector3f click_near, click_far;
		if( !GetClickInfo( click_near, click_far ) )
			return AXIS_TOTAL;

		Vector3f ray = click_far - click_near;
		ray.Normalize();

		f32 minTime = FLT_MAX;
		s32 minChoice = AXIS_TOTAL;

		Vector3f scale = mGizScale;

		Vector3f pos = GetPosition();
		Vector3f aPos[AXIS_TOTAL];

		aPos[AXIS_X] = pos + Vector3f( 0.9f * scale.x, 0.0f, 0.0f );
		aPos[AXIS_Y] = pos + Vector3f( 0.0f, 0.9f * scale.y, 0.0f );
		aPos[AXIS_Z] = pos + Vector3f( 0.0f, 0.0f, 0.9f * scale.z );

		scale *= 0.2f;

		Vector3f axis[3] = {
				Vector3f::UNIT_X,
				Vector3f::UNIT_Y,
				Vector3f::UNIT_Z
			};


		for ( s32 i = 0; i < AXIS_TOTAL; ++i )
		{
			f32 t = RayOBBIntersection( ray, click_near, aPos[i], scale, axis );

			if ( t >= 0.0f )
			{
				if ( minChoice == AXIS_TOTAL || t < minTime )
				{
					minTime = t;
					minChoice = i;
				}
			}
		}

		return minChoice;
#endif
	}

	bool EditorGizmo::PickGizmo()
	{
		if ( mMode == GIZMO_ROTATE )
		{
			mAxisPicked = RotGizmoPick3D();
		}
		else
		{
			mAxisPicked = GizmoPick3D();
		}
		//mAxisPicked = (mAxisPicked == -1) ? AXIS_TOTAL : mAxisPicked;
		mPrevClickPoint = GetCurrentClickPoint( mAxisPicked == AXIS_X,
												mAxisPicked == AXIS_Y,
												mAxisPicked == AXIS_Z );

		mCurrClickPoint = mPrevClickPoint;

		if ( mMode == GIZMO_MOVE )
		{
			mOldValue = Vector3f( *mParent->mPositionX, *mParent->mPositionY, *mParent->mPositionZ );
		}
		else if ( mMode == GIZMO_SCALE )
		{
			mOldValue = Vector3f( *mParent->mScaleX, *mParent->mScaleY, *mParent->mScaleZ );
		}
		else
		{
			mOldValue = Vector3f( *mParent->mRotationX, *mParent->mRotationY, *mParent->mRotationZ );
		}

		return mAxisPicked != AXIS_TOTAL;
	}

	void EditorGizmo::UnpickGizmo()
	{
		mAxisPicked = AXIS_TOTAL;
	}

	void EditorGizmo::IncreaseScale( f32 step )
	{
		if ( mGizScale.x + step > GIZMO_MAX_SCALE )
			return;

		mGizScale.x += step;
		mGizScale.y += step;
		mGizScale.z += step;
	}

	void EditorGizmo::DecreaseScale( f32 step )
	{
		if ( mGizScale.x - step < GIZMO_MIN_SCALE )
			return;

		mGizScale.x -= step;
		mGizScale.y -= step;
		mGizScale.z -= step;
	}

	Vector3f EditorGizmo::GetPosition() const
	{
		if ( mParent )
			return Vector3f( *mParent->mPositionX, *mParent->mPositionY, *mParent->mPositionZ );
		else
			return Vector3f();
	}
}
#endif