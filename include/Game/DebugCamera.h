#ifndef BH_DEBUG_CAMERA_H
#define BH_DEBUG_CAMERA_H

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
	class DebugCamera : public GameComponent, public IUpdatable
	{
	public:
		// Set debug camera to active or non-active, do we use current view?
		void SetDebugCameraActive( bool active, bool currentView );

	public:
		// Constructor
		DebugCamera();

		// Destructor
		~DebugCamera();

		// Initialise
		void Initialise() override;

		// Shutdown
		void Shutdown() override;

		// Update
		void Update( f32 dt ) override;

		// Focus on
		void FocusOn( const Matrix4 & world, const AABB & aabb );

	private:
		// Input callbacks
		void RightTriggerClickCallback( s32 x, s32 y );

		void RightReleaseClickCallback( s32 x, s32 y );

		void ScrollCallback( s32 x );

		void MouseMoveCallback( s32 x, s32 y );

		void MiddleTriggerClickCallback( s32 x, s32 y );

		void MiddleReleasedCallback( s32 x, s32 y );

		void WTriggeredCallback();

		void WReleaseCallback();

		void ATriggeredCallback();

		void AReleaseCallback();

		void STriggeredCallback();

		void SReleaseCallback();

		void DTriggeredCallback();

		void DReleaseCallback();

	private:
		Camera mDebugCamera;		//!< Debug camera
		Camera * mPrevCamera;		//!< Previous camera
		Vector2f mPreviousMousePos;	//!< Previous Position
		Vector2f mCurrentMousePos;	//!< Current Position
		bool mActive;				//!< Is debug camera active
		bool mClicked;				//!< Is there mouse input for camera
		bool mScrolled;				//!< Is there scrolling motion
		bool mRight;				//!< Is Alt key pressed
		bool mW;					//!< Is W pressed
		bool mA;					//!< Is A pressed
		bool mS;					//!< Is S pressed
		bool mD;					//!< Is D pressed
		f32 mPressedTime;			//!< Pressed time

	};
}

#include "Core/Typing.h"
DECLARE_TYPE(::BH::DebugCamera, DebugCamera);

#endif