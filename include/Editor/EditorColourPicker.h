#ifndef BH_EDITOR_COLOURPICKER_H
#define BH_EDITOR_COLOURPICKER_H

#include "Platform/Typedefs.h"
#include "Platform/Vector3f.h"
#include "Platform/Vector4f.h"
#include "Core/Color.h"

#include "Game/GameConfig.h"

#if GAME_EDITOR_MODE

// forward declarations
#include "CEGUI/ForwardRefs.h"
#include "CEGUI/CommonDialogs/ColourPicker/ColourPicker.h"

namespace BH
{
	class EditorColourPicker
	{
	public:
		// Constructor
		EditorColourPicker( const String & name, const String & label, Color * colour, CEGUI::Window * root, bool readOnly );
		EditorColourPicker( const String & name, const String & label, Vector3f * colour, CEGUI::Window * root, bool readOnly );
		EditorColourPicker( const String & name, const String & label, Vector4f * colour, CEGUI::Window * root, bool readOnly );

		// Destructor
		~EditorColourPicker();

	private:
		// Set value event
		bool SetValueEvent( const CEGUI::EventArgs & args );

	private:
		CEGUI::HorizontalLayoutContainer * mHLayout;
		CEGUI::ColourPicker * mPicker;
		CEGUI::Window * mLabel;
		CEGUI::Window * mRoot;

		Color * mColour;
		Vector3f * mVector3f;
		Vector4f * mVector4f;
		bool mReadOnly;

		friend class EditorWindow;
	};
}

#endif	// GAME_EDITOR_MODE

#endif