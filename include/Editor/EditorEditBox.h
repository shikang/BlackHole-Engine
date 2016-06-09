#ifndef BH_EDITOREDITBOX_H
#define BH_EDITOREDITBOX_H

#include "Platform/PlatformWindow.h"
#include "Platform/Vector2f.h"
#include "Game/GameConfig.h"

#if GAME_EDITOR_MODE

// forward declarations
#include "CEGUI/ForwardRefs.h"

namespace BH
{
	class EditorEditBox
	{
	public:
		typedef std::function< void( void * ) > RefreshCallback;

	public:
		// Constructor
		EditorEditBox( const String & name, const String & label, CEGUI::Window * root, bool readOnly );

		// Destructor
		virtual ~EditorEditBox();

		// Add callback for refresh event
		void AddRefreshCallback( const RefreshCallback & refreshCallback );

	protected:
		CEGUI::HorizontalLayoutContainer * mHLayout;
		CEGUI::Editbox * mEditBox;
		CEGUI::Window * mLabel;
		CEGUI::Window * mRoot;

		RefreshCallback mRefreshCallback;
		bool mReadOnly;

		friend class EditorWindow;

	};
}

#endif	// GAME_EDITOR_MODE

#endif