#include "Editor/EditorListBox.h"
#include "Editor/EditorUIManager.h"

#include "Core/Event.h"

#if GAME_EDITOR_MODE

// Dependicies
#include "CEGUI/System.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/InputEvent.h"
#include "CEGUI/CEGUI.h"

namespace BH
{
	class EditorListItem : public CEGUI::ListboxTextItem
	{
	public:
		EditorListItem( const String & text, CEGUI::uint item_id = 0 )
		: CEGUI::ListboxTextItem( text, item_id )
		{
			setSelectionBrushImage( "Vanilla-Images/GenericBrush" );
		}
	};

	EditorListBox::EditorListBox( const String & name, const String & label, String * var, CEGUI::Window * root, bool readOnly )
	: mListBox( static_cast<CEGUI::Listbox *>( CEGUI::WindowManager::getSingleton().createWindow( "Vanilla/Listbox", ( name + " Listbox" ).c_str() ) ) )
	, mRoot( root )
	, mVar( var )
	, mReadOnly( readOnly )
	{
		mListBox->setText( label.c_str() );
		mListBox->setSize( CEGUI::USize( cegui_reldim( 1.0f ), cegui_reldim( EditorUIManager::StandardSize.y ) ) );
		mListBox->setMargin( CEGUI::UBox( cegui_reldim( 0.005f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ) ) );
		root->addChild( mListBox );

		mListBox->setSize( CEGUI::USize( cegui_reldim( 1.0f ), 
										 cegui_reldim( 10.0f * EditorUIManager::StandardSize.y ) ) );

		mListBox->subscribeEvent( CEGUI::Listbox::EventSelectionChanged, 
								  CEGUI::Event::Subscriber( &EditorListBox::SetValueEvent,
															this ) );

		mListBox->subscribeEvent( CEGUI::Listbox::EventMouseDoubleClick, 
								  CEGUI::Event::Subscriber( &EditorListBox::DoubleClickEvent,
															this ) );

		mListBox->setSortingEnabled( true );
	}
	
	EditorListBox::~EditorListBox()
	{
		mListBox->removeEvent( CEGUI::Listbox::EventSelectionChanged );
		mListBox->removeEvent( CEGUI::Listbox::EventMouseDoubleClick );
		mRoot->removeChild( mListBox );

		CEGUI::WindowManager::getSingleton().destroyWindow( mListBox );
	}

	bool EditorListBox::SetValueEvent( const CEGUI::EventArgs & )
	{
		CEGUI::ListboxItem * item = mListBox->getFirstSelectedItem();

		if ( !item )
			return false;

		bool changed = ( mVar && *mVar != item->getText().c_str() );

		if ( mVar )
			*mVar = item->getText().c_str();

		if ( changed && mOnChangeCallback )
			mOnChangeCallback();

		return true;
	}

	bool EditorListBox::DoubleClickEvent( const CEGUI::EventArgs & )
	{
		CEGUI::ListboxItem * item = mListBox->getFirstSelectedItem();

		if ( item && mOnDblClickCallback )
			mOnDblClickCallback( item->getText().c_str() );

		return true;
	}

	void EditorListBox::SetOnChangeCallback( const OnChangeCallback & callback )
	{
		mOnChangeCallback = callback;
	}

	void EditorListBox::SetOnDblClickCallback( const OnDblClickCallback & callback )
	{
		mOnDblClickCallback = callback;
	}

	void EditorListBox::AddItem( const String & item )
	{
		mListBox->addItem( new EditorListItem( item, mListBox->getItemCount() ) );

		if ( mVar && item == *mVar )
		{
			mListBox->getListboxItemFromIndex( mListBox->getItemCount() - 1 )->setSelected( true );
			//mListBox->getEditbox()->setText( item.c_str() );
		}
	}

	void EditorListBox::ClearItems()
	{
		mListBox->resetList();
	}

	void EditorListBox::SetSelection( const String & item )
	{
		size_t size = mListBox->getItemCount();
		for ( size_t i = 0; i < size; ++i )
		{
			CEGUI::ListboxItem * listItem = mListBox->getListboxItemFromIndex( i );
			if( listItem && listItem->getText().c_str() == item )
				mListBox->setItemSelectState( listItem, true );
		}
	}

	void EditorListBox::Unselect()
	{
		while( CEGUI::ListboxItem * item = mListBox->getFirstSelectedItem() )
			mListBox->setItemSelectState( item, false );

		if ( mVar )
			*mVar = "";
	}
}

#endif