#ifndef BH_EDITOR_WINDOW_H
#define BH_EDITOR_WINDOW_H

#include "Platform/PlatformWindow.h"
#include "Platform/Vector2f.h"
#include "Core/Color.h"

#include "Editor/EditorVariableBox.h"
#include "Editor/EditorButton.h"
#include "Editor/EditorColourPicker.h"

#include "Game/GameConfig.h"

#if GAME_EDITOR_MODE

// forward declarations
#include "CEGUI/ForwardRefs.h"
#include "CEGUI/widgets/VerticalLayoutContainer.h"

#include <list>
#include <unordered_map>

namespace BH
{
	// Forward Declarations
	class EditorCheckBox;
	class EditorComboBox;
	class EditorColourPicker;
	class EditorDivider;
	class EditorListBox;

	class EditorWindow
	{
	public:
		typedef std::unordered_map< String, CEGUI::VerticalLayoutContainer * > GroupList;
		typedef std::pair< CEGUI::ScrollablePane *, CEGUI::VerticalLayoutContainer * > Tab;
		typedef std::unordered_map< String, Tab > TabList;
		typedef std::unordered_map< String, EditorEditBox * > EditBoxList;
		typedef std::unordered_map< String, EditorCheckBox * > CheckBoxList;
		typedef std::unordered_map< String, EditorComboBox * > ComboBoxList;
		typedef std::unordered_map< String, EditorListBox * > ListBoxList;
		typedef std::unordered_map< String, EditorColourPicker * > ColourPickerList;
		typedef std::unordered_map< String, EditorButton * > ButtonList;
		typedef std::unordered_map< String, EditorDivider * > DividerList;

		typedef std::function< void() > TabChangedCallback;
		typedef std::function< void() > WindowResizeCallback;

	public:
		// Constructor
		EditorWindow( const String & name, const Vector2f & position,
					  const Vector2f & size, CEGUI::Window * root );

		// Destructor
		~EditorWindow();

		// Deactivate Window
		void DeactivateWindow();

	public:
		// Add UI functions

		// Create group
		void AddGroup( const String & group_name, const String & tab_name );

		// Create group in a group
		void AddGroupToGroup( const String & group_name, const String & group_group_name );

		// Add edit box
		template< typename T >
		EditorEditBox * AddEditBox( const String & name, const String & tab_name, const String & label, T * var, bool readOnly );

		// Add edit box in group
		template< typename T >
		EditorEditBox * AddEditBoxToGroup( const String & name, const String & group_name, const String & label, T * var, bool readOnly );

		// Remove edit box
		void RemoveEditBox( const String & name );

		// Add check box
		void AddCheckBox( const String & name, const String & tab_name, const String & label, bool * var, bool readOnly );

		// Add check box in group
		void AddCheckBoxToGroup( const String & name, const String & group_name, const String & label, bool * var, bool readOnly );

		// Remove check box
		void RemoveCheckBox( const String & name );

		// Add combo box
		EditorComboBox * AddComboBox( const String & name, const String & tab_name, const String & label, s32 * id, bool readOnly );

		// Add combo box
		EditorComboBox * AddComboBox( const String & name, const String & tab_name, const String & label, String * text, bool readOnly );

		// Add combo box
		EditorComboBox * AddComboBoxToGroup( const String & name, const String & group_name, const String & label, s32 * id, bool readOnly );

		// Add combo box
		EditorComboBox * AddComboBoxToGroup( const String & name, const String & group_name, const String & label, String * text, bool readOnly );

		// Remove combo box
		void RemoveComboBox( const String & name );

		// Add list box
		EditorListBox * AddListBox( const String & name, const String & tab_name, const String & label, String * text, bool readOnly );

		// Add list box
		EditorListBox * AddListBoxToGroup( const String & name, const String & group_name, const String & label, String * text, bool readOnly );

		// Remove list box
		void RemoveListBox( const String & name );

		// Add colour picker
		template< typename T >
		void AddColourPicker( const String & name, const String & tab_name, const String & label, T * var, bool readOnly );

		// Add colour picker in group
		template< typename T >
		void AddColourPickerToGroup( const String & name, const String & group_name, const String & label, T * var, bool readOnly );

		// Remove colour picker
		void RemoveColourPicker( const String & name );

		// Add Button
		EditorButton * AddButton( const String & name, const String & tab_name, const String & label, 
								  const EditorButton::ClickCallback & callback );

		// Add Button to group
		EditorButton * AddButtonToGroup( const String & name, const String & group_name, const String & label, 
										 const EditorButton::ClickCallback & callback );

		// Remove button
		void RemoveButton( const String & name );

		// Add divider to tab
		void AddDivider( const String & name, const String & tab_name );

		// Add divider to group
		void AddDividerToGroup( const String & name, const String & group_name );

		// Remove divider
		void RemoveDivider( const String & name );

		// Remove empty tab
		void RemoveEmptyTab();

		// Remove empty group
		void RemoveEmptyGroup();

		// Debug purposes
		void PrintExistingUIInfo();

		// Set tab to be selected
		void SetSelectedTab( const String & tab );

		// Get current selected tab
		String GetSelectedTab() const;

		// Set tab to be selected
		void SetTabOnChangeCallback( const TabChangedCallback & callback );

		// Get window dimension
		Vector2f GetWindowDimension() const;

		// Get window offset
		Vector2f GetWindowOffset() const;

		// Set window resize callback
		void SetWindowResizeCallback( const WindowResizeCallback & callback );

	private:
		// Add tab 
		void AddTab( const String & tab_name );

		// Add edit box helper function
		template< typename T >
		EditorEditBox * AddEditBoxToWindow( const String & name, const String & label,
											CEGUI::VerticalLayoutContainer * vLayout,
											T * var, bool readOnly );

		// Add check box helper function
		EditorCheckBox * AddCheckBoxToWindow( const String & name, const String & label,
											  CEGUI::VerticalLayoutContainer * vLayout,
											  bool * var, bool readOnly );

		// Add combo box helper function
		EditorComboBox * AddComboBoxToWindow( const String & name, const String & label,
											  CEGUI::VerticalLayoutContainer * vLayout,
											  s32 * id, String * text, bool readOnly );

		// Add list box helper function
		EditorListBox * AddListBoxToWindow( const String & name, const String & label,
											CEGUI::VerticalLayoutContainer * vLayout,
											String * text, bool readOnly );

		// Add colour picker helper function
		template< typename T >
		EditorColourPicker * AddColourPickerToWindow( const String & name, const String & label,
													  CEGUI::VerticalLayoutContainer * vLayout,
													  T * var, bool readOnly );

		// Add button helper function
		EditorButton * AddButtonToWindow( const String & name, const String & label,
										  CEGUI::VerticalLayoutContainer * vLayout,
										  const EditorButton::ClickCallback & callback );

		// Add divider helper function
		void AddDividerToWindow( const String & name, CEGUI::VerticalLayoutContainer * vLayout );

		void DestoryChild( CEGUI::Window * child );

		void DestoryParent( CEGUI::Window * parent );

		// Implementation of tab changed
		bool TabOnChanged( const CEGUI::EventArgs & args );

		// Implementation of window resized
		bool OnResized( const CEGUI::EventArgs & args );

	private:
		CEGUI::FrameWindow * mWindow;
		CEGUI::TabControl * mTabControl;			//!< Tab controller
		CEGUI::Window * mRoot;

		DividerList mDividers;						//!< Dividers that belongs to this windows
		ButtonList mButtons;						//!< Buttons that belongs to this windows
		CheckBoxList mCheckBoxes;					//!< Check boxes that belongs to this windows
		ComboBoxList mComboBoxes;					//!< Combo boxes that belongs to this windows
		ListBoxList mListBoxes;						//!< List boxes that belongs to this windows
		ColourPickerList mColourPickers;			//!< Colour pickers that belongs to this windows
		EditBoxList mEditBoxes;						//!< Edit boxes that belongs to this windows
		GroupList mGroups;							//!< Groups in order
		TabList mTabs;								//!< Tabs

		TabChangedCallback mTabChangedCallback;		//!< Callback called when tab changed
		WindowResizeCallback mWindowResizeCallback;	//!< Callback called when window size changed
	};
}

#include "Editor/EditorWindow.inl"

#endif	// GAME_EDITOR_MODE

#endif