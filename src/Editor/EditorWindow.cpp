#include "Editor/EditorWindow.h"
#include "Editor/EditorCheckBox.h"
#include "Editor/EditorComboBox.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorDivider.h"

#if GAME_EDITOR_MODE

// Dependicies
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
#include "CEGUI/widgets/TabControl.h"

namespace BH
{
	EditorWindow::EditorWindow( const String & name, const Vector2f & position,
								const Vector2f & size, CEGUI::Window * root )
	: mWindow( static_cast<CEGUI::FrameWindow *>( CEGUI::WindowManager::getSingleton().createWindow( "Vanilla/FrameWindow", name.c_str() ) ) )
	, mTabControl( static_cast<CEGUI::TabControl *>( CEGUI::WindowManager::getSingleton().createWindow( "TaharezLook/TabControl", ( name + "TabControl" ).c_str() ) ) )
	, mRoot( root )
	{
		root->addChild( mWindow );

		// Window
		mWindow->setPosition( CEGUI::UVector2( cegui_reldim( position.x ), cegui_reldim( position.y ) ) );
		mWindow->setMaxSize( CEGUI::USize( cegui_reldim( 1.0f ), cegui_reldim( 1.0f ) ) );
		mWindow->setSize( CEGUI::USize( cegui_reldim( size.x ), cegui_reldim( size.y ) ) );
		mWindow->setText( name.c_str() );
		mWindow->setDragMovingEnabled( false );
		mWindow->subscribeEvent( CEGUI::FrameWindow::EventDragSizingEnded,
								 CEGUI::Event::Subscriber( &EditorWindow::OnResized,
														   this ) );

		// Tab
		mTabControl->setSize( CEGUI::USize( cegui_reldim( 1.0f ), cegui_reldim( 1.0f ) ) );
		mTabControl->subscribeEvent( CEGUI::TabControl::EventSelectionChanged,
									 CEGUI::Event::Subscriber( &EditorWindow::TabOnChanged,
															   this ) );
		mWindow->addChild( mTabControl );
	}
	
	EditorWindow::~EditorWindow()
	{
		// Clear edit box
		for ( auto & i : mEditBoxes )
			delete i.second;
		mEditBoxes.clear();

		// Clear buttons
		for ( auto & i : mButtons )
			delete i.second;
		mButtons.clear();

		// Clear check boxes
		for ( auto & i : mCheckBoxes )
			delete i.second;
		mCheckBoxes.clear();

		// Clear combo boxes
		for ( auto & i : mComboBoxes )
			delete i.second;
		mComboBoxes.clear();

		// Clear colour pickers
		for ( auto & i : mColourPickers )
			delete i.second;
		mColourPickers.clear();

		// Clear colour pickers
		for ( auto & i : mDividers )
			delete i.second;
		mDividers.clear();

		// Clear groups
		for ( auto & i : mGroups )
		{
			CEGUI::WindowManager::getSingleton().destroyWindow( i.second );
		}

		// Clear tabs
		for ( auto & i : mTabs )
		{
			i.second.first->removeChild( i.second.second );
			CEGUI::WindowManager::getSingleton().destroyWindow( i.second.second );
			CEGUI::WindowManager::getSingleton().destroyWindow( i.second.first );
		}

		mWindow->removeEvent( CEGUI::FrameWindow::EventDragSizingEnded );
		mTabControl->removeEvent( CEGUI::TabControl::EventSelectionChanged );

		// Clear main window stuff
		mWindow->removeChild( mTabControl );
		mRoot->removeChild( mWindow );

		CEGUI::WindowManager::getSingleton().destroyWindow( mTabControl );
		CEGUI::WindowManager::getSingleton().destroyWindow( mWindow );
	}

	void EditorWindow::DeactivateWindow()
	{
		mWindow->deactivate();
	}

	void EditorWindow::RemoveEmptyTab()
	{
		TabList::iterator it = mTabs.begin();

		while ( it != mTabs.end() )
		{
			// If tab has no element, remove
			if ( it->second.second->getChildCount() <= 1 )
			{
				mTabControl->removeChild( it->second.first );
				it->second.first->removeChild( it->second.second );
				CEGUI::WindowManager::getSingleton().destroyWindow( it->second.second );
				CEGUI::WindowManager::getSingleton().destroyWindow( it->second.first );
				it = mTabs.erase( it );
			}
			else
			{
				++it;
			}
		}
	}

	void EditorWindow::DestoryChild( CEGUI::Window * child )
	{
		while ( child->getChildCount() != 0 )
		{
			CEGUI::Window * childchild = child->getChildAtIdx( 0 );
			child->removeChild( childchild );

			DestoryChild( childchild );
		}

		CEGUI::WindowManager::getSingleton().destroyWindow( child );
	}

	void EditorWindow::DestoryParent( CEGUI::Window * parent )
	{
		if ( parent->getChildCount() == 1 )
		{
			CEGUI::Window * parentparent = parent->getParent();
			parentparent->removeChild( parent );

			CEGUI::WindowManager::getSingleton().destroyWindow( parent );
			DestoryParent( parentparent );
		}
	}

	void EditorWindow::RemoveEmptyGroup()
	{
		GroupList::iterator it = mGroups.begin();

		while ( it != mGroups.end() )
		{
			// If tab has no element, remove
			if ( it->second->getChildCount() <= 1 )	// 1 for group label
			{
				//CEGUI::Window * child = it->second->getChildAtIdx( 0 );
				//it->second->removeChild( child );
				//DestoryChild( child );
				
				if( CEGUI::Window * parent = it->second->getParent() )
					parent->removeChild( it->second );

				CEGUI::WindowManager::getSingleton().destroyWindow( it->second );
				it = mGroups.erase( it );
			}
			else
			{
				++it;
			}
		}
	}

	void EditorWindow::RemoveEditBox( const String & name )
	{
		// If name does not exist, nothing to do here
		EditBoxList::iterator it = mEditBoxes.find( name );
		if ( it == mEditBoxes.end() )
			return;

		//std::cout << "[Removing Edit Box]: " << name << std::endl;

		CEGUI::Window * parent = it->second->mRoot;

		// Remove entry from map, so same name can be added again
		delete it->second;
		mEditBoxes.erase( it );

		// Clean up tabs and group
		DestoryParent( parent );
		RemoveEmptyGroup();
		RemoveEmptyTab();
	}

	void EditorWindow::AddGroup( const String & group_name, const String & tab_name )
	{
		// Vertical layout for other stuff
		CEGUI::VerticalLayoutContainer * vLayout = static_cast<CEGUI::VerticalLayoutContainer *>( CEGUI::WindowManager::getSingleton().createWindow( "VerticalLayoutContainer", 
																																					 ( tab_name + " " + group_name + " Vert Layout" ).c_str() ) );
		vLayout->setMouseInputPropagationEnabled( true );
		vLayout->setMargin( CEGUI::UBox ( cegui_reldim( 0.0f ), cegui_reldim( EditorUIManager::IndentGap ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ) ) );
		mGroups[group_name] = vLayout;

		// Add to tab
		// If tab does not exist, create
		TabList::iterator it = mTabs.find( tab_name );
		if ( it == mTabs.end() )
		{
			AddTab( tab_name );
		}
		mTabs[tab_name].second->addChild( vLayout );
		//mVLayout->addChild( vLayout );

		// Add group name label
		CEGUI::Window * label = CEGUI::WindowManager::getSingleton().createWindow( "Vanilla/StaticText", group_name.c_str() );
		vLayout->addChild( label );
		label->setProperty( "FrameEnabled", "false" );
		label->setProperty( "BackgroundEnabled", "true" );
		label->setSize( CEGUI::USize( cegui_reldim( 1.0f - 0.0f * EditorUIManager::IndentGap ), cegui_reldim( EditorUIManager::StandardSize.y ) ) );
		label->setMargin( CEGUI::UBox( cegui_reldim( 0.005f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ) ) );
		label->setText( ( EditorUIManager::GroupLabelPad + group_name ).c_str() );
	}

	void EditorWindow::AddGroupToGroup( const String & group_name, const String & group_group_name )
	{
		// If group does not exist, exit
		GroupList::iterator it = mGroups.find( group_group_name );
		if ( it == mGroups.end() )
			return;

		// Vertical layout for other stuff
		CEGUI::VerticalLayoutContainer * vLayout = static_cast<CEGUI::VerticalLayoutContainer *>( CEGUI::WindowManager::getSingleton().createWindow( "VerticalLayoutContainer",
																																					 ( group_name + " " + group_group_name + " Vert Layout" ).c_str() ) );
		vLayout->setMouseInputPropagationEnabled( true );
		vLayout->setMargin( CEGUI::UBox ( cegui_reldim( 0.0f ), cegui_reldim( EditorUIManager::IndentGap ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ) ) );
		mGroups[group_name] = vLayout;

		// Add to group
		it->second->addChild( vLayout );
		//mVLayout->addChild( vLayout );

		// Add group name label
		CEGUI::Window * label = CEGUI::WindowManager::getSingleton().createWindow( "Vanilla/StaticText", group_name.c_str() );
		vLayout->addChild( label );
		label->setProperty( "FrameEnabled", "false" );
		label->setProperty( "BackgroundEnabled", "true" );
		label->setSize( CEGUI::USize( cegui_reldim( 1.0f - 0.0f * EditorUIManager::IndentGap ), cegui_reldim( EditorUIManager::StandardSize.y ) ) );
		label->setMargin( CEGUI::UBox( cegui_reldim( 0.005f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ) ) );

		label->setText( ( EditorUIManager::GroupLabelPad + group_name ).c_str() );
	}

	void EditorWindow::AddTab( const String & tab_name )
	{
		CEGUI::ScrollablePane * tab = static_cast<CEGUI::ScrollablePane *>( CEGUI::WindowManager::getSingleton().createWindow( "TaharezLook/ScrollablePane",
																																( tab_name + "Scroll" ).c_str() ) );

		// Scrollable
		CEGUI::VerticalLayoutContainer * layout = static_cast<CEGUI::VerticalLayoutContainer *>( CEGUI::WindowManager::getSingleton().createWindow( "VerticalLayoutContainer",
																																					( tab_name + " Vert Layout" ).c_str() ) );
		tab->setText( tab_name.c_str() );
		tab->setArea( CEGUI::URect( CEGUI::UDim( 0, 0 ), CEGUI::UDim( 0, 0 ), CEGUI::UDim( 0.99f, 0 ), CEGUI::UDim( 0.99f, 0 ) ) );
		tab->setContentPaneAutoSized( true );
		mTabControl->addTab( tab );

		// Layout
		// Vertical layout
		layout->setMargin( CEGUI::UBox( cegui_reldim( 0.0f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ), cegui_reldim( 0.0f ) ) );
		layout->setMouseInputPropagationEnabled( true );
		tab->addChild( layout );

		mTabs.insert( std::make_pair( tab_name, Tab( tab, layout ) ) );
	}

	EditorCheckBox * EditorWindow::AddCheckBoxToWindow( const String & name, const String & label,
														CEGUI::VerticalLayoutContainer * vLayout,
														bool * var, bool readOnly )
	{
		// If name exist, do not add
		CheckBoxList::iterator it = mCheckBoxes.find( name );
		if ( it != mCheckBoxes.end() )
			return nullptr;

		EditorCheckBox * newBox = new EditorCheckBox( name, label, var, vLayout, readOnly );
		mCheckBoxes[name] = newBox;
		return newBox;
	}

	void EditorWindow::AddCheckBox( const String & name, const String & tab_name, const String & label, bool * var, bool readOnly )
	{
		//std::cout << "[Adding Checkbox to " << tab_name << "]: " << name << std::endl;
		//std::cout << "[Adding Checkbox]: " << name << std::endl;
		// If tab does not exist, create
		TabList::iterator it = mTabs.find( tab_name );
		if ( it == mTabs.end() )
			AddTab( tab_name );

		AddCheckBoxToWindow( name, label, mTabs[tab_name].second, var, readOnly );
	}
	
	void EditorWindow::AddCheckBoxToGroup( const String & name, const String & group_name, const String & label, bool * var, bool readOnly )
	{
		//if (group_name == "ScriptComponent"){
		//	std::cout << "[Adding Checkbox to " << group_name << "]: " << name << std::endl;
		//	PrintExistingUIInfo();
		//}
		//std::cout << "[Adding Checkbox to " << group_name << "]: " << name << std::endl;
		//std::cout << "[Adding Checkbox]: " << name << std::endl;
		// If group does not exist, exit
		GroupList::iterator it = mGroups.find( group_name );
		if ( it == mGroups.end() )
			return;

		// Add Edit box
		AddCheckBoxToWindow( name, label, mGroups[group_name], var, readOnly );
	}
	
	void EditorWindow::RemoveCheckBox( const String & name )
	{
		//std::cout << "[Removing Checkbox]: " << name << std::endl;
		// If name does not exist, nothing to do here
		CheckBoxList::iterator it = mCheckBoxes.find( name );
		if ( it == mCheckBoxes.end() )
			return;

		CEGUI::Window * parent = it->second->mRoot;

		// Remove entry from map, so same name can be added again
		delete it->second;
		mCheckBoxes.erase( it );

		// Clean up tabs and group
		DestoryParent( parent );
		RemoveEmptyGroup();
		RemoveEmptyTab();
	}

	EditorComboBox * EditorWindow::AddComboBoxToWindow( const String & name, const String & label,
													    CEGUI::VerticalLayoutContainer * vLayout,
													    s32 * id, String * text, bool readOnly )
	{
		// If name exist, do not add
		ComboBoxList::iterator it = mComboBoxes.find( name );
		if ( it != mComboBoxes.end() )
			return nullptr;

		EditorComboBox * newBox = new EditorComboBox( name, label, id, text, vLayout, readOnly );
		mComboBoxes[name] = newBox;
		return newBox;
	}

	EditorComboBox * EditorWindow::AddComboBox( const String & name, const String & tab_name, const String & label, s32 * id, bool readOnly )
	{
		// If tab does not exist, create
		TabList::iterator it = mTabs.find( tab_name );
		if ( it == mTabs.end() )
			AddTab( tab_name );

		return AddComboBoxToWindow( name, label, mTabs[tab_name].second, id, nullptr, readOnly );
	}
	
	EditorComboBox * EditorWindow::AddComboBox( const String & name, const String & tab_name, const String & label, String * text, bool readOnly )
	{
		// If tab does not exist, create
		TabList::iterator it = mTabs.find( tab_name );
		if ( it == mTabs.end() )
			AddTab( tab_name );

		return AddComboBoxToWindow( name, label, mTabs[tab_name].second, nullptr, text, readOnly );
	}
	
	EditorComboBox * EditorWindow::AddComboBoxToGroup( const String & name, const String & group_name, const String & label, s32 * id, bool readOnly )
	{
		// If group does not exist, create
		GroupList::iterator it = mGroups.find( group_name );
		if ( it == mGroups.end() )
			return nullptr;

		// Add Edit box
		return AddComboBoxToWindow( name, label, mGroups[group_name], id, nullptr, readOnly );
	}
	
	EditorComboBox * EditorWindow::AddComboBoxToGroup( const String & name, const String & group_name, const String & label, String * text, bool readOnly )
	{
		// If group does not exist, create
		GroupList::iterator it = mGroups.find( group_name );
		if ( it == mGroups.end() )
			return nullptr;

		// Add Edit box
		return AddComboBoxToWindow( name, label, mGroups[group_name], nullptr, text, readOnly );
	}
	
	void EditorWindow::RemoveComboBox( const String & name )
	{
		// If name does not exist, nothing to do here
		ComboBoxList::iterator it = mComboBoxes.find( name );
		if ( it == mComboBoxes.end() )
			return;

		CEGUI::Window * parent = it->second->mRoot;

		// Remove entry from map, so same name can be added again
		delete it->second;
		mComboBoxes.erase( it );

		// Clean up tabs and group
		DestoryParent( parent );
		RemoveEmptyGroup();
		RemoveEmptyTab();
	}

	EditorListBox * EditorWindow::AddListBoxToWindow( const String & name, const String & label,
													  CEGUI::VerticalLayoutContainer * vLayout,
													  String * text, bool readOnly )
	{
		// If name exist, do not add
		ListBoxList::iterator it = mListBoxes.find( name );
		if ( it != mListBoxes.end() )
			return nullptr;

		EditorListBox * newBox = new EditorListBox( name, label, text, vLayout, readOnly );
		mListBoxes[name] = newBox;
		return newBox;
	}
	
	EditorListBox * EditorWindow::AddListBox( const String & name, const String & tab_name, const String & label, String * text, bool readOnly )
	{
		// If tab does not exist, create
		TabList::iterator it = mTabs.find( tab_name );
		if ( it == mTabs.end() )
			AddTab( tab_name );

		return AddListBoxToWindow( name, label, mTabs[tab_name].second, text, readOnly );
	}
	
	EditorListBox * EditorWindow::AddListBoxToGroup( const String & name, const String & group_name, const String & label, String * text, bool readOnly )
	{
		// If group does not exist, create
		GroupList::iterator it = mGroups.find( group_name );
		if ( it == mGroups.end() )
			return nullptr;

		// Add Edit box
		return AddListBoxToWindow( name, label, mGroups[group_name], text, readOnly );
	}
	
	void EditorWindow::RemoveListBox( const String & name )
	{
		// If name does not exist, nothing to do here
		ListBoxList::iterator it = mListBoxes.find( name );
		if ( it == mListBoxes.end() )
			return;

		CEGUI::Window * parent = it->second->mRoot;

		// Remove entry from map, so same name can be added again
		delete it->second;
		mListBoxes.erase( it );

		// Clean up tabs and group
		DestoryParent( parent );
		RemoveEmptyGroup();
		RemoveEmptyTab();
	}
	
	void EditorWindow::RemoveColourPicker( const String & name )
	{
		// If name does not exist, nothing to do here
		ColourPickerList::iterator it = mColourPickers.find( name );
		if ( it == mColourPickers.end() )
			return;

		CEGUI::Window * parent = it->second->mRoot;

		// Remove entry from map, so same name can be added again
		delete it->second;
		mColourPickers.erase( it );

		// Clean up tabs and group
		DestoryParent( parent );
		RemoveEmptyGroup();
		RemoveEmptyTab();
	}


	EditorButton * EditorWindow::AddButtonToWindow( const String & name, const String & label,
													CEGUI::VerticalLayoutContainer * vLayout,
													const EditorButton::ClickCallback & callback )
	{
		// If name exist, do not add
		ButtonList::iterator it = mButtons.find( name );
		if ( it != mButtons.end() )
			return nullptr;

		EditorButton * button = new EditorButton( name, label, callback, vLayout );
		mButtons[name] = button;
		return button;
	}

	EditorButton * EditorWindow::AddButton( const String & name, const String & tab_name, const String & label, 
											const EditorButton::ClickCallback & callback )
	{
		//std::cout << "[Adding Button to " << tab_name << "]: " << name << std::endl;
		//std::cout << "[Adding Button]: " << name << std::endl;
		// If tab does not exist, create
		TabList::iterator it = mTabs.find( tab_name );
		if ( it == mTabs.end() )
			AddTab( tab_name );

		return AddButtonToWindow( name, label, mTabs[tab_name].second, callback );
	}
	
	EditorButton * EditorWindow::AddButtonToGroup( const String & name, const String & group_name, const String & label,
												   const EditorButton::ClickCallback & callback )
	{
		//if (group_name == "ScriptComponent"){
		//	std::cout << "[Adding Button to " << group_name << "]: " << name << std::endl;
		//	PrintExistingUIInfo();
		//}
		//std::cout << "[Adding Button to " << group_name << "]: " << name << std::endl;
		//std::cout << "[Adding Button]: " << name << std::endl;
		// If group does not exist, create
		GroupList::iterator it = mGroups.find( group_name );
		if ( it == mGroups.end() )
			return nullptr;

		// Add Edit box
		return AddButtonToWindow( name, label, mGroups[group_name], callback );
	}
	
	void EditorWindow::RemoveButton( const String & name )
	{
		//std::cout << "[Removing Button]: " << name << std::endl;
		// If name does not exist, nothing to do here
		ButtonList::iterator it = mButtons.find( name );
		if ( it == mButtons.end() )
			return;

		CEGUI::Window * parent = it->second->mRoot;

		// Remove entry from map, so same name can be added again
		delete it->second;
		mButtons.erase( it );

		// Clean up tabs and group
		DestoryParent( parent );
		RemoveEmptyGroup();
		RemoveEmptyTab();
	}

	void EditorWindow::AddDividerToWindow( const String & name, CEGUI::VerticalLayoutContainer * vLayout )
	{
		// If name exist, do not add
		DividerList::iterator it = mDividers.find( name );
		if ( it != mDividers.end() )
			return;

		EditorDivider * divider = new EditorDivider( name, vLayout );
		mDividers[name] = divider;
	}

	void EditorWindow::AddDivider( const String & name, const String & tab_name )
	{
		//std::cout << "[Adding Divider to " << tab_name << "]: " << name << std::endl;
		//std::cout << "[Adding Divider]: " << name << std::endl;
		// If tab does not exist, create
		TabList::iterator it = mTabs.find( tab_name );
		if ( it == mTabs.end() )
			AddTab( tab_name );

		return AddDividerToWindow( name, mTabs[tab_name].second );
	}
	
	void EditorWindow::AddDividerToGroup( const String & name, const String & group_name )
	{
		//if (group_name == "ScriptComponent"){
		//	std::cout << "[Adding Divider to " << group_name << "]: " << name << std::endl;
		//	PrintExistingUIInfo();
		//}
		//std::cout << "[Adding Divider to " << group_name << "]: " << name << std::endl;
		//std::cout << "[Adding Divider]: " << name << std::endl;
		// If group does not exist, create
		GroupList::iterator it = mGroups.find( group_name );
		if ( it == mGroups.end() )
			return;

		// Add Edit box
		return AddDividerToWindow( name, mGroups[group_name] );
	}
	
	void EditorWindow::RemoveDivider( const String & name )
	{
		//std::cout << "[Removing Divider]: " << name << std::endl;
		// If name does not exist, nothing to do here
		DividerList::iterator it = mDividers.find( name );
		if ( it == mDividers.end() )
			return;

		CEGUI::Window * parent = it->second->mRoot;

		// Remove entry from map, so same name can be added again
		delete it->second;
		mDividers.erase( it );

		// Clean up tabs and group
		DestoryParent( parent );
		RemoveEmptyGroup();
		RemoveEmptyTab();
	}

	void EditorWindow::SetSelectedTab( const String & tab )
	{
		TabList::iterator it = mTabs.find( tab );
		if ( it == mTabs.end() )
			return;

		//mTabControl->setSelectedTab( it->second.first->getID() );

		s32 i = 0, size = mTabControl->getTabCount();
		for ( i = 0; i < size; ++i )
		{
			if( mTabControl->getTabContentsAtIndex( i ) == static_cast<CEGUI::Window *>( it->second.first ) )
			{
				mTabControl->setSelectedTabAtIndex( i );
				break;
			}
		}
	}

	bool EditorWindow::TabOnChanged( const CEGUI::EventArgs & )
	{
		if ( mTabChangedCallback )
			mTabChangedCallback();
		
		return true;
	}

	String EditorWindow::GetSelectedTab() const
	{
		const CEGUI::Window * selected = mTabControl->getTabContentsAtIndex( mTabControl->getSelectedTabIndex() );

		if ( selected )
		{
			return selected->getText().c_str();
		}

		return "";
	}

	void EditorWindow::SetTabOnChangeCallback( const TabChangedCallback & callback )
	{
		mTabChangedCallback = callback;
	}

	bool EditorWindow::OnResized( const CEGUI::EventArgs & )
	{
		if ( mWindowResizeCallback )
			mWindowResizeCallback();
		
		return true;
	}

	void EditorWindow::SetWindowResizeCallback( const WindowResizeCallback & callback )
	{
		mWindowResizeCallback = callback;
	}

	Vector2f EditorWindow::GetWindowDimension() const
	{
		return Vector2f( mWindow->getSize().d_width.d_scale, mWindow->getSize().d_height.d_scale );
	}

	Vector2f EditorWindow::GetWindowOffset() const
	{
		return Vector2f( mWindow->getSize().d_width.d_offset, mWindow->getSize().d_height.d_offset );
	}

	void EditorWindow::PrintExistingUIInfo()
	{
		for ( auto & i : mGroups )
		{
			//if ( i.first != "ScriptComponent" )
			//	continue;

			size_t size = i.second->getChildCount();
			std::cout << "[Group]: " << i.first << " child num: " << size << std::endl;
			
			for ( size_t j = 0; j < size; ++j )
			{
				CEGUI::Window * child = i.second->getChildAtIdx( j );
				std::cout << "[Group]: " << i.first << " child[" << j << "] name: " << child->getName() << " - " << child->getType() << std::endl;

				size_t child_size = child->getChildCount();
				for ( size_t k = 0; k < child_size; ++k )
				{
					CEGUI::Window * childchild = child->getChildAtIdx( k );
					std::cout << "\t[Group]: " << child->getName() << " child[" << j << "] name: " << childchild->getName() << " - " << childchild->getType() << std::endl;
				}
			}
		}

		std::cout << "------------------------------------------------" << std::endl;
		for ( auto & i : mTabs )
		{
			size_t size = i.second.second->getChildCount();
			std::cout << "[Tab]: " << i.first << " child num: " << size << std::endl;
			
			for ( u32 j = 0; j < size; ++j )
			{
				CEGUI::Window * child = i.second.second->getChildAtIdx( j );
				std::cout << "[Tab]: " << i.first << " child[" << j << "] name: " << child->getName() << std::endl;
			}
		}
	}
}

#endif