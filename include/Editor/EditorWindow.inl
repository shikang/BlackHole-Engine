#if GAME_EDITOR_MODE

namespace BH
{
	template< typename T >
	EditorEditBox * EditorWindow::AddEditBoxToWindow( const String & name, const String & label,
													  CEGUI::VerticalLayoutContainer * vLayout,
													  T * var, bool readOnly )
	{
		// If name exist, do not add
		EditBoxList::iterator it = mEditBoxes.find( name );
		if ( it != mEditBoxes.end() )
			return nullptr;

		//EditorEditBox * newBox = new EditorEditBox( name, label, vLayout );
		EditorEditBox * newBox = new EditorVariableBox<T>( name, label, var, vLayout, readOnly );
		mEditBoxes[name] = newBox;
		return newBox;
	}

	template< typename T >
	EditorEditBox * EditorWindow::AddEditBox( const String & name, const String & tab_name, const String & label, T * var, bool readOnly )
	{
		//std::cout << "[Adding Edit Box to " << tab_name << "]: " << name << std::endl;
		//std::cout << "[Adding Edit Box]: " << name << std::endl;
		// If tab does not exist, create
		TabList::iterator it = mTabs.find( tab_name );
		if ( it == mTabs.end() )
			AddTab( tab_name );

		return AddEditBoxToWindow( name, label, mTabs[tab_name].second, var, readOnly );
	}

	template< typename T >
	EditorEditBox * EditorWindow::AddEditBoxToGroup( const String & name, const String & group_name, const String & label, T * var, bool readOnly )
	{
		//if (group_name == "ScriptComponent"){
		//	std::cout << "[Adding Edit Box to " << group_name << "]: " << name << std::endl;
		//	PrintExistingUIInfo();
		//}
		//std::cout << "[Adding Edit Box to " << group_name << "]: " << name << std::endl;
		//std::cout << "[Adding Edit Box]: " << name << std::endl;
		// If group does not exist, exit
		GroupList::iterator it = mGroups.find( group_name );
		if ( it == mGroups.end() )
			return nullptr;

		// Add Edit box
		return AddEditBoxToWindow( name, label, mGroups[group_name], var, readOnly );
	}

	template< typename T >
	EditorColourPicker * EditorWindow::AddColourPickerToWindow( const String & name, const String & label,
																CEGUI::VerticalLayoutContainer * vLayout,
																T * var, bool readOnly )
	{
		// If name exist, do not add
		ColourPickerList::iterator it = mColourPickers.find( name );
		if ( it != mColourPickers.end() )
			return nullptr;

		EditorColourPicker * picker = new EditorColourPicker( name, label, var, vLayout, readOnly );
		mColourPickers[name] = picker;
		return picker;
	}

	template< typename T >
	void EditorWindow::AddColourPicker( const String & name, const String & tab_name, const String & label, T * var, bool readOnly )
	{
		// If tab does not exist, create
		TabList::iterator it = mTabs.find( tab_name );
		if ( it == mTabs.end() )
			AddTab( tab_name );

		AddColourPickerToWindow( name, label, mTabs[tab_name].second, var, readOnly );
	}
	
	template< typename T >
	void EditorWindow::AddColourPickerToGroup( const String & name, const String & group_name, const String & label, T * var, bool readOnly )
	{
		// If group does not exist, create
		GroupList::iterator it = mGroups.find( group_name );
		if ( it == mGroups.end() )
			return;

		// Add Edit box
		AddColourPickerToWindow( name, label, mGroups[group_name], var, readOnly );
	}
}

#endif