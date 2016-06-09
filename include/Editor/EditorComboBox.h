#ifndef BH_EDITOR_COMBOBOX_H
#define BH_EDITOR_COMBOBOX_H

#include "Platform/Typedefs.h"
#include "Game/GameConfig.h"

#include <functional>

#if GAME_EDITOR_MODE

// forward declarations
#include "CEGUI/ForwardRefs.h"

namespace BH
{
	class EditorComboBox
	{
	public:
		typedef std::function< void() > OnChangeCallback;		//!< Changed Callback function

	public:
		// Constructor
		EditorComboBox( const String & name, const String & label, s32 * id, String * text, CEGUI::Window * root, bool readOnly );

		// Destructor
		~EditorComboBox();

		// Add Item
		void AddItem( const String & item, s32 id );

		// Add Items
		void AddItem( const String & item );

		// Set on change callback
		void SetOnChangeCallback( const OnChangeCallback & callback );

	private:
		// Set event
		bool SetValueEvent( const CEGUI::EventArgs & args );

		// Drop event
		bool DropEvent( const CEGUI::EventArgs & args );

		// Hide event
		bool HideEvent( const CEGUI::EventArgs & args );

	private:
		CEGUI::HorizontalLayoutContainer * mHLayout;
		CEGUI::Combobox * mBox;
		CEGUI::Window * mLabel;
		CEGUI::Window * mRoot;

		s32 * mID;
		String * mText;
		bool mReadOnly;

		OnChangeCallback mOnChangeCallback;

		friend class EditorWindow;
	};
}

#endif	// GAME_EDITOR_MODE

#endif