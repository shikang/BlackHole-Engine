#ifndef BH_EDITOR_APPLICATION_H
#define BH_EDITOR_APPLICATION_H

#include "Platform/PlatformWindow.h"

#include "Core/IDrawable.h"
#include "Core/GameComponent.h"
#include "Core/Picking.h"

#include "Game/GameConfig.h"

#if GAME_EDITOR_MODE

#include "Editor/SelectedInfo.h"
#include "Editor/EditorGizmo.h"

namespace BH
{
	class Renderer;
	class Object;
	class EditorWindow;
	class EditorListBox;
	class Prefab;

	class EditorSystem : public GameComponent
	{
	public:
		typedef std::unordered_map< Hash, PickableInstance > PickableList;

	public:
		// Constructor
		EditorSystem();

		// Destructor
		~EditorSystem();

		// Initialise
		void Initialise() override;

		// Shutdown
		void Shutdown() override;

		// Run
		void Draw( f32 dt );

		// Get scene position from screen position
		bool GetScenePosFromScreen( const Vector2f & screenPos, Vector2f & scenePos ) const;

		// Hack function
		void RepickGizmo();

	private:
		// Recompile script
		void RecompileScript();

		// Toggle Editor state
		void ToogleEditorState();

		// Create Editor Layout
		void CreateEditorLayout();

		// Remove Editor Layout
		void RemoveEditorLayout();

		// Deselect Object
		void DeselectObject();

		// Select Object
		void SelectObject( Object * object );

		// Prepare Object List
		void InitObjectList();

		// Clear Object List
		void ClearObjectList();

		// Refresh Object List
		void RefreshObjectList();

		// Add object
		void AddNewObject();

		// Delete selected object
		void DeleteSelectedObject();

		// Deselect Object
		void DeselectPrefab();

		// Select Object
		void SelectPrefab();

		// Prepare Prefab List
		void InitPrefabList();

		// Clear Prefab List
		void ClearPrefabList();

		// Refresh Prefab List
		void RefreshPrefabList();

		// Add Prefab
		void AddNewPrefab();

		// Delete selected prefab
		void DeleteSelectedPrefab();

		// Init Pickables
		void InitPickables();

		// Update Selected pickable
		void UpdateSelectedPickable();

		// Remove pickable
		void RemovePickable( Object * obj );

		// Add pickable
		void AddPickable( Object * obj );

		// Get Picking info
		bool GetPickingInfo( Object * obj, PickableInstance & info );

		// Get Selected info
		bool GetSelectedInfo( Object * obj, SelectedInfo & info );

		// Clear Pickables
		void ClearPickables();

		// Draw Bounding Box
		void DrawBoundingBox();

		// Prepare level list
		void InitLevelList();

		// Clear level list
		void ClearLevelList();

		// Go to selected level
		void GoToSelectedLevel();

		// Delete selected level
		void DeleteSelectedLevel();

		// Add new level
		void AddNewLevel();

		// Refresh level list
		void RefreshLevelList();

		// Prepare graphic variable
		void InitGraphicsVariables();

		// Clear graphic variable
		void ClearGraphicsVariables();

	private:
		// Object List Change Callback
		void ObjectListChangeCallback();

		// Prefab List Change Callback
		void PrefabListChangeCallback();

		// Tab change Callback
		void TabChangedCallback();

		// Save Object List Callback
		void SaveObjectListCallback();

		// Save Archetype Callback
		void SaveArcheTypeCallback();

		// Create from archetype call back
		void CreateFromArcheTypeCallback();

		// Save callback
		void SaveCallback();

		// Control button pressed callback
		void ControlPressedCallback();

		// Control button released callback
		void ControlReleaseCallback();

		// Toggle showing bounding box
		void ToggleBoundingBox();

		// Pick callback
		void PickCallback( s32 x, s32 y );

		// Deselect callback
		void DeselectCallback( s32 x, s32 y );

		// Editor window Resize callback
		void EditorWindowResizeCallback();

		// Double click Object List
		void ObjectListDoubleClickCallback( const String & objectName );

		// Double click Prefab List
		void PrefabListDoubleClickCallback( const String & prefabName );

		// Un-pick gizmo callback
		void UnpickGizmo( s32 x, s32 y );

		// gizmo translate mode callback
		void GizmoSetTranslateModeCalback();

		// gizmo scale mode callback
		void GizmoSetScaleModeCalback();

		// gizmo translate mode callback
		void GizmoSetRotateModeCalback();

		// Double click Level List
		void LevelListDoubleClickCallback( const String & levelName );

		// Button click saved
		void ButtonSaveCallback();

		// Focus callback
		void FocusObjectCallback();

	private:
		Renderer *					mRenderer;

	private:
		// Editor objects variables
		bool mActive;						//!< Is editor active
		EditorWindow * mObjectWindow;		//!< Window that contains object(s) info
		EditorListBox * mObjectListBox;		//!< List box that contains objects list
		Object * mCurrentObject;			//!< Currently selected object
		String mCurrentObjectName;			//!< Currently selected object name from list box
		SelectedInfo mCurrentObjectInfo;	//!< Currently selected object info
		EditorGizmo mGizmo;					//!< Gizmo for easy moving

		// Editor Prefab variables
		EditorListBox * mPrefabListBox;		//!< List box that contains prefab list
		String mCurrentPrefabName;			//!< Current prefab name
		Prefab * mCurrentPrefab;			//!< Current prefab

		// Editor picking variables
		PickableList mPickables;			//!< All Pickables
		Mesh mLineCube;						//!< Line cube mesh
		bool mShowBB;						//!< Show Bounding Box

		// Editor level variables
		EditorListBox * mLevelListBox;		//!< List box that contains level list
		String mCurrentLevelName;			//!< Current level name
		String mNewLevelName;				//!< New level name
		bool mLevelChange;					//!< Is level changing
		bool mLevelDelete;					//!< Is level delete
		bool mLevelPostChange;				//!< Is level change complete

		bool mCtrlPressed;					//!< If controlled is pressed
	};
}

#include "Core/Typing.h"
DECLARE_TYPE(::BH::EditorSystem, EditorSystem);

#endif	// GAME_EDITOR_MODE

#endif