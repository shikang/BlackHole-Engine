#include "Editor/EditorDivider.h"
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
	EditorDivider::EditorDivider( const String & name, CEGUI::Window * root )
	: mDivider( CEGUI::WindowManager::getSingleton().createWindow( "Vanilla/StaticText", name.c_str() ) )
	, mRoot( root )
	{
		// @hard code
		mDivider->setSize( CEGUI::USize( cegui_reldim( 1.0f ), cegui_reldim( EditorUIManager::StandardSize.y ) ) );
		mDivider->setText( "_________________________________" );
		mDivider->setMargin( CEGUI::UBox( cegui_reldim( 0.005f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ) ) );
		mDivider->setProperty( "FrameEnabled", "false" );
		mDivider->setProperty( "BackgroundEnabled", "false" );
		mDivider->setProperty( "TextColours", "tl:FF808080 tr:FF808080 bl:FF808080 br:FF808080" );

		mRoot->addChild( mDivider );
	}

	EditorDivider::~EditorDivider()
	{
		mRoot->removeChild( mDivider );
		CEGUI::WindowManager::getSingleton().destroyWindow( mDivider );
	}
}

#endif