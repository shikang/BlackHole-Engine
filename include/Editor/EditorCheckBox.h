#ifndef BH_EDITOR_CHECKBOX_H
#define BH_EDITOR_CHECKBOX_H

#include "Platform/Typedefs.h"
#include "Game/GameConfig.h"

#if GAME_EDITOR_MODE

// forward declarations
#include "CEGUI/ForwardRefs.h"
//#include "CEGUI/widgets/ToggleButton.h"

namespace BH
{
	class EditorCheckBox
	{
	public:
		// Constructor
		EditorCheckBox( const String & name, const String & label, bool * var, CEGUI::Window * root, bool readOnly );

		// Destructor
		~EditorCheckBox();

	private:
		// Set event
		bool SetValueEvent( const CEGUI::EventArgs & args );

		// Refresh Values
		void Refresh();

	private:
		CEGUI::ToggleButton * mButton;
		CEGUI::Window * mRoot;

		bool * mVar;
		bool mReadOnly;

		friend class EditorWindow;
	};
}

#endif	// GAME_EDITOR_MODE

#endif