#ifndef BH_MATERIAL_EDITOR_CAMERA_H
#define BH_MATERIAL_EDITOR_CAMERA_H

#include "Core/Event.h"
#include "Core/CoreEssential.h"
#include "Core/FunctionsFramework.h"
#include "Core/IUpdatable.h"
#include "Core/GameComponent.h"
#include "Core/SystemManager.h"

#include "Platform/Camera.h"
#include "Platform/Vector2f.h"
#include "Platform/Vector3f.h"

namespace BH
{
	class MaterialEditorCamera : public GameComponent, public IUpdatable
	{
	public:
		// Set debug camera to active or non-active
		void SetEditorCameraActive( bool active );

	public:
		// Constructor
		MaterialEditorCamera();

		// Destructor
		~MaterialEditorCamera();

		// Initialise
		void Initialise() override;

		// Shutdown
		void Shutdown() override;

		// Update
		void Update( f32 dt ) override;

	private:
		// Input callbacks
		void ClickTriggerClickCallback( s32 x, s32 y );

		void ClickReleaseClickCallback( s32 x, s32 y );

		void ScrollCallback( s32 x );

		void MouseMoveCallback( s32 x, s32 y );

	private:
		Camera mEditorCamera;		//!< Editor camera
		Camera * mPrevCamera;		//!< Editor camera
		Vector2f mPreviousMousePos;	//!< Previous Position
		Vector2f mCurrentMousePos;	//!< Current Position
		bool mActive;				//!< Is debug camera active
		bool mClicked;				//!< Is there mouse input for camera
		bool mScrolled;				//!< Is there scrolling motion

	};
}

#include "Core/Typing.h"
DECLARE_TYPE(::BH::MaterialEditorCamera, MaterialEditorCamera);

#endif