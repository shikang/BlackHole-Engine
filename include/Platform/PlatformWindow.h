#ifndef BH_WINDOW_H
#define BH_WINDOW_H

#include "Platform/Typedefs.h"
#include "Platform/Keybind.h"
#include "Platform/WindowSettings.h"

#include <windows.h>
#include <functional>

namespace BH
{
	struct WindowEvents
	{
	private:
		typedef std::function<void( )>									WindowEvent;
		typedef std::function<void( u32, u32, u32, u32, u16, Real ) >	WindowSizeEvent;
		typedef std::function<void( bool, bool )>						WindowFullscreenEvent;

		typedef std::function<void( u32 )>								WindowCharEvent;
		typedef std::function<void( Key::KeyCode )>						WindowKeyEvent;

		typedef std::function<void( s32, s32 )>							WindowMouseMoveEvent;
		typedef std::function<void( s32, s32, Mouse::Button )>			WindowMouseButtonEvent;
		typedef std::function<void( s32 )>								WindowMouseWheelEvent;

	public:
		typedef WindowEvent					WindowCreatedEvent;
		typedef WindowEvent					WindowDestroyedEvent;
		typedef WindowEvent					WindowClosedEvent;
		typedef WindowEvent					WindowLostFocusEvent;
		typedef WindowEvent					WindowGainedFocusEvent;
		typedef WindowEvent					WindowMouseEnterEvent;
		typedef WindowEvent					WindowMouseLeaveEvent;
		typedef WindowMouseButtonEvent		WindowMouseUpEvent;
		typedef WindowMouseButtonEvent		WindowMouseDownEvent;
		typedef WindowMouseButtonEvent		WindowMouseDblClickEvent;
		typedef WindowKeyEvent				WindowKeyUpEvent;
		typedef WindowKeyEvent				WindowKeyDownEvent;

	public:
		WindowSizeEvent						mOnWindowResized;				// Triggered when window is resized.
		WindowCreatedEvent					mOnWindowCreated;				// Triggered when window is created.
		WindowDestroyedEvent				mOnWindowDestroyed;				// Triggered when window is destroyed.
		WindowClosedEvent					mOnWindowClosed;				// Triggered when window is closed.
		WindowLostFocusEvent				mOnWindowLostFocus;				// Triggered when window is loses focus.
		WindowGainedFocusEvent				mOnWindowGainedFocus;			// Triggered when window is gains focus.
		WindowFullscreenEvent				mOnWindowFullscreen;			// Triggered when window toggles fullscreen mode.

		WindowKeyDownEvent					mOnWindowKeyDown;				// Triggered when a key is pressed down.
		WindowKeyUpEvent					mOnWindowKeyUp;					// Triggered when a key is released.
		WindowCharEvent						mOnWindowKeyChar;				// Triggered when a key character is pressed.

		WindowMouseMoveEvent				mOnWindowMouseMove;				// Triggered when mouse is moved.
		WindowMouseDblClickEvent			mOnWindowMouseDoubleClick;		// Trigger when mouse is double clicked.
		WindowMouseDownEvent				mOnWindowMouseDown;				// Triggered when mouse button is pressed down.
		WindowMouseUpEvent					mOnWindowMouseUp;				// Triggered when mouse button is released.
		WindowMouseWheelEvent				mOnWindowMouseWheel;			// Triggered when mouse wheel is moved.

		WindowMouseEnterEvent				mOnWindowMouseEnter;			// Triggered when mouse enters the window.
		WindowMouseLeaveEvent				mOnWindowMouseLeave;			// Triggered when mouse leaves the window.
	};

	class PlatformWindow
	{
	public:
		PlatformWindow( );
		PlatformWindow( const WindowSettings & gs );
		~PlatformWindow( );

		void SetTitle( const String & name );
		const String & GetTitle( ) const;

		void SetFullscreen( bool state );
		bool GetFullscreen( ) const;

		void SetPosition( u32 x, u32 y );
		u32  GetX( ) const;
		u32  GetY( ) const;

		u32  GetWidth() const;
		u32  GetHeight() const;

		bool IsCursorIn( ) const;

		void OpenWindow( );
		void CloseWindow( );

		void Update( );

		WindowEvents & GetEventList( );
		WindowSettings & GetSettings( );

	private:
		void RegisterWindow( );

		void ProcessWindowEvents( );
		void ProcessEvents( u32 msg, WPARAM wParam, LPARAM lParam );

		static LRESULT CALLBACK WinProc( HWND handle, UINT msg, WPARAM wParam, LPARAM lParam );

		// events
		WindowEvents	mEventList;

		// fullscreen
		bool			mFullscreen = false;

		// window attributes
		WindowSettings	mSettings = { };

		// cursor
		bool			mIsCursorIn = true;
		HCURSOR			mCurrentCursor = nullptr;
		HICON			mIcon = nullptr;

		// windows data
		HINSTANCE		mInstance = GetModuleHandle( NULL );
		HWND			mHwnd = nullptr;

		// helper
		// std::wstring	sWindowClassNameW;
		std::wstring	sTitle;

		friend class Graphics;
	};

}

#endif