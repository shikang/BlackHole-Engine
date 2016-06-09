#include "Platform/Stdafx.h"
#include "Platform/PlatformWindow.h"

namespace BH
{
	const unsigned int WINDOW_WIDTH_OFFSET = 16;
	const unsigned int WINDOW_HEIGHT_OFFSET = 39;

	PlatformWindow::PlatformWindow( ) :
		PlatformWindow( WindowSettings() )
	{
	}

	PlatformWindow::PlatformWindow( const WindowSettings & gs ) :
		mSettings( gs )
	{
		SetTitle( mSettings.title );
		RegisterWindow( );
	}

	PlatformWindow::~PlatformWindow( )
	{
		CloseWindow( );
	}

	/////////////////////////////////////////////////////////////////////////////

	void PlatformWindow::SetTitle( const String & name )
	{
		mSettings.title = name;

		sTitle.resize( name.length( ), L' ' );
		std::copy( name.begin( ), name.end( ), sTitle.begin( ) );

		if ( mHwnd != nullptr )
			SetWindowText( mHwnd, mSettings.title.c_str( ) );
	}

	const String & PlatformWindow::GetTitle( ) const
	{
		return mSettings.title;
	}

	// @TODO : fullscreen functionality not finalized
	void PlatformWindow::SetFullscreen( bool state )
	{
		mFullscreen = state;
	}

	bool PlatformWindow::GetFullscreen( ) const
	{
		return mFullscreen;
	}

	void PlatformWindow::SetPosition( u32 x, u32 y )
	{
		mSettings.x = x;
		mSettings.y = y;
	}

	u32 PlatformWindow::GetX( ) const
	{
		return mSettings.x;
	}

	u32 PlatformWindow::GetY( ) const
	{
		return mSettings.y;
	}

	u32  PlatformWindow::GetWidth() const
	{
		return mSettings.w;
	}

	u32  PlatformWindow::GetHeight() const
	{
		return mSettings.h;
	}

	bool PlatformWindow::IsCursorIn( ) const
	{
		return mIsCursorIn;
	}

	WindowEvents & PlatformWindow::GetEventList( )
	{
		return mEventList;
	}

	WindowSettings & PlatformWindow::GetSettings( )
	{
		return mSettings;
	}

	/////////////////////////////////////////////////////////////////////////////

	void PlatformWindow::OpenWindow( )
	{
		mHwnd = CreateWindowW(  sTitle.c_str( ),		// name of our registered class
							    sTitle.c_str( ),		// Window name/title
								mSettings.flags,				// Style flags
								mSettings.x,					// X position
								mSettings.y,					// Y position
								mSettings.w + WINDOW_WIDTH_OFFSET,					// width of window
								mSettings.h + WINDOW_HEIGHT_OFFSET,					// height of window
								NULL,							// Parent window
								NULL,							// Menu
								mInstance,						// application instance handle
								this );							// pointer to window-creation data

		mCurrentCursor = LoadCursor( NULL, IDC_ARROW );
		ShowWindow( mHwnd, SW_SHOW );
		ShowCursor( true );

		if ( mHwnd == nullptr )
		{
			CreateMessageBox( "Create Window failed." );
		}

	}

	void PlatformWindow::CloseWindow( )
	{
		if ( mHwnd != nullptr )
			DestroyWindow( mHwnd );

		mHwnd = nullptr;
	}

	void PlatformWindow::Update( )
	{
		ProcessWindowEvents( );
	}

	/////////////////////////////////////////////////////////////////////////////

	void PlatformWindow::RegisterWindow( )
	{
		WNDCLASSW windowClass;

		windowClass.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
		windowClass.hInstance = GetModuleHandle( NULL );
		windowClass.lpszClassName = sTitle.c_str( );
		windowClass.lpfnWndProc = &PlatformWindow::WinProc;
		
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hbrBackground = 0;
		windowClass.hIcon = NULL;
		windowClass.hCursor = NULL;
		windowClass.lpszMenuName = NULL;

		if ( RegisterClassW( &windowClass ) == false )
		{
			//CreateMessageBox( "Register Window failed." );

		}
	}

	/////////////////////////////////////////////////////////////////////////////

	void PlatformWindow::ProcessWindowEvents( )
	{
		if ( mHwnd == nullptr )
			return;

		MSG msg;

		while ( PeekMessage( &msg, mHwnd, 0, 0, PM_REMOVE ) > 0 )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	void PlatformWindow::ProcessEvents( u32 msg, WPARAM wParam, LPARAM lParam )
	{
		UNREFERENCED_PARAMETER( wParam );
		UNREFERENCED_PARAMETER( lParam );

		if ( mHwnd == nullptr )
			return;

		switch ( msg )
		{
			// Window close event.
		case WM_CLOSE:
		{
			if ( mEventList.mOnWindowClosed )
				mEventList.mOnWindowClosed( );

			CloseWindow();

			break;
		}

		// Set cursor event
		case WM_SETCURSOR:
		{
			break;
		}

		// Window gain focus event.
		case WM_SETFOCUS:
		{
			if ( mEventList.mOnWindowGainedFocus )
				mEventList.mOnWindowGainedFocus( );

			break;
		}

		// Window lost focus event.
		case WM_KILLFOCUS:
		{
			if ( mEventList.mOnWindowLostFocus )
				mEventList.mOnWindowLostFocus( );

			break;
		}

		// Sent after a window has been moved
		case WM_MOVE:
		{
			if ( mIsCursorIn == false )
			{
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof( TRACKMOUSEEVENT );
				tme.hwndTrack = mHwnd;
				tme.dwFlags = TME_LEAVE;
				TrackMouseEvent( &tme );

				mIsCursorIn = true;
				if ( mEventList.mOnWindowMouseEnter )
					mEventList.mOnWindowMouseEnter( );
			}

			if ( mEventList.mOnWindowMouseMove )
				mEventList.mOnWindowMouseMove( LOWORD( lParam ), HIWORD( lParam ) );

			break;
		}

		// Text event.
		case WM_CHAR:
		{
			if ( mEventList.mOnWindowKeyChar )
				mEventList.mOnWindowKeyChar( u32( wParam ) );

			break;
		}

		// Key down event.
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			Key::KeyCode key = ConvertVirtualCode( wParam, lParam );

			if ( mEventList.mOnWindowKeyDown )
				mEventList.mOnWindowKeyDown( key );

			break;
		}

		// Key up event.
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			Key::KeyCode key = ConvertVirtualCode( wParam, lParam );

			if ( mEventList.mOnWindowKeyUp )
				mEventList.mOnWindowKeyUp( key );

			break;
		}

		// Mouse wheel event.
		case WM_MOUSEWHEEL:
		{
			if ( mEventList.mOnWindowMouseWheel )
				mEventList.mOnWindowMouseWheel( static_cast< s16 >( HIWORD( wParam ) ) / 120 );

			break;
		}

		// Mouse move event, pretty cool stuff
		case WM_MOUSEMOVE:
		{
			if ( mEventList.mOnWindowMouseMove )
				mEventList.mOnWindowMouseMove( LOWORD( lParam ), HIWORD( lParam ) );

			break;
		}

		// Left mouse button down event.
		case WM_LBUTTONDOWN:
		{
			if ( mEventList.mOnWindowMouseDown )
				mEventList.mOnWindowMouseDown( LOWORD( lParam ), HIWORD( lParam ), Mouse::Left );

			break;
		}

		// Left mouse button up event.
		case WM_LBUTTONUP:
		{
			if ( mEventList.mOnWindowMouseUp )
				mEventList.mOnWindowMouseUp( LOWORD( lParam ), HIWORD( lParam ), Mouse::Left );

			break;
		}

		// Left mouse button double click event.
		case WM_LBUTTONDBLCLK:
		{
			if ( mEventList.mOnWindowMouseDoubleClick )
				mEventList.mOnWindowMouseDoubleClick( LOWORD( lParam ), HIWORD( lParam ), Mouse::Left );

			break;
		}

		// Right mouse button down event.
		case WM_RBUTTONDOWN:
		{
			if ( mEventList.mOnWindowMouseDown )
				mEventList.mOnWindowMouseDown( LOWORD( lParam ), HIWORD( lParam ), Mouse::Right );

			break;
		}

		// Right mouse button up event.
		case WM_RBUTTONUP:
		{
			if ( mEventList.mOnWindowMouseUp )
				mEventList.mOnWindowMouseUp( LOWORD( lParam ), HIWORD( lParam ), Mouse::Right );

			break;
		}

		// Right mouse button double click event.
		case WM_RBUTTONDBLCLK:
		{
			if ( mEventList.mOnWindowMouseDoubleClick )
				mEventList.mOnWindowMouseDoubleClick( LOWORD( lParam ), HIWORD( lParam ), Mouse::Right );

			break;
		}

		// Middle mouse button down event.
		case WM_MBUTTONDOWN:
		{
			if ( mEventList.mOnWindowMouseDown )
				mEventList.mOnWindowMouseDown( LOWORD( lParam ), HIWORD( lParam ), Mouse::Middle );

			break;
		}

		// Middle mouse button up event.
		case WM_MBUTTONUP:
		{
			if ( mEventList.mOnWindowMouseUp )
				mEventList.mOnWindowMouseUp( LOWORD( lParam ), HIWORD( lParam ), Mouse::Middle );

			break;
		}

		// Middle mouse button double click event.
		case WM_MBUTTONDBLCLK:
		{
			if ( mEventList.mOnWindowMouseDoubleClick )
				mEventList.mOnWindowMouseDoubleClick( LOWORD( lParam ), HIWORD( lParam ), Mouse::Middle );

			break;
		}

		// Posted to a window when the cursor leaves the client area of the window
		case WM_MOUSELEAVE:
		{
			if ( mEventList.mOnWindowMouseLeave )
				mEventList.mOnWindowMouseLeave( );

			mIsCursorIn = false;

			break;
		}

		// When window resize
		case WM_SIZE:
		{
			if( wParam != SIZE_MINIMIZED && mHwnd )
			{
				RECT rect;
				GetClientRect( mHwnd, &rect );
				mSettings.w = rect.right - rect.left;
				mSettings.h = rect.bottom - rect.top;
				break;
			}
		}

		}
	}

	/////////////////////////////////////////////////////////////////////////////

	LRESULT CALLBACK PlatformWindow::WinProc( HWND handle, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		// associate handle and window instance when the creation message is received
		if ( msg == WM_CREATE )
		{
			// Get PlatformWindow instance 
			// last argument of CreateWindow
			long This = reinterpret_cast< long >( reinterpret_cast< CREATESTRUCT* >( lParam )->lpCreateParams );

			// Set as the "user data" parameter of the window
			SetWindowLongPtr( handle, GWLP_USERDATA, This );
		}

		// Get the PlatformWindow instance corresponding to the window handle
		PlatformWindow * window = reinterpret_cast< PlatformWindow * >( GetWindowLongPtr( handle, GWLP_USERDATA ) );

		// Forward the event to the appropriate function
		if ( window )
			window->ProcessEvents( msg, wParam, lParam );

		// We don't forward the WM_CLOSE message to prevent the OS from automatically destroying the window
		switch ( msg )
		{
		case WM_CLOSE:
			return 0;
		}

		return DefWindowProcW( handle, msg, wParam, lParam );

	}

}