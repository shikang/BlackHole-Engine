#ifndef BH_MATERIAL_EDITOR_H
#define BH_MATERIAL_EDITOR_H

#include "Platform/PlatformWindow.h"

#include "Core/IDrawable.h"
#include "Core/GameComponent.h"

#include "Game/GameConfig.h"

#if GAME_EDITOR_MODE

namespace BH
{
	class MaterialManager;
	class Phong;
	class EditorWindow;
	class EditorListBox;

	// Dependant on EditorApplication
	class MaterialEditor : public GameComponent, public IDrawable
	{
	public:
		// Constructor
		MaterialEditor();

		// Destructor
		~MaterialEditor();

		// Initialise
		void Initialise() override;

		// Shutdown
		void Shutdown() override;

		// Run
		void Draw( f32 dt ) override;

	private:
		// Toggle Editor state
		void ToogleEditorState();

		// Create Editor Layout
		void CreateEditorLayout();

		// Remove Editor Layout
		void RemoveEditorLayout();

		// Deselect Material
		void DeselectMaterial();

		// Select Material
		void SelectMaterial( Material * material );

		// Prepare Material List
		void InitMaterialList();

		// Clear Material List
		void ClearMaterialList();

		// Refresh Material List
		void RefreshMaterialList();

		// Add Material
		void AddNewMaterial();

		// Delete selected Material
		void DeleteSelectedMaterial();

	private:
		// Material List Change Callback
		void MaterialListChangeCallback();

		// Tab change Callback
		void TabChangedCallback();

		// Save Material List Callback
		void SaveMaterialListCallback();

		// Save callback
		void SaveCallback();

		// Control button pressed callback
		void ControlPressedCallback();

		// Control button released callback
		void ControlReleaseCallback();

		// Double click Object List
		void MaterialListDoubleClickCallback( const String & materialName );

		// Button Save Callback
		void ButtonSaveCallback();

	private:
		MaterialManager *					mMaterialManager;

	private:
		// Editor objects variables
		bool mActive;						//!< Is editor active
		EditorWindow * mMaterialWindow;		//!< Window that contains material(s) info
		EditorListBox * mMaterialListBox;	//!< List box that contains material list
		Material * mCurrentMaterial;		//!< Currently selected Material
		String mCurrentMaterialName;		//!< Currently selected material name from list box
		String mCurrentMaterialOrigName;	//!< Currently selected material original name from list box

		bool mCtrlPressed;					//!< If controlled is pressed

	};
}

#include "Core/Typing.h"
DECLARE_TYPE(::BH::MaterialEditor, MaterialEditor);

#endif	// GAME_EDITOR_MODE

#endif