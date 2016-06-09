#include "Editor/EditorCheckBox.h"
#include "Editor/EditorUIManager.h"

#include "Core/Event.h"

#if GAME_EDITOR_MODE

// Dependicies
#include "CEGUI/System.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/InputEvent.h"
#include "CEGUI/CEGUI.h"
#include "CEGUI/widgets/ToggleButton.h"

namespace BH
{
	EditorCheckBox::EditorCheckBox( const String & name, const String & label, bool * var, CEGUI::Window * root, bool readOnly )
	: mButton( static_cast<CEGUI::ToggleButton *>( CEGUI::WindowManager::getSingleton().createWindow( "Vanilla/Checkbox", ( name + " Checkbox" ).c_str() ) ) )
	, mRoot( root )
	, mVar( var )
	, mReadOnly( readOnly )
	{
		mButton->setText( label.c_str() );
		mButton->setSize( CEGUI::USize( cegui_reldim( 1.0f ), cegui_reldim( EditorUIManager::StandardSize.y ) ) );
		mButton->setMargin( CEGUI::UBox( cegui_reldim( 0.005f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ) ) );
		root->addChild( mButton );

		mButton->subscribeEvent( CEGUI::ToggleButton::EventSelectStateChanged, 
								 CEGUI::Event::Subscriber( &EditorCheckBox::SetValueEvent,
															this ) );

		EditorUIManager::Instance().AddRefreshEvent( Delegate<void()>::FromMethod<E_METHOD( EditorCheckBox, Refresh )>( this ) );
		Refresh();
	}
	
	EditorCheckBox::~EditorCheckBox()
	{
		mButton->removeEvent( CEGUI::ToggleButton::EventSelectStateChanged );
		mRoot->removeChild( mButton );

		EditorUIManager::Instance().RemoveRefreshEvent( Delegate<void()>::FromMethod<E_METHOD( EditorCheckBox, Refresh )>( this ) );

		CEGUI::WindowManager::getSingleton().destroyWindow( mButton );
	}

	bool EditorCheckBox::SetValueEvent( const CEGUI::EventArgs & )
	{
		if ( !mReadOnly )
			*mVar = mButton->isSelected();

		return true;
	}
	
	void EditorCheckBox::Refresh()
	{
		mButton->setSelected( *mVar );
	}
}

#endif