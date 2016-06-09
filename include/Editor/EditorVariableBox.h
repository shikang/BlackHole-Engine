#ifndef BH_EDITORVARBOX_H
#define BH_EDITORVARBOX_H

#include "Platform/Typedefs.h"

#include "Editor/EditorEditBox.h"
#include "Editor/EditorUIManager.h"

#include "Game/GameConfig.h"

#if GAME_EDITOR_MODE

#include "CEGUI/widgets/Editbox.h"
#include "CEGUI/Event.h"
#include "CEGUI/SubscriberSlot.h"

#include <string>
#include <sstream>

namespace BH
{
	template< typename T >
	class EditorVariableBox : public EditorEditBox
	{
	public:
		// For tools use:
		inline bool SetRefresh( bool refresh ) { mRefresh = refresh; }

	public:
		// Constructor
		EditorVariableBox( const String & name, const String & label, T * var, CEGUI::Window * root, bool readOnly );

		// Destructor
		~EditorVariableBox();

	private:
		// Set Validate String (Not valid type will lead to compile error)
		void SetValidateString();

		// Set callback
		void SetValue();

		// Set value (For gizmo)
		void SetValue( const T & val );

		// Set event
		bool SetValueEvent( const CEGUI::EventArgs & args );

		// Refresh Values
		void Refresh();

		// Off refresh event
		bool OffRefreshEvent( const CEGUI::EventArgs & args );

	private:
		T * mVar;

		bool mRefresh;

	};
}

#include "Editor/EditorVariableBox.inl"

#endif	// GAME_EDITOR_MODE

#endif