#include "Editor/EditorUIManager.h"
#include "Editor/EditorInputMapping.h"
#include "Editor/EditorComboBox.h"

#include "Core/Color.h"
#include "Core/CoreEssential.h"
#include "Core/Hash.h"
#include "Core/Game.h"
#include "Core/SystemManager.h"
#include "Core/Renderer.h"
#include "Core/System.h"
#include "Core/InputManager.h"

#if GAME_EDITOR_MODE

// Dependicies
#include "CEGUI/RendererModules/Direct3D11/Renderer.h"
#include "CEGUI/System.h"
#include "CEGUI/DefaultResourceProvider.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/Image.h"
#include "CEGUI/Font.h"
#include "CEGUI/Scheme.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/falagard/WidgetLookManager.h"
#include "CEGUI/ScriptModule.h"
#include "CEGUI/XMLParser.h"
#include "CEGUI/AnimationManager.h"
#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/GUIContext.h"
#include "CEGUI/RenderTarget.h"
#include "CEGUI/InputEvent.h"
#include "CEGUI/CEGUI.h"
#include "CEGUI/Logger.h"
#include "CEGUI/CommonDialogs/Module.h"
#include "CEGUI/Logger.h"

#include <algorithm>

#define TEST_EDITOR_UI 0
namespace BH
{
#if TEST_EDITOR_UI
	// @test
	static f32 fval = 0.0f;
	static u32 uval = 1;
	static s32 ival = 2;
	static String sval = "Four";
	static String sval2 = "Five";
	static String sval3 = "Six";
	static bool bval = false;
	static Color cval(1.0f,1.0f, 1.0f, 1.0f);

	void ResetStatic()
	{
		fval = 0.0f;
		uval = 1;
		ival = 2;
		sval = "Four";
		sval2 = "Five";
		bval = false;
	}
#endif

	f32	EditorUIManager::RefreshRate = 1.0f;
	Vector2f EditorUIManager::StandardSize( 0.5f, 0.04f );
	f32 EditorUIManager::IndentGap = 0.025f;
	const Char * EditorUIManager::GroupLabelPad = " ";

	class NoLogger : public CEGUI::Logger
	{
		void logEvent( const CEGUI::String&, CEGUI::LoggingLevel )
		{

		}
		void setLogFilename( const CEGUI::String&, bool )
		{

		}
	};

	NoLogger nL;

	EditorUIManager::EditorUIManager()
	: mUIRenderer( nullptr )
	, mImageCodec( nullptr )
	, mResourceProvider( nullptr )
	, mCurrentRefreshTimer( 0.0f ) {}

	EditorUIManager::~EditorUIManager() {}

	void EditorUIManager::Initialise( Renderer * renderer )
	{
		// Common Dialogs
		initialiseCEGUICommonDialogs();

		// Init UI Renderer
		mUIRenderer = &CEGUI::Direct3D11Renderer::create( renderer->GetPipeline()->GetDevice(),
														  renderer->GetPipeline()->GetContext() );

		// Initialise CEGUI System
		CEGUI::System::create( *mUIRenderer, mResourceProvider, nullptr, mImageCodec );

		// Initialise Resource System
		InitialiseResourceGroupDirectories();
		InitialiseDefaultResourceGroups();

		// Subscribe handler to reposition logo when window is sized.
		CEGUI::System::getSingleton().subscribeEvent(
			CEGUI::System::EventDisplaySizeChanged,
			CEGUI::Event::Subscriber( &EditorUIManager::ResizeEvent,
									  this ) );

		const CEGUI::Rectf& area( CEGUI::System::getSingleton().getRenderer()->
								  getDefaultRenderTarget().getArea() );

		mWindowWidth = static_cast<s32>( area.getWidth() );
		mWindowHeight = static_cast<s32>( area.getHeight() );
		CEGUI::System::getSingleton().notifyDisplaySizeChanged( CEGUI::Sizef( area.getWidth(), area.getHeight() ) );

		// Inject inputs callbacks for CEGUI
		InjectInputs();

		InitialiseLayoutResources();
		InitialiseRoot();
		FinialiseInitialisation();

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddMouseEvent( Mouse::Left, Input::Triggered, 
																				  Delegate<void( s32, s32 )>::FromMethod<E_METHOD(EditorUIManager, ClickOutsideCallback)>( this ) );

		CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setVisible( false );

#if TEST_EDITOR_UI
		// @test
		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::F1, Input::Triggered, Delegate<void()>::FromFunction<&ResetStatic>() );
#endif
	}

	void EditorUIManager::Shutdown()
	{
		EjectInputs();

		CEGUI::System::destroy();

		CEGUI::Direct3D11Renderer::destroy(*mUIRenderer);

		for ( auto & i : mWindowList )
		{
			delete i.second;
		}
		mWindowList.clear();

#if TEST_EDITOR_UI
		// @test
		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::F1, Input::Triggered, Delegate<void()>::FromFunction<&ResetStatic>() );
#endif
	}

	void EditorUIManager::Draw( f32 dt )
	{	
		// Refresh
		mCurrentRefreshTimer += dt;
		if ( mCurrentRefreshTimer >= RefreshRate )
		{
			mRefreshEvent.Raise();
			mCurrentRefreshTimer = 0.0f;
		}

		// Draw
		CEGUI::GUIContext & defaultGUIContext =
			CEGUI::System::getSingleton().getDefaultGUIContext();
		defaultGUIContext.injectTimePulse( dt );

		CEGUI::System::getSingleton().renderAllGUIContexts();
	}

	void EditorUIManager::InitialiseResourceGroupDirectories()
	{
		// Initialise the required dirs for the DefaultResourceProvider
		CEGUI::DefaultResourceProvider* rp =
			static_cast<CEGUI::DefaultResourceProvider*>
			( CEGUI::System::getSingleton().getResourceProvider() );

		const char* dataPathPrefix = "../resources/CEGUI";
		char resourcePath[_MAX_PATH];

		// For each resource type, set a resource group directory
		sprintf(resourcePath, "%s/%s", dataPathPrefix, "schemes/");
		rp->setResourceGroupDirectory("schemes", resourcePath);
		sprintf(resourcePath, "%s/%s", dataPathPrefix, "imagesets/");
		rp->setResourceGroupDirectory("imagesets", resourcePath);
		sprintf(resourcePath, "%s/%s", dataPathPrefix, "fonts/");
		rp->setResourceGroupDirectory("fonts", resourcePath);
		sprintf(resourcePath, "%s/%s", dataPathPrefix, "layouts/");
		rp->setResourceGroupDirectory("layouts", resourcePath);
		sprintf(resourcePath, "%s/%s", dataPathPrefix, "looknfeel/");
		rp->setResourceGroupDirectory("looknfeels", resourcePath);
		sprintf(resourcePath, "%s/%s", dataPathPrefix, "lua_scripts/");
		rp->setResourceGroupDirectory("lua_scripts", resourcePath);
		sprintf(resourcePath, "%s/%s", dataPathPrefix, "xml_schemas/");
		rp->setResourceGroupDirectory("schemas", resourcePath);
		sprintf(resourcePath, "%s/%s", dataPathPrefix, "animations/");
		rp->setResourceGroupDirectory("animations", resourcePath);
	}

	void EditorUIManager::InitialiseDefaultResourceGroups()
	{
		// Set the default resource groups to be used
		CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
		CEGUI::Font::setDefaultResourceGroup("fonts");
		CEGUI::Scheme::setDefaultResourceGroup("schemes");
		CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
		CEGUI::WindowManager::setDefaultResourceGroup("layouts");
		CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
		CEGUI::AnimationManager::setDefaultResourceGroup("animations");

		// setup default group for validation schemas
		CEGUI::XMLParser* parser = CEGUI::System::getSingleton().getXMLParser();
		if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
			parser->setProperty("SchemaDefaultResourceGroup", "schemas");
	}

	bool EditorUIManager::ResizeEvent(const CEGUI::EventArgs &)
	{
		const CEGUI::Rectf& area( CEGUI::System::getSingleton().getRenderer()->
								  getDefaultRenderTarget().getArea() );
		mWindowWidth = static_cast<s32>( area.getWidth() );
		mWindowHeight = static_cast<s32>( area.getHeight() );

		return true;
	}

	void EditorUIManager::InitialiseLayoutResources()
	{
		//CEGUI::Font& font =
		//CEGUI::FontManager::getSingleton().
		//	createFromFile( "Jura-10.font" );
		CEGUI::Font& font =
		CEGUI::FontManager::getSingleton().
			createFromFile( "Verdana8.font" );

		CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont(&font);

		CEGUI::SchemeManager::getSingleton().createFromFile( "TaharezLook.scheme" );
		CEGUI::SchemeManager::getSingleton().createFromFile( "VanillaSkin.scheme" );
		CEGUI::SchemeManager::getSingleton().createFromFile( "VanillaCommonDialogs.scheme" );
		CEGUI::SchemeManager::getSingleton().createFromFile( "WindowsLook.scheme" );
	}

	void EditorUIManager::InitialiseRoot()
	{
		CEGUI::WindowManager & winMgr( CEGUI::WindowManager::getSingleton() );

		// Here we will use a StaticImage as the root, then we can use it to place a background image
		mRoot = winMgr.createWindow( "Vanilla/StaticImage", "RootWindow" );
		// Set position and size
		mRoot->setPosition( CEGUI::UVector2( cegui_reldim( 0 ), cegui_reldim( 0 ) ) );
		mRoot->setSize( CEGUI::USize( cegui_reldim( 1 ), cegui_reldim( 1 ) ) );
		// Disable frame and standard background
		mRoot->setProperty( "FrameEnabled", "false" );
		mRoot->setProperty( "BackgroundEnabled", "false" );
		// Set the background image
		//mRoot->setProperty( "Image", "SpaceBackgroundImage" );

#if TEST_EDITOR_UI
		// @test: ------------------- Start -----------------------
		EditorWindow * w = CreateEditorWindow( "TestWindow", Vector2f( 0.8f, 0.0f ), Vector2f( 0.2f, 1.0f ) );
		w->AddEditBox( "Test1", "Main", "Test", &fval, false );
		w->AddEditBox( "Test2", "TestTab", "Test", &uval, true );
		w->AddGroup( "TestGroup1", "Main" );
		w->AddGroup( "TestGroup2", "TestTab" );
		w->AddEditBoxToGroup( "Test3", "TestGroup1", "TestGroupEntry1", &ival, false );
		w->AddEditBoxToGroup( "Test4", "TestGroup1", "TestGroupEntry2", &sval, false );
		w->AddEditBoxToGroup( "Test5", "TestGroup2", "TestGroupEntry1", &sval2, false );
		w->AddCheckBoxToGroup( "Test6", "TestGroup2", "TestGroupEntry2", &bval, false );
		EditorComboBox * combobox = w->AddComboBox( "Test7", "Main", "TestComboBox", &sval3, false );
		combobox->AddItem( "One" );
		combobox->AddItem( "Two" );
		combobox->AddItem( "Three" );
		combobox->AddItem( "Four" );
		combobox->AddItem( "Five" );
		combobox->AddItem( "Six" );
		w->AddColourPicker( "Test7", "TestTab", "TestColourPicker", &cval, false );
		w->AddGroup("TestGroup3", "Main");
		w->AddCheckBoxToGroup( "Test8", "TestGroup3", "TestRemoveGroup", &bval, false );
		w->RemoveCheckBox( "Test8" );
		// @test: ------------------- End -----------------------
#endif
	}

	void EditorUIManager::FinialiseInitialisation()
	{
		CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().
			setDefaultImage( "Vanilla-Images/MouseArrow" );

		CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow( mRoot );
	}

	void EditorUIManager::InjectInputs()
	{
		GAME.Window.GetEventList().mOnWindowKeyChar = std::bind( &EditorUIManager::InjectChar, this, std::placeholders::_1 );
		InputManager * inputManager = SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>();
		inputManager->AddMouseMoveEvent( Delegate< void( s32, s32 ) >::FromMethod<E_METHOD(EditorUIManager, InjectMousePosition)>( this ) );
		inputManager->AddMouseWheelEvent( Delegate< void( s32 ) >::FromMethod<E_METHOD(EditorUIManager, InjectMouseWheelChange)>( this ) );
		inputManager->AddKeyDownEvent( Delegate< void( Key::KeyCode ) >::FromMethod<E_METHOD(EditorUIManager, InjectKeyDown)>( this ) );
		inputManager->AddKeyUpEvent( Delegate< void( Key::KeyCode ) >::FromMethod<E_METHOD(EditorUIManager, InjectKeyUp)>( this ) );
		inputManager->AddMouseDownEvent( Delegate< void( s32, s32, Mouse::Button ) >::FromMethod<E_METHOD(EditorUIManager, InjectMouseDown)>( this ) );
		inputManager->AddMouseUpEvent( Delegate< void( s32, s32, Mouse::Button ) >::FromMethod<E_METHOD(EditorUIManager, InjectMouseUp)>( this ) );
	}

	void EditorUIManager::EjectInputs()
	{
		GAME.Window.GetEventList().mOnWindowKeyChar = std::function< void( u32 ) >();
		InputManager * inputManager = SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>();
		inputManager->RemoveMouseMoveEvent( Delegate< void( s32, s32 ) >::FromMethod<E_METHOD(EditorUIManager, InjectMousePosition)>( this ) );
		inputManager->RemoveMouseWheelEvent( Delegate< void( s32 ) >::FromMethod<E_METHOD(EditorUIManager, InjectMouseWheelChange)>( this ) );
		inputManager->RemoveKeyDownEvent( Delegate< void( Key::KeyCode ) >::FromMethod<E_METHOD(EditorUIManager, InjectKeyDown)>( this ) );
		inputManager->RemoveKeyUpEvent( Delegate< void( Key::KeyCode ) >::FromMethod<E_METHOD(EditorUIManager, InjectKeyUp)>( this ) );
		inputManager->RemoveMouseDownEvent( Delegate< void( s32, s32, Mouse::Button ) >::FromMethod<E_METHOD(EditorUIManager, InjectMouseDown)>( this ) );
		inputManager->RemoveMouseUpEvent( Delegate< void( s32, s32, Mouse::Button ) >::FromMethod<E_METHOD(EditorUIManager, InjectMouseUp)>( this ) );
	}

	void EditorUIManager::InjectChar( u32 character )
	{
		if ( CEGUI::System* ceguiSystem = CEGUI::System::getSingletonPtr() )
			ceguiSystem->getDefaultGUIContext().injectChar( character );
	}

	void EditorUIManager::InjectMouseWheelChange( s32 x )
	{
		// Magic number by demo
		if ( CEGUI::System* ceguiSystem = CEGUI::System::getSingletonPtr() )
			ceguiSystem->getDefaultGUIContext().injectMouseWheelChange( x / 120.0f );
	}

	void EditorUIManager::InjectMousePosition( s32 x, s32 y )
	{
		if ( CEGUI::System* ceguiSystem = CEGUI::System::getSingletonPtr() )
		{
			f32 mouseX = static_cast<f32>( x ) * mWindowWidth / GAME.Window.GetWidth();
			f32 mouseY = static_cast<f32>( y ) * mWindowHeight / GAME.Window.GetHeight();
			ceguiSystem->getDefaultGUIContext().injectMousePosition( static_cast<f32>( mouseX ), static_cast<f32>( mouseY ) );
		}
	}

	void EditorUIManager::InjectKeyDown( Key::KeyCode code )
	{
		if ( CEGUI::System* ceguiSystem = CEGUI::System::getSingletonPtr() )
		{
			ceguiSystem->getDefaultGUIContext().injectKeyDown( EditorInputMapping::KeyMappingTable[code] );
		}
	}

	void EditorUIManager::InjectKeyUp( Key::KeyCode code )
	{
		if ( CEGUI::System* ceguiSystem = CEGUI::System::getSingletonPtr() )
			ceguiSystem->getDefaultGUIContext().injectKeyUp( EditorInputMapping::KeyMappingTable[code] );
	}

	void EditorUIManager::InjectMouseDown( s32, s32, Mouse::Button button )
	{
		if ( CEGUI::System* ceguiSystem = CEGUI::System::getSingletonPtr() )
			ceguiSystem->getDefaultGUIContext().
				injectMouseButtonDown( EditorInputMapping::MouseMappingTable[button] );
	}

	void EditorUIManager::InjectMouseUp( s32, s32, Mouse::Button button )
	{
		if ( CEGUI::System* ceguiSystem = CEGUI::System::getSingletonPtr() )
			ceguiSystem->getDefaultGUIContext().
				injectMouseButtonUp( EditorInputMapping::MouseMappingTable[button] );
	}

	EditorWindow * EditorUIManager::CreateEditorWindow( const String & name, const Vector2f & position,
														const Vector2f & size )
	{
		// Window exist, ignore
		if ( mWindowList.find( name ) != mWindowList.end() )
			return nullptr;

		EditorWindow * window = new EditorWindow( name, position,size, mRoot );
		mWindowList.insert( std::make_pair( name, window ) );
		return window;
	}
	
	void EditorUIManager::DestroyEditorWindow( EditorWindow * window )
	{
		WindowList::iterator it = std::find_if( mWindowList.begin(), mWindowList.end(),
												[=](const WindowList::value_type & p)
												{
													return p.second == window;
												} );
		if ( it != mWindowList.end() )
		{
			delete it->second;
			mWindowList.erase( it );
		}
	}
	
	void EditorUIManager::DestroyEditorWindow( const String & name )
	{
		WindowList::iterator it = mWindowList.find( name );
		if ( it != mWindowList.end() )
		{
			delete it->second;
			mWindowList.erase( it );
		}
	}

	void EditorUIManager::AddRefreshEvent( const RefreshDelegate & rdelegate )
	{
		mRefreshEvent.Register( rdelegate );
	}
	
	void EditorUIManager::RemoveRefreshEvent( const RefreshDelegate & rdelegate )
	{
		mRefreshEvent.Unregister( rdelegate );
	}

	void EditorUIManager::ClickOutsideCallback( s32, s32 )
	{
		CEGUI::Window* mouseOverWin = CEGUI::System::getSingleton().getDefaultGUIContext().getWindowContainingMouse();
		if ( mouseOverWin == nullptr || mouseOverWin == mRoot )
		{
			for ( auto & i : mWindowList )
				i.second->DeactivateWindow();
		}
	}
}

#endif