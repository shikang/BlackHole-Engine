#ifndef BH_EDITOR_LISTBOX_H
#define BH_EDITOR_LISTBOX_H

#include "Platform/Typedefs.h"
#include "Game/GameConfig.h"

#if GAME_EDITOR_MODE

// forward declarations
#include "CEGUI/ForwardRefs.h"
//#include "CEGUI/widgets/ToggleButton.h"

#include <functional>

namespace BH
{
	class EditorListBox
	{
	public:
		typedef std::function< void() > OnChangeCallback;						//!< Changed Callback function
		typedef std::function< void( const String & ) > OnDblClickCallback;	//!< Double click Callback function

	public:
		// Constructor
		EditorListBox( const String & name, const String & label, String * var, CEGUI::Window * root, bool readOnly );

		// Destructor
		~EditorListBox();

		// Add Items
		void AddItem( const String & item );

		// Clear Items
		void ClearItems();

		// Set on change callback
		void SetOnChangeCallback( const OnChangeCallback & callback );

		// Set on change callback
		void SetOnDblClickCallback( const OnDblClickCallback & callback );

		// Set selection
		void SetSelection( const String & item );

		// Unselect selection
		void Unselect();

	private:
		// Set event
		bool SetValueEvent( const CEGUI::EventArgs & args );

		// Double click event
		bool DoubleClickEvent( const CEGUI::EventArgs & args );

	private:
		CEGUI::Listbox * mListBox;
		CEGUI::Window * mRoot;

		String * mVar;
		bool mReadOnly;

		OnChangeCallback mOnChangeCallback;
		OnDblClickCallback mOnDblClickCallback;

		friend class EditorWindow;
	};
}

#endif	// GAME_EDITOR_MODE

#endif