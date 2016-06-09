#ifndef BH_EDITOR_GIZMO_H
#define BH_EDITOR_GIZMO_H

#include "Game/GameConfig.h"

#if GAME_EDITOR_MODE == BH_ENABLED

#include "Core/CoreEssential.h"
#include "Core/FunctionsFramework.h"

#include "Platform/Mesh.h"
#include "Platform/Vector4f.h"
#include "Platform/Matrix4.h"

#include "Editor/SelectedInfo.h"

namespace BH
{
	class EditorSystem;

	class EditorGizmo
	{
	public:
		enum GIZMO_AXIS
		{
			AXIS_X = 0,
			AXIS_Y,
			AXIS_Z,

			AXIS_TOTAL,
		};

		enum GIZMO_MODE
		{
			GIZMO_MOVE = 0,
			GIZMO_ROTATE,
			GIZMO_SCALE,

			GIZMO_MODE_NUM,
		};

		static const f32 GIZMO_DEFAULT_SCALE;

	public:
		GIZMO_MODE mMode;

		GETSET_AUTO( GIZMO_MODE, Mode );

	public:
		typedef std::array<Matrix4, AXIS_TOTAL> AxisMatrixArray;

		GETTER_AUTO_PTR( SelectedInfo, Parent );
		void SetParent( SelectedInfo * parent );

		GETTER_AUTO_CREF( Vector3f, GizScale );
		Vector3f GetPosition() const;

		GETTER_AUTO_REF( f32, SnapValue );
		GETTER_AUTO_REF( bool, IsSnap );
		inline s32 GetAxisPicked() const { return mAxisPicked; }

	public:
		// Constructor
		EditorGizmo();

		// Destructor
		virtual ~EditorGizmo();

		// Draw
		void Draw( f32 dt );

		bool PickGizmo();

		void UnpickGizmo();

		void IncreaseScale( f32 step );

		void DecreaseScale( f32 step );

	private:
		// Helper Functions
		void CreateGizmoMesh();

		// Get scene position from screen position
		bool GetScenePosFromScreen( const Vector2f & screenPos, Vector2f & scenePos ) const;

		// Get click info
		bool GetClickInfo( Vector3f & clickNear, Vector3f & clickFar );

		// Get click info
		bool GetViewClickInfo( Vector3f & clickView );

		// Click point in world coord
		Vector3f GetCurrentClickPoint( bool alongX, bool alongY, bool alongZ );

		// Check for picking for rotation mode
		s32 RotGizmoPick3D();

		// Check for picking for normal mode
		s32 GizmoPick3D();

	private:
		SelectedInfo * mParent;
		Mesh mAxis;
		Mesh mArrowHead;
		Mesh mBoxHead;
		Mesh mCircle;
		Vector4f mColors[AXIS_TOTAL];

		AxisMatrixArray mRotations;
		AxisMatrixArray mTransforms;
		AxisMatrixArray mCircleRotations;

		s32 mAxisPicked;

		Vector3f mPrevClickPoint;
		Vector3f mCurrClickPoint;

		Vector3f mOldValue;
		Vector3f mGizScale;

		bool mIsSnap;
		f32 mSnapValue;

		friend class EditorSystem;
	};
}
#endif

#endif