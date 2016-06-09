#ifndef BH_EDITOR_BUTTON_H
#define BH_EDITOR_BUTTON_H

#include "Platform/Typedefs.h"
#include "Game/GameConfig.h"

#if GAME_EDITOR_MODE

// forward declarations
#include "CEGUI/ForwardRefs.h"

#include <functional>

namespace BH
{
	class EditorButton
	{
	public:
		typedef std::function< void() > ClickCallback;		//!< Click Callback function

	public:
		// Constructor
		EditorButton( const String & name, const String & label, const ClickCallback & callback, CEGUI::Window * root );

		// Destructor
		~EditorButton();

		// Set click call back
		void SetClickCallback( const ClickCallback & callback );

	private:
		// Button API callback
		bool CallClickCallback( const CEGUI::EventArgs & args );

	private:
		CEGUI::PushButton * mButton;
		CEGUI::Window * mRoot;

		ClickCallback mClickCallback;

		friend class EditorWindow;
	};
}

#endif	// GAME_EDITOR_MODE

#endif