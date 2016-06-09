#include "Editor/EditorButton.h"
#include "Editor/EditorUIManager.h"

#if GAME_EDITOR_MODE

// Dependicies
#include "CEGUI/System.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/InputEvent.h"
#include "CEGUI/CEGUI.h"
#include "CEGUI/widgets/PushButton.h"

namespace BH
{
	EditorButton::EditorButton( const String & name, const String & label, const ClickCallback & callback, CEGUI::Window * root )
	: mButton( static_cast<CEGUI::PushButton*>( CEGUI::WindowManager::getSingleton().createWindow( "Vanilla/Button", ( name + "Button" ).c_str() ) ) )
	, mRoot( root )
	, mClickCallback( callback )
	{
		// @hard code
		f32 buttonSize = label.size() > 15 ? std::min( EditorUIManager::StandardSize.x + ( label.size() - 15 ) * 0.025f, 1.0f ) : EditorUIManager::StandardSize.x;
		mButton->setSize( CEGUI::USize( cegui_reldim( buttonSize ), cegui_reldim( EditorUIManager::StandardSize.y ) ) );
		mButton->setHorizontalAlignment( CEGUI::HA_CENTRE );
		mButton->subscribeEvent( CEGUI::PushButton::EventClicked,
								 CEGUI::Event::Subscriber( &EditorButton::CallClickCallback, this ) );
		mButton->setText( label.c_str() );
		mButton->setMargin( CEGUI::UBox( cegui_reldim( 0.005f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ) ) );

		mRoot->addChild( mButton );
	}

	EditorButton::~EditorButton()
	{
		mRoot->removeChild( mButton );
		CEGUI::WindowManager::getSingleton().destroyWindow( mButton );
	}

	bool EditorButton::CallClickCallback( const CEGUI::EventArgs & )
	{
		if ( mClickCallback )
			mClickCallback();

		return true;
	}

	void EditorButton::SetClickCallback( const ClickCallback & callback )
	{
		mClickCallback = callback;

	}
}

#endif