#include "Editor/EditorEditBox.h"
#include "Editor/EditorWindow.h"

#if GAME_EDITOR_MODE

// Dependicies
#include "CEGUI/System.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/InputEvent.h"
#include "CEGUI/CEGUI.h"

namespace BH
{
	EditorEditBox::EditorEditBox( const String & name, const String & label, CEGUI::Window * root, bool readOnly )
	: mEditBox( static_cast<CEGUI::Editbox *>( CEGUI::WindowManager::getSingleton().createWindow( "Vanilla/Editbox", (name + " Editbox").c_str() ) ) )
	, mLabel( CEGUI::WindowManager::getSingleton().createWindow( "Vanilla/StaticText", name.c_str() ) )
	, mHLayout( static_cast<CEGUI::HorizontalLayoutContainer *>( CEGUI::WindowManager::getSingleton().createWindow( "HorizontalLayoutContainer" ) ) )
	, mRoot( root )
	, mRefreshCallback()
	, mReadOnly( readOnly )
	{
		// H Layout
		//mHLayout->setSize( CEGUI::USize( cegui_reldim( 1.0f ), cegui_absdim( 40.0f ) ) );
		mHLayout->setMouseInputPropagationEnabled( true );
		mHLayout->setMargin( CEGUI::UBox( CEGUI::UDim( 0.0f, 0.0f ), CEGUI::UDim( 0.0f, 0.0f ), 
										  CEGUI::UDim( 0.0f, 0.0f ), CEGUI::UDim( 0.0f, 0.0f ) ) );
		root->addChild( mHLayout );

		// Label
		mHLayout->addChild( mLabel );
		mLabel->setProperty( "FrameEnabled", "false" );
		mLabel->setProperty( "BackgroundEnabled", "false" );
		mLabel->setSize( CEGUI::USize( cegui_reldim( 1.0f - EditorUIManager::StandardSize.x - 2.0f * EditorUIManager::IndentGap ), cegui_reldim( EditorUIManager::StandardSize.y ) ) );
		mLabel->setText( label.c_str() );
		mLabel->setMargin( CEGUI::UBox( cegui_reldim( 0.005f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ) ) );

		// Editbox
		mHLayout->addChild( mEditBox );
		mEditBox->setSize( CEGUI::USize( cegui_reldim( EditorUIManager::StandardSize.x ), cegui_reldim( EditorUIManager::StandardSize.y ) ) );
		mEditBox->setMargin( CEGUI::UBox( cegui_reldim( 0.005f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ) ) );
		mEditBox->setReadOnly( readOnly );
		// mEditBox->setValidationString( "\\d*" ); // <- int
		// mEditBox->setValidationString(".*");		// <- string & float
	}

	EditorEditBox::~EditorEditBox()
	{
		mHLayout->removeChild( mEditBox );
		mHLayout->removeChild( mLabel );
		mRoot->removeChild( mHLayout );

		CEGUI::WindowManager::getSingleton().destroyWindow( mEditBox );
		CEGUI::WindowManager::getSingleton().destroyWindow( mLabel );
		CEGUI::WindowManager::getSingleton().destroyWindow( mHLayout );
	}

	void EditorEditBox::AddRefreshCallback( const RefreshCallback & refreshCallback )
	{
		mRefreshCallback = refreshCallback;
	}
}

#endif