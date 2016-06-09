// Precompiled Header
#include "Core/Stdafx.h"

#include "Core/InputManager.h"
#include "Core/ObjectAllocator.h"
#include "Core/Game.h"

namespace BH
{
	InputManager::InputManager()
	: mKeyEvents( Input::Count, InputEventList( Key::Count, nullptr ) )
	, mMouseEvents( Input::Count, MouseEventList( Mouse::Count, nullptr ) )
	, mMouseDblClickEvents( Mouse::Count, nullptr )
	{
		mKeyPressed.fill( false );
		mPrevKeyPressed.fill( false );
		mMousePressed.fill( false );
		mPrevMousePressed.fill( false );
	}

	InputManager::~InputManager()
	{

	}

	void InputManager::Initialise()
	{
		LOG("Initialising Input Manager...");
		for ( auto & elem : mKeyEvents )
		{
			for ( u32 i = 0; i < Key::Count; ++i )
				elem[i] = OBJECT_ALLOCATOR(InputEvent).New();
		}

		for ( auto & elem : mMouseEvents )
		{
			for ( u32 i = 0; i < Mouse::Count; ++i )
				elem[i] = OBJECT_ALLOCATOR(MouseEvent).New();
		}

		for ( u32 i = 0; i < Mouse::Count; ++i )
			mMouseDblClickEvents[i] = OBJECT_ALLOCATOR(MouseEvent).New();

		// Add to window callback
		GAME.Window.GetEventList().mOnWindowKeyDown = std::bind( &InputManager::OnKeyDownCallback, this, 
																 std::placeholders::_1 );
		GAME.Window.GetEventList().mOnWindowKeyUp = std::bind( &InputManager::OnKeyUpCallback, this, 
															   std::placeholders::_1 );
		GAME.Window.GetEventList().mOnWindowMouseDown = std::bind( &InputManager::OnMouseDownCallback, this, 
																   std::placeholders::_1,
																   std::placeholders::_2, 
																   std::placeholders::_3 );
		GAME.Window.GetEventList().mOnWindowMouseUp = std::bind( &InputManager::OnMouseUpCallback, this, 
																 std::placeholders::_1,
																 std::placeholders::_2, 
																 std::placeholders::_3 );
		GAME.Window.GetEventList().mOnWindowMouseDoubleClick = std::bind( &InputManager::OnMouseDoubleClickCallback, this, 
																		  std::placeholders::_1,
																		  std::placeholders::_2, 
																		  std::placeholders::_3 );
		GAME.Window.GetEventList().mOnWindowMouseMove = std::bind( &MouseEvent::Raise, &mMouseMoveEvent,
																   std::placeholders::_1,
																   std::placeholders::_2 );
		GAME.Window.GetEventList().mOnWindowMouseWheel = std::bind( &MouseWheelEvent::Raise, &mMouseWheelEvent,
																	std::placeholders::_1 );

		mMouseMoveEvent.Register( MouseEvent::Method< E_METHOD( InputManager, MouseMoveCallback ) >( this ) );

		LOG("Input Manager Initialised");
	}

	void InputManager::Shutdown()
	{
		mMouseMoveEvent.Unregister( MouseEvent::Method< E_METHOD( InputManager, MouseMoveCallback ) >( this ) );

		//Remove from window callback
		GAME.Window.GetEventList().mOnWindowKeyDown = WindowEvents::WindowKeyDownEvent();
		GAME.Window.GetEventList().mOnWindowKeyUp = WindowEvents::WindowKeyUpEvent();
		GAME.Window.GetEventList().mOnWindowMouseDown = WindowEvents::WindowMouseDownEvent();
		GAME.Window.GetEventList().mOnWindowMouseUp = WindowEvents::WindowMouseUpEvent();
		GAME.Window.GetEventList().mOnWindowMouseDoubleClick = WindowEvents::WindowMouseDblClickEvent();
		GAME.Window.GetEventList().mOnWindowMouseMove = std::function< void( s32, s32 ) >();
		GAME.Window.GetEventList().mOnWindowMouseWheel = std::function< void( s32 ) >();

		for ( auto & elem : mKeyEvents )
		{
			for ( u32 i = 0; i < Key::Count; ++i )
			{
				OBJECT_ALLOCATOR(InputEvent).Delete( elem[i] );
				elem[i] = nullptr;
			}
		}

		for ( auto & elem : mMouseEvents )
		{
			for ( u32 i = 0; i < Mouse::Count; ++i )
			{
				OBJECT_ALLOCATOR(MouseEvent).Delete( elem[i] );
				elem[i] = nullptr;
			}
		}

		for ( u32 i = 0; i < Mouse::Count; ++i )
		{
			OBJECT_ALLOCATOR(MouseEvent).Delete( mMouseDblClickEvents[i] );
			mMouseDblClickEvents[i] = nullptr;
		}
	}

	/*
	void InputManager::Update( f32 )
	{
		// Copy to old values
		//std::copy( mKeyPressed.begin(), mKeyPressed.end(), mPrevKeyPressed.begin() );
		//std::copy( mMousePressed.begin(), mMousePressed.end(), mPrevMousePressed.begin() );
		//mKeyPressed.fill( false );
		//mMousePressed.fill( false );
	}
	*/

	void InputManager::AddKeyEvent( Key::KeyCode key, Input::Action action, const InputDelegate & idelegate )
	{
		mKeyEvents[action][key]->Register( idelegate );
	}

	void InputManager::RemoveKeyEvent( Key::KeyCode key, Input::Action action, const InputDelegate & idelegate )
	{
		mKeyEvents[action][key]->Unregister( idelegate );
	}

	void InputManager::AddMouseEvent( Mouse::Button button, Input::Action action, const MouseDelegate & mdelegate )
	{
		mMouseEvents[action][button]->Register( mdelegate );
	}

	void InputManager::RemoveMouseEvent( Mouse::Button button, Input::Action action, const MouseDelegate & mdelegate )
	{
		mMouseEvents[action][button]->Unregister( mdelegate );
	}

	void InputManager::AddMouseDoubleClickEvent( Mouse::Button button, const MouseDelegate & mdelegate )
	{
		mMouseDblClickEvents[button]->Register( mdelegate );
	}
	
	void InputManager::RemoveMouseDoubleClickEvent( Mouse::Button button, const MouseDelegate & mdelegate )
	{
		mMouseDblClickEvents[button]->Unregister(mdelegate);
	}

	void InputManager::AddMouseMoveEvent( const MouseDelegate & mdelegate )
	{
		mMouseMoveEvent.Register( mdelegate );
	}
	
	void InputManager::RemoveMouseMoveEvent( const MouseDelegate & mdelegate )
	{
		mMouseMoveEvent.Unregister( mdelegate );
	}
	
	void InputManager::AddMouseWheelEvent( const MouseWheelDelegate & mdelegate )
	{
		mMouseWheelEvent.Register( mdelegate );
	}
	
	void InputManager::RemoveMouseWheelEvent( const MouseWheelDelegate & mdelegate )
	{
		mMouseWheelEvent.Unregister( mdelegate );
	}

	void InputManager::OnKeyDownCallback( Key::KeyCode code )
	{
		mKeyPressed[code] = true;

		if ( !mPrevKeyPressed[code] )
		{
			mKeyEvents[Input::Triggered][code]->Raise();
			mKeyTriggeredEvent.Raise( code );
		}

		mKeyEvents[Input::Pressed][code]->Raise();

		// Events for APIs
		mKeyDownEvent.Raise( code );

		mPrevKeyPressed[code] = true;
	}
	
	void InputManager::OnKeyUpCallback( Key::KeyCode code )
	{
		mKeyPressed[code] = false;
		mPrevKeyPressed[code] = false;

		//if ( mPrevKeyPressed[code] )
		mKeyEvents[Input::Released][code]->Raise();

		// Events for APIs
		mKeyUpEvent.Raise( code );
	}
	
	void InputManager::OnMouseDownCallback( s32 x, s32 y, Mouse::Button button )
	{
		mMousePressed[button] = true;

		if ( !mPrevMousePressed[button] )
		{
			mMouseEvents[Input::Triggered][button]->Raise( x, y );
			mMouseTriggeredEvent.Raise( x, y, button );
		}

		mMouseEvents[Input::Pressed][button]->Raise( x, y );

		// Events for APIs
		mMouseDownEvent.Raise( x, y,button );

		mPrevMousePressed[button] = true;
	}
	
	void InputManager::OnMouseUpCallback( s32 x, s32 y, Mouse::Button button )
	{
		mMousePressed[button] = false;
		mPrevMousePressed[button] = false;

		mMouseEvents[Input::Released][button]->Raise( x, y );

		// Events for APIs
		mMouseUpEvent.Raise( x, y,button );
	}

	void InputManager::OnMouseDoubleClickCallback( s32 x, s32 y, Mouse::Button button )
	{
		mMouseDblClickEvents[button]->Raise( x, y );
	}

	void InputManager::AddKeyDownEvent( const KeyCodeDelegate & kdelegate )
	{
		mKeyDownEvent.Register( kdelegate );
	}

	void InputManager::AddKeyUpEvent( const KeyCodeDelegate & kdelegate )
	{
		mKeyUpEvent.Register( kdelegate );
	}

	void InputManager::AddKeyTriggeredEvent( const KeyCodeDelegate & kdelegate )
	{
		mKeyTriggeredEvent.Register( kdelegate );
	}

	void InputManager::AddMouseDownEvent( const MouseCodeDelegate & mdelegate )
	{
		mMouseDownEvent.Register( mdelegate );
	}

	void InputManager::AddMouseUpEvent( const MouseCodeDelegate & mdelegate )
	{
		mMouseUpEvent.Register( mdelegate );
	}

	void InputManager::AddMouseTriggeredEvent( const MouseCodeDelegate & mdelegate )
	{
		mMouseTriggeredEvent.Register( mdelegate );
	}

	void InputManager::RemoveKeyDownEvent( const KeyCodeDelegate & kdelegate )
	{
		mKeyDownEvent.Unregister( kdelegate );
	}

	void InputManager::RemoveKeyUpEvent( const KeyCodeDelegate & kdelegate )
	{
		mKeyUpEvent.Unregister( kdelegate );
	}

	void InputManager::RemoveKeyTriggeredEvent( const KeyCodeDelegate & kdelegate )
	{
		mKeyTriggeredEvent.Unregister( kdelegate );
	}

	void InputManager::RemoveMouseDownEvent( const MouseCodeDelegate & mdelegate )
	{
		mMouseDownEvent.Unregister( mdelegate );
	}

	void InputManager::RemoveMouseUpEvent( const MouseCodeDelegate & mdelegate )
	{
		mMouseUpEvent.Unregister( mdelegate );
	}

	void InputManager::RemoveMouseTriggeredEvent( const MouseCodeDelegate & mdelegate )
	{
		mMouseTriggeredEvent.Unregister( mdelegate );
	}

	void InputManager::MouseMoveCallback( s32 x, s32 y )
	{
		mMousePosition.x = static_cast<f32>( x );
		mMousePosition.y = static_cast<f32>( y );
	}
}