#ifndef BH_UI_MANAGER_H
#define BH_UI_MANAGER_H

#include "Platform/Typedefs.h"
#include "Platform/PlatformWindow.h"

#include "Core/Singleton.h"
#include "Core/Uncopyable.h"
#include "Core/Event.h"

#include "Editor/EditorWindow.h"

#include "Game/GameConfig.h"

#if GAME_EDITOR_MODE

// forward declarations
#include "CEGUI/ForwardRefs.h"

namespace CEGUI
{
	class Direct3D11Renderer;
	class ImageCodec;
	class ResourceProvider;
	class GeometryBuffer;
	class EventArgs;
	class GUIContext;
}

#include <unordered_map>

namespace BH
{
	// Forward declaration
	class Renderer;

	class EditorUIManager
	{
	public:
		typedef std::unordered_map<String, EditorWindow *> WindowList;
		typedef Event< void() > RefreshEvent;
		typedef RefreshEvent::Delegate RefreshDelegate;

		DECLARE_SINGLETON(EditorUIManager);

	public:
		// Destructor
		~EditorUIManager();

		// Initialise
		void Initialise( Renderer * renderer );

		// Shutdown
		void Shutdown();

		// Draw
		void Draw( f32 dt );

		// Create Editor Window
		EditorWindow * CreateEditorWindow( const String & name, const Vector2f & position = Vector2f::ZERO,
										   const Vector2f & size = Vector2f( 0.1f, 0.2f ) );

		// Destory window
		void DestroyEditorWindow( EditorWindow * window );

		// Destory window
		void DestroyEditorWindow( const String & name );

		// Add Refresh Callback
		void AddRefreshEvent( const RefreshDelegate & rdelegate );

		// Remove refresh callback
		void RemoveRefreshEvent( const RefreshDelegate & rdelegate );

	private:
		// UI Initialisation Helper function

		// Initialise Resource Group Directories
		void InitialiseResourceGroupDirectories();

		// Initialise Default Resource Groups
		void InitialiseDefaultResourceGroups();

		// Resize Event
		bool ResizeEvent(const CEGUI::EventArgs & args);

		// Inject inputs for CEGUI
		void InjectInputs();

		// Eject inputs for CEGUI
		void EjectInputs();

		CEGUI::ImageCodec *			mImageCodec;
		CEGUI::ResourceProvider *	mResourceProvider;

	private:
		// Inputs callbacks
		void InjectChar(u32 character);
		void InjectMouseWheelChange(s32 x);
		void InjectMousePosition(s32 x, s32 y);
		void InjectKeyDown(Key::KeyCode code);
		void InjectKeyUp(Key::KeyCode code);
		void InjectMouseDown(s32 x, s32 y, Mouse::Button button);
		void InjectMouseUp(s32 x, s32 y, Mouse::Button button);

		// Click outside window callback
		void ClickOutsideCallback( s32 x, s32 y );

	private:
		// Layout

		// Initialise editor layout
		void InitialiseLayoutResources();

		// Initialise UI Root
		void InitialiseRoot();

		// Finalise Initialisation (Switch to editor layout)
		void FinialiseInitialisation();

	private:
		// Constructor
		EditorUIManager();

	private:
		CEGUI::Direct3D11Renderer * mUIRenderer;

		CEGUI::Window *				mRoot;

		s32							mWindowWidth;
		s32							mWindowHeight;

		WindowList					mWindowList;

		f32							mCurrentRefreshTimer;
		RefreshEvent				mRefreshEvent;

	public:
		static f32					RefreshRate;			//!< In seconds
		static Vector2f				StandardSize;			//!< Standard UI size
		static f32					IndentGap;				//!< Indent gap when new group is created
		static const Char *			GroupLabelPad;			//!< Group label pad

	};
}

#define EDITOR_UI\
	::BH::EditorUIManager::Instance()

#endif	// GAME_EDITOR_MODE

#endif