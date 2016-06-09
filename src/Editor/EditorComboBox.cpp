#include "Editor/EditorComboBox.h"
#include "Editor/EditorUIManager.h"

#if GAME_EDITOR_MODE

// Dependicies
#include "CEGUI/System.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/InputEvent.h"
#include "CEGUI/CEGUI.h"
#include "CEGUI/widgets/Combobox.h"
#include "CEGUI/widgets/ListboxTextItem.h"
#include "CEGUI/Event.h"

#include <algorithm>

namespace BH
{
	class EditorListItem : public CEGUI::ListboxTextItem
	{
	public:
		EditorListItem( const String& text, CEGUI::uint item_id = 0 )
		: ListboxTextItem( text, item_id )
		{
			setSelectionBrushImage( "Vanilla-Images/GenericBrush" );
		}
	};

	EditorComboBox::EditorComboBox( const String & name, const String & label, s32 * id, String * text, CEGUI::Window * root, bool readOnly )
	: mHLayout( static_cast<CEGUI::HorizontalLayoutContainer *>( CEGUI::WindowManager::getSingleton().createWindow( "HorizontalLayoutContainer" ) ) )
	, mBox( static_cast<CEGUI::Combobox *>( CEGUI::WindowManager::getSingleton().createWindow( "Vanilla/Combobox", (name + " Combobox").c_str() ) ) )
	, mLabel( CEGUI::WindowManager::getSingleton().createWindow( "Vanilla/StaticText", name.c_str() ) )
	, mRoot( root )
	, mID( id )
	, mText( text )
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

		// Combo Box
		mHLayout->addChild( mBox );
		mBox->setSize( CEGUI::USize( cegui_reldim( EditorUIManager::StandardSize.x ), cegui_reldim( EditorUIManager::StandardSize.y ) ) );
		mBox->setMargin( CEGUI::UBox( cegui_reldim( 0.005f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ) ) );

		if ( !readOnly )
		{
			mBox->subscribeEvent( CEGUI::Combobox::EventListSelectionAccepted,
								  CEGUI::Event::Subscriber( &EditorComboBox::SetValueEvent,
															this ) );
			mBox->subscribeEvent( CEGUI::Combobox::EventDropListDisplayed,
								  CEGUI::Event::Subscriber( &EditorComboBox::DropEvent,
															this ) );
			mBox->subscribeEvent( CEGUI::Combobox::EventDropListRemoved,
								  CEGUI::Event::Subscriber( &EditorComboBox::HideEvent,
															this ) );
		}

		mBox->setSortingEnabled( true );
		mBox->setReadOnly( readOnly );
	}

	EditorComboBox::~EditorComboBox()
	{
		if ( !mReadOnly )
		{
			mBox->removeEvent( CEGUI::Combobox::EventListSelectionAccepted );
			mBox->removeEvent( CEGUI::Combobox::EventDropListDisplayed );
			mBox->removeEvent( CEGUI::Combobox::EventDropListRemoved );
		}

		mHLayout->removeChild( mBox );
		mHLayout->removeChild( mLabel );
		mRoot->removeChild( mHLayout );

		CEGUI::WindowManager::getSingleton().destroyWindow( mBox );
		CEGUI::WindowManager::getSingleton().destroyWindow( mLabel );
		CEGUI::WindowManager::getSingleton().destroyWindow( mHLayout );
	}

	bool EditorComboBox::SetValueEvent( const CEGUI::EventArgs & )
	{
		CEGUI::ListboxItem * item = mBox->getSelectedItem();

		bool changed = ( mID && *mID != static_cast<s32>( item->getID() ) ) || ( mText && *mText != item->getText().c_str() );

		if ( mID )
			*mID = item->getID();

		if ( mText )
			*mText = item->getText().c_str();

		if ( changed && mOnChangeCallback )
			mOnChangeCallback();

		return true;
	}

	void EditorComboBox::SetOnChangeCallback( const OnChangeCallback & callback )
	{
		mOnChangeCallback = callback;
	}

	bool EditorComboBox::DropEvent( const CEGUI::EventArgs & )
	{
		mBox->setSize( CEGUI::USize( cegui_reldim( EditorUIManager::StandardSize.x ), 
									 cegui_reldim( std::min( mBox->getItemCount(), 10u ) * EditorUIManager::StandardSize.y ) ) );
		return true;
	}

	bool EditorComboBox::HideEvent( const CEGUI::EventArgs & )
	{
		mBox->setSize( CEGUI::USize( cegui_reldim( EditorUIManager::StandardSize.x ), cegui_reldim( EditorUIManager::StandardSize.y ) ) );
		return true;
	}

	void EditorComboBox::AddItem( const String & item, s32 id )
	{
		mBox->addItem( new EditorListItem( item, id ) );

		if ( ( mID && id == *mID ) || ( mText && item == *mText ) )
		{
			mBox->getListboxItemFromIndex( mBox->getItemCount() - 1 )->setSelected( true );
			mBox->getEditbox()->setText( item.c_str() );
		}
	}
	
	void EditorComboBox::AddItem( const String & item )
	{
		mBox->addItem( new EditorListItem( item, mBox->getItemCount() ) );

		if ( mText && item == *mText )
		{
			mBox->getListboxItemFromIndex( mBox->getItemCount() - 1 )->setSelected( true );
			mBox->getEditbox()->setText( item.c_str() );
		}
	}
}
#endif