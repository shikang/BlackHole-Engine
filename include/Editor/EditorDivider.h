#ifndef BH_EDITOR_DIVIDER_H
#define BH_EDITOR_DIVIDER_H

#include "Platform/Typedefs.h"
#include "Game/GameConfig.h"

#if GAME_EDITOR_MODE

// forward declarations
#include "CEGUI/ForwardRefs.h"

namespace BH
{
	class EditorDivider
	{
	public:
		// Constructor
		EditorDivider( const String & name, CEGUI::Window * root );

		// Destructor
		~EditorDivider();

	private:
		CEGUI::Window * mDivider;
		CEGUI::Window * mRoot;

		friend class EditorWindow;
	};
}

#endif	// GAME_EDITOR_MODE

#endif