#include "Platform/Platform.h"

#include "Core/CoreEssential.h"
#include "Core/Hash.h"
#include "Core/Game.h"
#include "Core/SystemManager.h"
#include "Core/Renderer.h"
#include "Core/System.h"
#include "Core/ObjectManager.h"
#include "Core/TypeCreator.h"
#include "Core/InputManager.h"
#include "Core/XMLSerialiser.h"
#include "Core/XMLDeserialiser.h"
#include "Core/LevelManager.h"
#include "Core/Math.h"
#include "Core/ModelManager.h"

#include "Script/ScriptComponent.h"

#include "Game/ScriptLoader.h"
#include "Game/DebugCamera.h"

#include "Editor/EditorApplication.h"
#include "Editor/EditorUIManager.h"
#include "Editor/EditorReflection.h"
#include "Editor/EditorListBox.h"
#include "Editor/Prefab.h"

#include "Platform/Graphics.h"
#include "Platform/GraphicsPipeline.h"
#include "Platform/MeshLoader.h"

#if GAME_EDITOR_MODE

#define GRAPHICS_DEMO 0

namespace BH
{
	EditorSystem::EditorSystem()
	: mRenderer( SYSTEM_MANAGER.GetGameComponentFromSystem<Renderer>() )
	, mObjectWindow( nullptr )
	, mCurrentObject( nullptr )
	, mCurrentPrefab( nullptr )
	, mObjectListBox( nullptr )
	, mPrefabListBox( nullptr )
	, mActive( false )
	, mCtrlPressed( false )
	, mShowBB( true )
	, mLevelListBox( nullptr )
	, mNewLevelName( "[New Level Name]" )
	, mLevelChange( false )
	, mLevelDelete( false )
	, mLevelPostChange( false )
	{
	}

	EditorSystem::~EditorSystem()
	{
	}

	void EditorSystem::Initialise()
	{
		// UI Init
		EDITOR_UI.Initialise( mRenderer );
		mRenderer->RegisterToDrawEvent( Renderer::DrawDelegate::FromMethod< E_METHOD( EditorSystem, Draw ) >( this ) );

		// Editor types init
		EDITOR_REFLECTION.RegisterTypes();

		// Add input callback
		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::F1, Input::Triggered, 
																				Delegate<void()>::FromMethod<E_METHOD( EditorSystem, ToogleEditorState )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::F2, Input::Triggered, 
																				Delegate<void()>::FromMethod<E_METHOD( EditorSystem, RecompileScript )>( this ) );

		// Add prefab type in reflection system
		ENTITY_MANAGER.Types.Create<Prefab>()
					  .FIELD( Name, Prefab )
					  .FIELD( Components, Prefab );

		MeshLoader::CreateLineCube( mLineCube );
		mGizmo.CreateGizmoMesh();
	}

	void EditorSystem::Shutdown()
	{
		if ( mActive )
			RemoveEditorLayout();

		// UI Shutdown
		EDITOR_UI.Shutdown();

		mRenderer->UnregisterFromDrawEvent( Renderer::DrawDelegate::FromMethod< E_METHOD( EditorSystem, Draw ) >( this ) );

		// Remove input callback
		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::F1, Input::Triggered, 
																				   Delegate<void()>::FromMethod<E_METHOD( EditorSystem, ToogleEditorState )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::F2, Input::Triggered, 
																				   Delegate<void()>::FromMethod<E_METHOD( EditorSystem, RecompileScript )>( this ) );

		mLineCube.Shutdown();
	}

	void EditorSystem::Draw( f32 dt )
	{
		EDITOR_REFLECTION.Update();

		// Update selected pickables
		//UpdateSelectedPickable();

		if ( mLevelPostChange )
		{
			mCurrentLevelName = SYSTEM_MANAGER.GetGameComponentFromSystem<BH::LevelManager>()->GetCurrentLevel();
			CreateEditorLayout();
			mLevelPostChange = false;
		}

		if ( mActive && ( mLevelChange || mLevelDelete ) )
		{
			String changedLevelName = mCurrentLevelName;
			RemoveEditorLayout();

			LevelManager * levelManager = SYSTEM_MANAGER.GetGameComponentFromSystem<BH::LevelManager>();

			if ( mLevelDelete )
			{
				levelManager->DeleteLevel( changedLevelName );
				BH_ASSERT( !levelManager->GetLevelList().empty(), "Level list is empty. Impossible!!!!" );
				changedLevelName = *levelManager->GetLevelList().begin();
			}
			levelManager->SetLevel( changedLevelName.c_str() );

			mLevelChange = false;
			mLevelDelete = false;
			mLevelPostChange = true;
		}

		// Draw Bounding box
		DrawBoundingBox();

		if ( mActive )
			mGizmo.Draw( dt );

		// UI Draw
		EDITOR_UI.Draw( dt );
	}

	void EditorSystem::RecompileScript()
	{
		//EDITOR_REFLECTION.RemoveFromEditor( window, ENTITY_MANAGER.Types.Get<Object>(), obj );
		String currentObjectName = "[No Name]";
		String currentPrefabName = "[No Name]";
		String activeTab;
		if ( mObjectWindow )
			activeTab = mObjectWindow->GetSelectedTab();

		if ( mObjectWindow && mCurrentObject )
		{
			currentObjectName = mCurrentObject->GetName();
			DeselectObject();
		}
		else if ( mObjectWindow && mCurrentPrefab )
		{
			currentPrefabName = mCurrentPrefab->Name;
			DeselectPrefab();
		}

		// Serialise state
		if ( mObjectWindow )
		{
			ClearObjectList();
			ClearPrefabList();
		}

		if ( FileSystem::FileCheckExists( "EditorRecompileObjectsState.xml" ) )
			FileSystem::FileDelete( "", "EditorRecompileObjectsState.xml" );

		if ( FileSystem::FileCheckExists( "EditorRecompilePrefabsState.xml" ) )
			FileSystem::FileDelete( "", "EditorRecompilePrefabsState.xml" );

		GAME.ObjectManager.SaveAllObjectsToFile<XMLSerialiser>( "EditorRecompileObjectsState.xml" );
		GAME.ObjectManager.ForceFullPurge();
		GAME.ObjectManager.SaveArcheTypes<XMLSerialiser>( "EditorRecompilePrefabsState.xml" );
		GAME.ObjectManager.UnloadArcheTypes();

		// Recompiled
		ScriptLoader * loader = SYSTEM_MANAGER.GetGameComponentFromSystem<ScriptLoader>();
		if ( loader )
			loader->ReInitialise();

		// Deserialise state
		GAME.ObjectManager.LoadAllObjectFromFile<XMLDeserialiser>( "EditorRecompileObjectsState.xml" );
		GAME.ObjectManager.LoadArcheTypes<XMLDeserialiser>( "EditorRecompilePrefabsState.xml" );
		if ( mObjectWindow )
		{
			InitObjectList();
			InitPrefabList();
		}

		//EDITOR_REFLECTION.AddToEditor( window, ENTITY_MANAGER.Types.Get<Object>(), obj );
		if ( mObjectWindow && currentObjectName != "[No Name]" )
		{
			Object * obj = GAME.ObjectManager.Get( CName( currentObjectName ) );
			SelectObject( obj );
		}
		else if ( mObjectWindow && currentPrefabName != "[No Name]" )
		{
			mCurrentPrefabName = currentPrefabName;
			SelectPrefab();
		}

		if ( mObjectWindow )
			mObjectWindow->SetSelectedTab( activeTab );
	}

	void EditorSystem::ToogleEditorState()
	{
		if ( mActive )
			RemoveEditorLayout();
		else
			CreateEditorLayout();
	}

	void EditorSystem::CreateEditorLayout()
	{
		if ( mActive )
			return;

		SYSTEM_MANAGER.PauseObjectComponents();

		// Create window
		//mObjectWindow = EDITOR_UI.CreateEditorWindow( "Object Editor", Vector2f( 0.8f, 0.0f ), Vector2f( 0.2f, 1.0f ) );
		mObjectWindow = EDITOR_UI.CreateEditorWindow( "Object Editor", Vector2f::ZERO, Vector2f( 0.2f, 1.0f ) );
		mActive = true;

#if GRAPHICS_DEMO
		InitGraphicsVariables();
#endif

		// Add all level to level list (with callbacks)
		InitLevelList();

		// Add all objects to object list (with callbacks)
		InitObjectList();

		// Add all prefabs to prefab list (with callbacks)
		InitPrefabList();

		// Set callback
		mObjectWindow->SetTabOnChangeCallback( std::bind( &EditorSystem::TabChangedCallback, this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::LCtrl, Input::Triggered, 
																				Delegate<void()>::FromMethod<E_METHOD( EditorSystem, ControlPressedCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::LCtrl, Input::Released, 
																				Delegate<void()>::FromMethod<E_METHOD( EditorSystem, ControlReleaseCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::S, Input::Triggered, 
																				Delegate<void()>::FromMethod<E_METHOD( EditorSystem, SaveCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddMouseEvent( Mouse::Left, Input::Triggered, 
																				  Delegate<void( s32, s32 )>::FromMethod<E_METHOD( EditorSystem, PickCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddMouseEvent( Mouse::Left, Input::Released, 
																				  Delegate<void( s32, s32 )>::FromMethod<E_METHOD( EditorSystem, UnpickGizmo )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddMouseDoubleClickEvent( Mouse::Right, 
																							 Delegate<void( s32, s32 )>::FromMethod<E_METHOD( EditorSystem, DeselectCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::W, Input::Triggered, 
																				Delegate<void()>::FromMethod<E_METHOD( EditorSystem, GizmoSetTranslateModeCalback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::R, Input::Triggered, 
																				Delegate<void()>::FromMethod<E_METHOD( EditorSystem, GizmoSetScaleModeCalback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::E, Input::Triggered, 
																				Delegate<void()>::FromMethod<E_METHOD( EditorSystem, GizmoSetRotateModeCalback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::F, Input::Triggered, 
																				Delegate<void()>::FromMethod<E_METHOD( EditorSystem, FocusObjectCallback )>( this ) );

		// Update Pickables
		InitPickables();

		// Set scene viewport
		EditorWindowResizeCallback();
		mObjectWindow->SetWindowResizeCallback( std::bind( &EditorSystem::EditorWindowResizeCallback, this ) );
	}

	void EditorSystem::RemoveEditorLayout()
	{
		if ( !mActive )
			return;

		SYSTEM_MANAGER.ResumeObjectComponents();

		// Remove existing object from editor
		DeselectObject();
		DeselectPrefab();

#if GRAPHICS_DEMO
		ClearGraphicsVariables();
#endif

		// Remove all level list
		ClearLevelList();

		// Remove all existing object from object list
		ClearObjectList();

		// Remove all existing prefabs from prefab list
		ClearPrefabList();

		// Destroy window
		if ( mObjectWindow )
			EDITOR_UI.DestroyEditorWindow( mObjectWindow );

		mObjectWindow = nullptr;
		mActive = false;
		mCurrentObjectName = "";
		mCurrentPrefabName = "";
		mCtrlPressed = false;

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::LCtrl, Input::Triggered, 
																				   Delegate<void()>::FromMethod<E_METHOD( EditorSystem, ControlPressedCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::LCtrl, Input::Released, 
																				   Delegate<void()>::FromMethod<E_METHOD( EditorSystem, ControlReleaseCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::S, Input::Triggered, 
																				   Delegate<void()>::FromMethod<E_METHOD( EditorSystem, SaveCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveMouseEvent( Mouse::Left, Input::Triggered, 
																					 Delegate<void( s32, s32 )>::FromMethod<E_METHOD( EditorSystem, PickCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveMouseEvent( Mouse::Left, Input::Released, 
																					 Delegate<void( s32, s32 )>::FromMethod<E_METHOD( EditorSystem, UnpickGizmo )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveMouseDoubleClickEvent( Mouse::Right, 
																								Delegate<void( s32, s32 )>::FromMethod<E_METHOD( EditorSystem, DeselectCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::W, Input::Triggered, 
																				   Delegate<void()>::FromMethod<E_METHOD( EditorSystem, GizmoSetTranslateModeCalback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::R, Input::Triggered, 
																				   Delegate<void()>::FromMethod<E_METHOD( EditorSystem, GizmoSetScaleModeCalback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::E, Input::Triggered, 
																				   Delegate<void()>::FromMethod<E_METHOD( EditorSystem, GizmoSetRotateModeCalback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::F, Input::Triggered, 
																				   Delegate<void()>::FromMethod<E_METHOD( EditorSystem, FocusObjectCallback )>( this ) );

		// Remove pickables
		ClearPickables();

		mRenderer->SetSceneViewport( 0.0f, 0.0f, 1.0f, 1.0f );
		mCurrentObjectInfo.Invalidize();
		mGizmo.UnpickGizmo();
		mGizmo.SetParent( nullptr );

		// Update all stationary box collider
		ScriptLoader * loader = SYSTEM_MANAGER.GetGameComponentFromSystem<ScriptLoader>();
		if ( loader )
		{
			for ( auto & i : loader->GetScriptList() )
			{
				if ( i->GetScript().mType == "StationaryBoxCollider" )
					i->CallScriptFunction( "ResetBox" );
			}
		}
	}

	void EditorSystem::DeselectObject()
	{
		if ( mObjectWindow && mCurrentObject )
		{
			if ( mObjectListBox )
				mObjectListBox->Unselect();

			// Update lastest info before deselecting
			UpdateSelectedPickable();
			EDITOR_REFLECTION.RemoveFromEditor( mObjectWindow, ENTITY_MANAGER.Types.Get<Object>(), mCurrentObject );
			mCurrentObject = nullptr;
			mCurrentObjectInfo.Invalidize();
			mGizmo.UnpickGizmo();
			mGizmo.SetParent( nullptr );
		}
	}

	void EditorSystem::RepickGizmo()
	{
		mCurrentObjectInfo.Invalidize();
		mGizmo.UnpickGizmo();
		mGizmo.SetParent( nullptr );

		if ( mCurrentObject )
		{
			GetSelectedInfo( mCurrentObject, mCurrentObjectInfo );
			if ( mCurrentObjectInfo.mPositionX )
				mGizmo.SetParent( &mCurrentObjectInfo );
		}
	}

	void EditorSystem::SelectObject( Object * object )
	{
		if ( !object || object == mCurrentObject )
			return;

		DeselectObject();
		DeselectPrefab();
		mPrefabListBox->Unselect();
		EDITOR_REFLECTION.AddToEditor( mObjectWindow, ENTITY_MANAGER.Types.Get<Object>(), object );
		mCurrentObject = object;
		GetSelectedInfo( object, mCurrentObjectInfo );

		if ( mCurrentObjectInfo.mPositionX )
			mGizmo.SetParent( &mCurrentObjectInfo );
		else
			mGizmo.SetParent( nullptr );

		//mObjectWindow->SetSelectedTab( "Object" );
	}

	void EditorSystem::InitObjectList()
	{
		if ( !mObjectWindow )
			return;

		if ( mObjectListBox )
			return;

		mObjectListBox = mObjectWindow->AddListBox( "ObjectListBox", "Object List", "Object List", &mCurrentObjectName, false );
		bool nameExist = false;
		for ( const auto & i : GAME.ObjectManager.GetObjectList() )
		{
			if ( i )
			{
				mObjectListBox->AddItem( i->GetName() );
				nameExist = nameExist ? true : i->GetName() == mCurrentObjectName;
			}
		}

		// Name does not exist, reset it
		if ( !nameExist )
			mCurrentObjectName = "";

		mObjectListBox->SetOnChangeCallback( std::bind( &EditorSystem::ObjectListChangeCallback, this ) );
		mObjectListBox->SetOnDblClickCallback( std::bind( &EditorSystem::ObjectListDoubleClickCallback, this, std::placeholders::_1 ) );

		mObjectWindow->AddButton( "EditorDeleteSelectedButton", "Object List", "Delete Object", 
								  std::bind( &EditorSystem::DeleteSelectedObject, this ) );

		mObjectWindow->AddDivider( "ObjectListBoxDivider", "Object List" );

		mObjectWindow->AddButton( "EditorAddNewButton", "Object List", "Add New Object", 
								  std::bind( &EditorSystem::AddNewObject, this ) );
	}

	void EditorSystem::ClearObjectList()
	{
		if ( !mObjectWindow )
			return;

		if ( !mObjectListBox )
			return;

		mObjectWindow->RemoveListBox( "ObjectListBox" );
		mObjectWindow->RemoveButton( "EditorDeleteSelectedButton" );
		mObjectWindow->RemoveDivider( "ObjectListBoxDivider" );
		mObjectWindow->RemoveButton( "EditorAddNewButton" );
		mObjectListBox = nullptr;
	}

	void EditorSystem::RefreshObjectList()
	{
		if ( !mObjectListBox )
			return;

		mObjectListBox->ClearItems();

		bool nameExist = false;
		for ( const auto & i : GAME.ObjectManager.GetObjectList() )
		{
			if ( i )
			{
				mObjectListBox->AddItem( i->GetName() );
				nameExist = nameExist ? true : i->GetName() == mCurrentObjectName;
			}
		}

		// Name does not exist, reset it
		if ( !nameExist )
		{
			mCurrentObjectName = "";
			mObjectListBox->Unselect();
		}
		else
			mObjectListBox->SetSelection( mCurrentObjectName );
	}

	void EditorSystem::ObjectListChangeCallback()
	{
		if ( !mCurrentObject || mCurrentObjectName != mCurrentObject->GetName() )
		{
			SelectObject( GAME.ObjectManager.Get( mCurrentObjectName.c_str() ) );
		}
	}

	void EditorSystem::AddNewObject()
	{
		// To add names back into object name table
		DeselectObject();
		DeselectPrefab();
		mPrefabListBox->Unselect();

		ObjectPtr object = GAME.ObjectManager.Create( "NewObject" );

		if ( !object )
			return;

		//ClearObjectList();
		mCurrentObjectName = object->GetName();
		//InitObjectList();
		RefreshObjectList();
		SelectObject( object );
		AddPickable( object );
		mObjectWindow->SetSelectedTab( "Object" );
	}

	void EditorSystem::DeleteSelectedObject()
	{
		if ( !mCurrentObject )
			return;

		Object * deleting = mCurrentObject;
		RemovePickable( deleting );
		DeselectObject();
		//ClearObjectList();
		GAME.ObjectManager.Destroy( deleting );
		GAME.ObjectManager.Purge();
		mCurrentObjectName = "";
		//InitObjectList();
		RefreshObjectList();


	}

	void EditorSystem::DeselectPrefab()
	{
		if ( mObjectWindow && mCurrentPrefab )
		{
			if ( mPrefabListBox )
				mPrefabListBox->Unselect();

			mObjectWindow->RemoveButton( "EditorCreateObjectFromPrefabButton" );
			mObjectWindow->RemoveDivider( "EditorCreateObjectFromPrefabDivider" );
			EDITOR_REFLECTION.RemoveFromEditor( mObjectWindow, ENTITY_MANAGER.Types.Get<Prefab>(), mCurrentPrefab );
			ENTITY_MANAGER.Delete( mCurrentPrefab );
			mCurrentPrefab = nullptr;
		}
	}

	void EditorSystem::SelectPrefab()
	{
		if ( mCurrentPrefabName == "" )
			return;

		DeselectObject();
		DeselectPrefab();
		mObjectListBox->Unselect();

		if ( mCurrentPrefab )
			return;

		// Prepare prefab and add it to editor
		Object * obj = GAME.ObjectManager.GetArcheType( mCurrentPrefabName );
		if ( !obj )
		{
			mCurrentPrefabName = "";
			return;
		}

		mCurrentPrefab = ENTITY_MANAGER.New<Prefab>();
		mCurrentPrefab->Initialise( mCurrentPrefabName, obj );
		mObjectWindow->AddButton( "EditorCreateObjectFromPrefabButton", "Prefab", "Create Object From Prefab", 
								  std::bind( &EditorSystem::CreateFromArcheTypeCallback, this ) );
		mObjectWindow->AddDivider( "EditorCreateObjectFromPrefabDivider", "Prefab" );
		EDITOR_REFLECTION.AddToEditor( mObjectWindow, ENTITY_MANAGER.Types.Get<Prefab>(), mCurrentPrefab );

		//mObjectWindow->SetSelectedTab( "Prefab" );
	}

	void EditorSystem::InitPrefabList()
	{
		if ( !mObjectWindow )
			return;

		if ( mPrefabListBox )
			return;

		mPrefabListBox = mObjectWindow->AddListBox( "PrefabListBox", "Prefab List", "Prefab List", &mCurrentPrefabName, false );

		bool nameExist = false;
		for ( const auto & i : GAME.ObjectManager.GetArcheTypeTable() )
		{
			mPrefabListBox->AddItem( i.first.GetText() );
			nameExist = nameExist ? true : i.first.GetText() == mCurrentPrefabName;
		}

		mPrefabListBox->SetOnChangeCallback( std::bind( &EditorSystem::PrefabListChangeCallback, this ) );
		mPrefabListBox->SetOnDblClickCallback( std::bind( &EditorSystem::PrefabListDoubleClickCallback, this, std::placeholders::_1 ) );

		// Name does not exist, reset it
		if ( !nameExist )
			mCurrentPrefabName = "";

		mObjectWindow->AddButton( "EditorDeleteSelectedPrefabButton", "Prefab List", "Delete Prefab", 
								  std::bind( &EditorSystem::DeleteSelectedPrefab, this ) );

		mObjectWindow->AddDivider( "PrefabListBoxDivider", "Prefab List" );

		mObjectWindow->AddButton( "EditorAddNewPrefabButton", "Prefab List", "Add New Prefab", 
								  std::bind( &EditorSystem::AddNewPrefab, this ) );
	}

	void EditorSystem::ClearPrefabList()
	{
		if ( !mObjectWindow )
			return;

		if ( !mPrefabListBox )
			return;

		if ( mCurrentPrefab )
			ENTITY_MANAGER.Delete( mCurrentPrefab );

		mObjectWindow->RemoveListBox( "PrefabListBox" );
		mObjectWindow->RemoveButton( "EditorDeleteSelectedPrefabButton" );
		mObjectWindow->RemoveDivider( "PrefabListBoxDivider" );
		mObjectWindow->RemoveButton( "EditorAddNewPrefabButton" );

		mPrefabListBox = nullptr;
		mCurrentPrefab = nullptr;
	}

	void EditorSystem::RefreshPrefabList()
	{
		if ( !mPrefabListBox )
			return;

		mPrefabListBox->ClearItems();

		bool nameExist = false;
		for ( const auto & i : GAME.ObjectManager.GetArcheTypeTable() )
		{
			mPrefabListBox->AddItem( i.first.GetText() );
			nameExist = nameExist ? true : i.first.GetText() == mCurrentPrefabName;
		}

		// Name does not exist, reset it
		if ( !nameExist )
		{
			mCurrentPrefabName = "";
			mPrefabListBox->Unselect();
		}
		else
			mPrefabListBox->SetSelection( mCurrentPrefabName );
	}

	void EditorSystem::PrefabListChangeCallback()
	{
		if ( !mCurrentPrefab || mCurrentPrefabName != mCurrentPrefab->Name )
		{
			SelectPrefab();
		}
	}

	void EditorSystem::AddNewPrefab()
	{
		DeselectObject();
		DeselectPrefab();
		mObjectListBox->Unselect();

		Hash newPrefabName = "NewPrefab";
		GAME.ObjectManager.GetAvailableNewArcheType( newPrefabName );
		Object * obj = GAME.ObjectManager.CreateArcheType( newPrefabName );

		if ( obj )
		{
			mCurrentPrefabName = newPrefabName.GetText();
			RefreshPrefabList();
			SelectPrefab();

			//mCurrentPrefab = ENTITY_MANAGER.New<Prefab>();
			//mCurrentPrefab->Initialise( newPrefabName.GetText(), obj );
			//RefreshPrefabList();
			//mObjectWindow->AddButton( "EditorCreateObjectFromPrefabButton", "Prefab", "Create Object From Prefab", 
			//						  std::bind( &EditorSystem::CreateFromArcheTypeCallback, this ) );
			//mObjectWindow->AddDivider( "EditorCreateObjectFromPrefabDivider", "Prefab" );
			//EDITOR_REFLECTION.AddToEditor( mObjectWindow, ENTITY_MANAGER.Types.Get<Prefab>(), mCurrentPrefab );
		}

		mObjectWindow->SetSelectedTab( "Prefab" );
	}

	void EditorSystem::DeleteSelectedPrefab()
	{
		if ( mObjectWindow && mCurrentPrefab )
		{
			EDITOR_REFLECTION.RemoveFromEditor( mObjectWindow, ENTITY_MANAGER.Types.Get<Prefab>(), mCurrentPrefab );
			GAME.ObjectManager.DeleteArcheType( mCurrentPrefab->Name );
			ENTITY_MANAGER.Delete( mCurrentPrefab );
			mCurrentPrefab = nullptr;

			//ClearPrefabList();
			//InitPrefabList();
			RefreshPrefabList();
		}
	}

	void EditorSystem::TabChangedCallback()
	{
		String tab = mObjectWindow->GetSelectedTab();

		if ( tab == "Object List" )
		{
			// Object rename
			if ( mCurrentObject && mCurrentObject->GetName() != mCurrentObjectName )
			{
				mCurrentObjectName = mCurrentObject->GetName();
				//ClearObjectList();
				//InitObjectList();
				RefreshObjectList();
				mObjectWindow->SetSelectedTab( "Object List" );
			}
		}
		else if ( tab == "Prefab List" )
		{
			// Prefab rename
			if ( mCurrentPrefab && mCurrentPrefab->Name != mCurrentPrefabName )
			{
				mCurrentPrefabName = mCurrentPrefab->Name;
				DeselectPrefab();
				//ClearPrefabList();
				//InitPrefabList();
				RefreshPrefabList();
				SelectPrefab();
				mObjectWindow->SetSelectedTab( "Prefab List" );
			}
		}
	}

	void EditorSystem::SaveObjectListCallback()
	{
		String filename = BH::LevelManager::LoadDirectory + 
						  SYSTEM_MANAGER.GetGameComponentFromSystem<BH::LevelManager>()->GetCurrentLevel() +
						  ".xml";

		if ( FileSystem::FileCheckExists( filename.c_str() ) )
			FileSystem::FileDelete( "", filename.c_str() );

		GAME.ObjectManager.SaveAllObjectsToFile<XMLSerialiser>( filename.c_str() );
	}

	void EditorSystem::SaveArcheTypeCallback()
	{
		if ( FileSystem::FileCheckExists( "../resources/Prefabs.xml" ) )
			FileSystem::FileDelete( "", "../resources/Prefabs.xml" );

		GAME.ObjectManager.SaveArcheTypes<XMLSerialiser>( "../resources/Prefabs.xml" );
	}

	void EditorSystem::ControlPressedCallback()
	{
		mCtrlPressed = true;
	}

	void EditorSystem::ControlReleaseCallback()
	{
		mCtrlPressed = false;
	}

	void EditorSystem::ButtonSaveCallback()
	{
		mCtrlPressed = true;
		SaveCallback();
		mCtrlPressed = false;
	}

	void EditorSystem::SaveCallback()
	{
		if ( !mCtrlPressed )
			return;

		String currentObjectName = "[No Name]";
		String currentPrefabName = "[No Name]";
		String activeTab;
		if ( mObjectWindow )
			activeTab = mObjectWindow->GetSelectedTab();

		if ( mObjectWindow && mCurrentObject )
		{
			currentObjectName = mCurrentObject->GetName();
			DeselectObject();
		}
		else if ( mObjectWindow && mCurrentPrefab )
		{
			currentPrefabName = mCurrentPrefab->Name;
			DeselectPrefab();
		}

		SaveObjectListCallback();
		SaveArcheTypeCallback();

		if ( mObjectWindow && currentObjectName != "[No Name]" )
		{
			Object * obj = GAME.ObjectManager.Get( CName( currentObjectName ) );
			SelectObject( obj );
		}
		else if ( mObjectWindow && currentPrefabName != "[No Name]" )
		{
			mCurrentPrefabName = currentPrefabName;
			SelectPrefab();
		}

		if ( mObjectWindow )
			mObjectWindow->SetSelectedTab( activeTab );
	}

	void EditorSystem::CreateFromArcheTypeCallback()
	{
		if ( !mCurrentPrefab )
			return;

		String prefabName = mCurrentPrefab->Name;
		DeselectObject();
		DeselectPrefab();
		mPrefabListBox->Unselect();

		//ObjectPtr object = GAME.ObjectManager.CreateFromArcheType( mCurrentPrefabName, "NewObject" );
		if ( FileSystem::FileCheckExists( "EditorCreateFromArcheType.xml" ) )
			FileSystem::FileDelete( "", "EditorCreateFromArcheType.xml" );

		Hash newPrefabName = "New" + prefabName;
		GAME.ObjectManager.AssignAvailableObjectName( newPrefabName );
		GAME.ObjectManager.SaveObjectToFile<XMLSerialiser>( "EditorCreateFromArcheType.xml", GAME.ObjectManager.GetArcheType( prefabName ), 
															 newPrefabName.GetText().c_str() );
		Object * object = GAME.ObjectManager.LoadObjectFromFile<XMLDeserialiser>( "EditorCreateFromArcheType.xml" );

		if ( !object )
		{
			mCurrentPrefabName = prefabName;
			SelectPrefab();
			return;
		}

		//ClearObjectList();
		mCurrentObjectName = object->GetName();
		//InitObjectList();
		RefreshObjectList();
		SelectObject( object );
		AddPickable( object );
		mObjectWindow->SetSelectedTab( "Object" );
	}

	bool EditorSystem::GetPickingInfo( Object * obj, PickableInstance & info )
	{
		auto scripts = obj->GetAllComponent<ScriptComponent>();

		bool transform = false;
		bool modal = false;

		for ( const auto & j : scripts )
		{
			if ( j->GetScript().mType == "TransformComponent" )
			{
				// get position, scale and rotation
				void * data = j->GetScriptValue();

				std::list< const Char * > l { "mPosition", "x" };
				void * val = j->GetValueFromScriptValue( l, data );
				BH_ASSERT( val, "EditorSystem::InitPickables - Failed to get position x from ScriptComponent" );
				info.mPosition.x = *reinterpret_cast< f32 * >( val );

				l = { "mPosition", "y" };
				val = j->GetValueFromScriptValue( l, data );
				BH_ASSERT( val, "EditorSystem::InitPickables - Failed to get position y from ScriptComponent" );
				info.mPosition.y = *reinterpret_cast< f32 * >( val );

				l = { "mPosition", "z" };
				val = j->GetValueFromScriptValue( l, data );
				BH_ASSERT( val, "EditorSystem::InitPickables - Failed to get position z from ScriptComponent" );
				info.mPosition.z = *reinterpret_cast< f32 * >( val );

				l = { "mScale", "x" };
				val = j->GetValueFromScriptValue( l, data );
				BH_ASSERT( val, "EditorSystem::InitPickables - Failed to get scale x from ScriptComponent" );
				info.mScale.x = *reinterpret_cast< f32 * >( val );

				l = { "mScale", "y" };
				val = j->GetValueFromScriptValue( l, data );
				BH_ASSERT( val, "EditorSystem::InitPickables - Failed to get scale y from ScriptComponent" );
				info.mScale.y = *reinterpret_cast< f32 * >( val );

				l = { "mScale", "z" };
				val = j->GetValueFromScriptValue( l, data );
				BH_ASSERT( val, "EditorSystem::InitPickables - Failed to get scale z from ScriptComponent" );
				info.mScale.z = *reinterpret_cast< f32 * >( val );

				l = { "mRotation", "x" };
				val = j->GetValueFromScriptValue( l, data );
				BH_ASSERT( val, "EditorSystem::InitPickables - Failed to get rotation x from ScriptComponent" );
				info.mRotation.x = Math::DegToRad( *reinterpret_cast< f32 * >( val ) );

				l = { "mRotation", "y" };
				val = j->GetValueFromScriptValue( l, data );
				BH_ASSERT( val, "EditorSystem::InitPickables - Failed to get rotation y from ScriptComponent" );
				info.mRotation.y = Math::DegToRad( *reinterpret_cast< f32 * >( val ) );

				l = { "mRotation", "z" };
				val = j->GetValueFromScriptValue( l, data );
				BH_ASSERT( val, "EditorSystem::InitPickables - Failed to get rotation z from ScriptComponent" );
				info.mRotation.z = Math::DegToRad( *reinterpret_cast< f32 * >( val ) );

				j->FreeScriptValue( data );
				transform = true;
			}
			else if ( j->GetScript().mType == "ModelComponent" )
			{
				// get aabb
				void * data = j->GetScriptValue();

				std::list< const Char * > l{ "mModel" };
				void * val = j->GetValueFromScriptValue( l, data );
				BH_ASSERT( val, "EditorSystem::InitPickables - Failed to get position x from ScriptComponent" );
				String modelName = *reinterpret_cast< String * >( val );

				info.mAABB = SYSTEM_MANAGER.GetGameComponentFromSystem<ModelManager>()->GetAABB( modelName );

				j->FreeScriptValue( data );
				modal = true;
			}

			if ( modal && transform )
				return true;
		}

		return false;
	}

	bool EditorSystem::GetSelectedInfo( Object * obj, SelectedInfo & info )
	{
		auto scripts = obj->GetAllComponent<ScriptComponent>();

		bool transform = false;
		bool model = false;

		for ( const auto & j : scripts )
		{
			if ( j->GetScript().mType == "TransformComponent" )
			{
				// get position, scale and rotation
				void * data = j->GetScriptValue();

				std::list< const Char * > l { "mPosition", "x" };
				void * val = j->GetValueFromScriptValue( l, data );
				BH_ASSERT( val, "EditorSystem::InitPickables - Failed to get position x from ScriptComponent" );
				info.mPositionX = reinterpret_cast< f32 * >( val );

				l = { "mPosition", "y" };
				val = j->GetValueFromScriptValue( l, data );
				BH_ASSERT( val, "EditorSystem::InitPickables - Failed to get position y from ScriptComponent" );
				info.mPositionY = reinterpret_cast< f32 * >( val );

				l = { "mPosition", "z" };
				val = j->GetValueFromScriptValue( l, data );
				BH_ASSERT( val, "EditorSystem::InitPickables - Failed to get position z from ScriptComponent" );
				info.mPositionZ = reinterpret_cast< f32 * >( val );

				l = { "mScale", "x" };
				val = j->GetValueFromScriptValue( l, data );
				BH_ASSERT( val, "EditorSystem::InitPickables - Failed to get scale x from ScriptComponent" );
				info.mScaleX = reinterpret_cast< f32 * >( val );

				l = { "mScale", "y" };
				val = j->GetValueFromScriptValue( l, data );
				BH_ASSERT( val, "EditorSystem::InitPickables - Failed to get scale y from ScriptComponent" );
				info.mScaleY = reinterpret_cast< f32 * >( val );

				l = { "mScale", "z" };
				val = j->GetValueFromScriptValue( l, data );
				BH_ASSERT( val, "EditorSystem::InitPickables - Failed to get scale z from ScriptComponent" );
				info.mScaleZ = reinterpret_cast< f32 * >( val );

				l = { "mRotation", "x" };
				val = j->GetValueFromScriptValue( l, data );
				BH_ASSERT( val, "EditorSystem::InitPickables - Failed to get rotation x from ScriptComponent" );
				info.mRotationX = reinterpret_cast< f32 * >( val );

				l = { "mRotation", "y" };
				val = j->GetValueFromScriptValue( l, data );
				BH_ASSERT( val, "EditorSystem::InitPickables - Failed to get rotation y from ScriptComponent" );
				info.mRotationY = reinterpret_cast< f32 * >( val );

				l = { "mRotation", "z" };
				val = j->GetValueFromScriptValue( l, data );
				BH_ASSERT( val, "EditorSystem::InitPickables - Failed to get rotation z from ScriptComponent" );
				info.mRotationZ = reinterpret_cast< f32 * >( val );

				j->FreeScriptValue( data );
				transform = true;
			}
			else if ( j->GetScript().mType == "ModelComponent" )
			{
				// get aabb
				void * data = j->GetScriptValue();

				std::list< const Char * > l{ "mModel" };
				void * val = j->GetValueFromScriptValue( l, data );
				BH_ASSERT( val, "EditorSystem::InitPickables - Failed to get position x from ScriptComponent" );
				info.mModelName = reinterpret_cast< String * >( val );

				//info.mAABB = SYSTEM_MANAGER.GetGameComponentFromSystem<ModelManager>()->GetAABB( modelName );

				j->FreeScriptValue( data );
				model = true;
			}

			if ( model && transform )
				return true;
		}

		info.Invalidize();

		return false;
	}

	void EditorSystem::InitPickables()
	{
		for ( const auto & i : GAME.ObjectManager.GetObjectList() )
		{
			if ( mPickables.find( i->GetName() ) != mPickables.end() )
				continue;

			PickableInstance p;

			if ( GetPickingInfo( i, p ) )
				mPickables[ i->GetName() ] = p;
		}
	}

	void EditorSystem::AddPickable( Object * object )
	{
		if ( !object )
			return;

		if ( mPickables.find( object->GetName() ) != mPickables.end() )
			return;

		PickableInstance p;

		if ( GetPickingInfo( object, p ) )
			mPickables[ object->GetName() ] = p;
	}

	void EditorSystem::RemovePickable( Object * object )
	{
		if ( !object )
			return;

		mPickables.erase( object->GetName() );
	}

	void EditorSystem::ClearPickables()
	{
		mPickables.clear();
	}

	void EditorSystem::UpdateSelectedPickable()
	{
		// Either editoris not active, or no selected object or selected object is not pickable
		if ( !mActive || !mCurrentObject || mPickables.find( mCurrentObject->GetName() ) == mPickables.end() )
			return;

		PickableInstance p;

		if ( GetPickingInfo( mCurrentObject, p ) )
			mPickables[ mCurrentObject->GetName() ] = p;
	}

	void EditorSystem::ToggleBoundingBox()
	{
		mShowBB = !mShowBB;
	}

	void EditorSystem::DrawBoundingBox()
	{
		if ( !mActive || !mShowBB )
			return;

		for ( const auto & i : mPickables )
		{
			// Skip
			if ( mCurrentObject && i.first == mCurrentObject->GetName() )
				continue;

			const Vector3f & rot = i.second.mRotation;

			Matrix4 rotation = Matrix4::CreateFromYawPitchRoll( rot.y, rot.x, rot.z );
			Matrix4 trans = Matrix4::CreateTranslation( i.second.mPosition );
			Matrix4 scale = Matrix4::CreateScale( i.second.mScale );

			trans = trans * scale;// *gModelMatrix;

			Vector3f n_min = trans * i.second.mAABB.mMin;
			Vector3f n_max = trans * i.second.mAABB.mMax;
			Vector3f n_scale = ( n_max - n_min );
			Vector3f n_pos = n_min + 0.5f * n_scale;

			Matrix4 trans_o = Matrix4::CreateTranslation( n_pos - i.second.mPosition );

			trans = Matrix4::CreateTranslation( i.second.mPosition );
			scale = Matrix4::CreateScale( n_scale );

			Matrix4 transform = trans * rotation * trans_o * scale;

			Vector4f color = Vector4f::ONE;
			mRenderer->DrawLines( transform, color, &mLineCube );
		}

		// Draw selected object BB
		if ( mCurrentObject &&
			 mCurrentObjectInfo.mPositionX  &&
			 mCurrentObjectInfo.mModelName )
		{
			Vector3f pos( *mCurrentObjectInfo.mPositionX, *mCurrentObjectInfo.mPositionY, *mCurrentObjectInfo.mPositionZ );
			Matrix4 rotation = Matrix4::CreateFromYawPitchRoll( Math::DegToRad( *mCurrentObjectInfo.mRotationY ), 
																Math::DegToRad( *mCurrentObjectInfo.mRotationX ), 
																Math::DegToRad( *mCurrentObjectInfo.mRotationZ ) );
			Matrix4 trans = Matrix4::CreateTranslation( pos );
			Matrix4 scale = Matrix4::CreateScale( *mCurrentObjectInfo.mScaleX, *mCurrentObjectInfo.mScaleY, *mCurrentObjectInfo.mScaleZ );

			AABB aabb = SYSTEM_MANAGER.GetGameComponentFromSystem<ModelManager>()->GetAABB( *mCurrentObjectInfo.mModelName );
			trans = trans * scale;// *gModelMatrix;

			Vector3f n_min = trans * aabb.mMin;
			Vector3f n_max = trans * aabb.mMax;
			Vector3f n_scale = ( n_max - n_min );
			Vector3f n_pos = n_min + 0.5f * n_scale;

			Matrix4 trans_o = Matrix4::CreateTranslation( n_pos - pos );

			trans = Matrix4::CreateTranslation( pos );
			scale = Matrix4::CreateScale( n_scale );

			Matrix4 transform = trans * rotation * trans_o * scale;

			Vector4f color = Vector4f::UNIT_X + Vector4f::UNIT_W;
			mRenderer->DrawLines( transform, color, &mLineCube );
		}
	}

	bool EditorSystem::GetScenePosFromScreen( const Vector2f & screenPos, Vector2f & scenePos ) const
	{
		Vector2f offset = mRenderer->GetSceneOffset();
		Vector2f dim = mRenderer->GetSceneDimension();
		Vector2f maxBorder = offset + dim;

		f32 mX = screenPos.x / static_cast< f32 >( GAME.Window.GetWidth() );
		f32 mY = 1.0f - screenPos.y / static_cast< f32 >( GAME.Window.GetHeight() );

		//std::cout << "Screen: " << mX << ", " << mY << std::endl;

		// If mouse position is outside scene
		if ( mX < offset.x || mX > maxBorder.x ||
			 mY < offset.y || mY > maxBorder.y )
		{
			//std::cout << "--------------------------------------------------------------------------------" << std::endl;
			return false;
		}

		mX = ( mX - offset.x ) / dim.x * static_cast< f32 >( GAME.Window.GetWidth() );
		mY = ( 1.0f - ( mY - offset.y ) / dim.y ) * static_cast< f32 >( GAME.Window.GetHeight() );

		//std::cout << "Scene screen space: " << mX << ", " << mY << std::endl;
		//std::cout << "--------------------------------------------------------------------------------" << std::endl;
		
		scenePos.x = mX;
		scenePos.y = mY;
		return true;
	}

	void EditorSystem::PickCallback( s32 x, s32 y )
	{
		UpdateSelectedPickable();

		Vector2f scenePos;
		if ( !GetScenePosFromScreen( Vector2f( static_cast< f32 >( x ), static_cast< f32 >( y ) ), scenePos ) )
			return;

		bool gizmo = false;
		if ( mCurrentObject && mGizmo.GetParent() != nullptr )
			gizmo = mGizmo.PickGizmo();

		if ( gizmo )
			return;

		Hash objName = Pick( mPickables,
							 mRenderer->GetPipeline()->GetWorld(),
							 mRenderer->GetCamera()->GetView(),
							 mRenderer->GetPipeline()->GetProjection(),
							 scenePos,
							 Vector2f( f32( GAME.Window.GetWidth() ), f32( GAME.Window.GetHeight() ) ) );

		if ( objName.GetText() == "" || 
			 ( mCurrentObject && mCurrentObject->GetHashValue() == objName.GetHash() ) )
			return;

		SelectObject( GAME.ObjectManager.Get( objName.GetText().c_str() ) );
	}

	void EditorSystem::DeselectCallback( s32 x, s32 y )
	{
		Vector2f scenePos;
		if ( !GetScenePosFromScreen( Vector2f( static_cast< f32 >( x ), static_cast< f32 >( y ) ), scenePos ) )
			return;

		DeselectObject();
	}

	void EditorSystem::EditorWindowResizeCallback()
	{
		if ( !mObjectWindow )
			return;

		Vector2f windowDim = mObjectWindow->GetWindowDimension();
		Vector2f windowOffset = mObjectWindow->GetWindowOffset();
		windowOffset.x /= static_cast< f32 >( GAME.Window.GetWidth() );
		windowOffset.y /= static_cast< f32 >( GAME.Window.GetHeight() );
		windowDim += windowOffset;
		mRenderer->SetSceneViewport( windowDim.x, windowDim.x, 1.0f - windowDim.x, 1.0f - windowDim.x );
	}

	void EditorSystem::ObjectListDoubleClickCallback( const String & objectName )
	{
		if ( mObjectWindow && mCurrentObject && mCurrentObject->GetName() == objectName )
			mObjectWindow->SetSelectedTab( "Object" );
	}

	void EditorSystem::PrefabListDoubleClickCallback( const String & prefabName )
	{
		if ( mObjectWindow && mCurrentPrefab && mCurrentPrefab->Name == prefabName )
			mObjectWindow->SetSelectedTab( "Prefab" );
	}

	void EditorSystem::UnpickGizmo( s32, s32 )
	{
		mGizmo.UnpickGizmo();
	}

	void EditorSystem::GizmoSetTranslateModeCalback()
	{
		if ( mGizmo.GetAxisPicked() == EditorGizmo::AXIS_TOTAL )
			mGizmo.SetMode( EditorGizmo::GIZMO_MOVE );
	}

	void EditorSystem::GizmoSetScaleModeCalback()
	{
		if ( mGizmo.GetAxisPicked() == EditorGizmo::AXIS_TOTAL )
			mGizmo.SetMode( EditorGizmo::GIZMO_SCALE );
	}

	void EditorSystem::GizmoSetRotateModeCalback()
	{
		if ( mGizmo.GetAxisPicked() == EditorGizmo::AXIS_TOTAL )
			mGizmo.SetMode( EditorGizmo::GIZMO_ROTATE );
	}

	void EditorSystem::InitLevelList()
	{
		if ( !mObjectWindow )
			return;

		if ( mLevelListBox )
			return;

		const LevelManager * levelManager = SYSTEM_MANAGER.GetGameComponentFromSystem<BH::LevelManager>();
		mCurrentLevelName = levelManager->GetCurrentLevel();
		mLevelListBox = mObjectWindow->AddListBox( "LevelListBox", "Levels", "Levels", &mCurrentLevelName, false );

		for ( const auto & i : levelManager->GetLevelList() )
		{
			mLevelListBox->AddItem( i );
		}

		mLevelListBox->SetOnDblClickCallback( std::bind( &EditorSystem::LevelListDoubleClickCallback, this, std::placeholders::_1 ) );

		mObjectWindow->AddButton( "EditorGoToSelectedLevelButton", "Levels", "Load Level", 
								  std::bind( &EditorSystem::GoToSelectedLevel, this ) );

		mObjectWindow->AddButton( "EditorDeleteSelectedLevelButton", "Levels", "Delete Level", 
								  std::bind( &EditorSystem::DeleteSelectedLevel, this ) );

		mObjectWindow->AddDivider( "LevelListBoxDivider", "Levels" );

		mObjectWindow->AddEditBox<String>( "EditorAddNewLevelName", "Levels", "New Level Name", &mNewLevelName, false );
		mObjectWindow->AddButton( "EditorAddNewLevelButton", "Levels", "Add Level", 
								  std::bind( &EditorSystem::AddNewLevel, this ) );

		mObjectWindow->AddDivider("LevelSaveDivider", "Levels");
		mObjectWindow->AddButton( "EditorSaveLevelButton", "Levels", "Save Level", 
								  std::bind( &EditorSystem::ButtonSaveCallback, this ) );

		mLevelListBox->SetSelection( mCurrentLevelName );
	}

	void EditorSystem::ClearLevelList()
	{
		if ( !mObjectWindow )
			return;

		if ( !mLevelListBox )
			return;

		mObjectWindow->RemoveListBox( "LevelListBox" );
		mObjectWindow->RemoveButton( "EditorGoToSelectedLevelButton" );
		mObjectWindow->RemoveButton( "EditorDeleteSelectedLevelButton" );
		mObjectWindow->RemoveDivider( "LevelListBoxDivider" );
		mObjectWindow->RemoveEditBox( "EditorAddNewLevelName" );
		mObjectWindow->RemoveButton( "EditorAddNewLevelButton" );
		mObjectWindow->RemoveDivider( "LevelSaveDivider" );
		mObjectWindow->RemoveButton( "EditorSaveLevelButton" );

		mLevelListBox = nullptr;
	}

	void EditorSystem::LevelListDoubleClickCallback( const String & levelName )
	{
		mCurrentLevelName = levelName;
		GoToSelectedLevel();
	}

	void EditorSystem::GoToSelectedLevel()
	{
		mLevelChange = SYSTEM_MANAGER.GetGameComponentFromSystem<BH::LevelManager>()->GetCurrentLevel() != mCurrentLevelName;
	}

	void EditorSystem::DeleteSelectedLevel()
	{
		LevelManager * levelManager = SYSTEM_MANAGER.GetGameComponentFromSystem<BH::LevelManager>();

		// If last level remaining, prevent it to delete
		if ( levelManager->GetLevelList().size() <= 1 )
		{
			// @todo: log it
			return;
		}

		if ( levelManager->GetCurrentLevel() == mCurrentLevelName )
		{
			mLevelDelete = true;
		}
		else
		{
			levelManager->DeleteLevel( mCurrentLevelName );
			RefreshLevelList();
		}
	}

	void EditorSystem::AddNewLevel()
	{
		if ( mNewLevelName == "[New Level Name]" )
			return;

		SYSTEM_MANAGER.GetGameComponentFromSystem<BH::LevelManager>()->CreateLevel( mNewLevelName );

		RefreshLevelList();
	}

	void EditorSystem::RefreshLevelList()
	{
		if ( !mLevelListBox )
			return;

		mLevelListBox->ClearItems();

		for ( const auto & i : SYSTEM_MANAGER.GetGameComponentFromSystem<BH::LevelManager>()->GetLevelList() )
		{
			mLevelListBox->AddItem( i );
		}
	}

	void EditorSystem::FocusObjectCallback()
	{
		if ( mCurrentObject && mCurrentObjectInfo.mPositionX && mCurrentObjectInfo.mModelName )
		{
			Vector3f pos( *mCurrentObjectInfo.mPositionX, *mCurrentObjectInfo.mPositionY, *mCurrentObjectInfo.mPositionZ );
			Matrix4 rotation = Matrix4::CreateFromYawPitchRoll( Math::DegToRad( *mCurrentObjectInfo.mRotationY ), 
																Math::DegToRad( *mCurrentObjectInfo.mRotationX ), 
																Math::DegToRad( *mCurrentObjectInfo.mRotationZ ) );
			Matrix4 trans = Matrix4::CreateTranslation( pos );
			Matrix4 scale = Matrix4::CreateScale( *mCurrentObjectInfo.mScaleX, *mCurrentObjectInfo.mScaleY, *mCurrentObjectInfo.mScaleZ );

			AABB aabb = SYSTEM_MANAGER.GetGameComponentFromSystem<ModelManager>()->GetAABB( *mCurrentObjectInfo.mModelName );
			SYSTEM_MANAGER.GetGameComponentFromSystem<DebugCamera>()->FocusOn( trans * rotation * scale,
																			   aabb );
		}
	}

	void EditorSystem::InitGraphicsVariables()
	{
		if ( !mObjectWindow )
			return;

		mObjectWindow->AddEditBox<f32>( "GraphicsTraceSampleRate", "Graphics", "Sample Rate", &Renderer::TraceSampleRate, false );
		mObjectWindow->AddCheckBox( "GraphicsTraceDownSample", "Graphics", "Down Sample", &Renderer::TraceDownSample, false );
		mObjectWindow->AddCheckBox( "GraphicsTraceJitter", "Graphics", "Jitter", &Renderer::TraceJitter, false );
		mObjectWindow->AddCheckBox( "GraphicsTraceEdgeBlur", "Graphics", "Edge Blur", &Renderer::TraceEdgeBlur, false );
		mObjectWindow->AddCheckBox( "GraphicsShowSkyDome", "Graphics", "Show Sky Dome", &Renderer::ShowSkyDome, false );
		mObjectWindow->AddCheckBox( "GraphicsShowLight", "Graphics", "Show Light", &Renderer::ShowDebugLight, false );
	}

	void EditorSystem::ClearGraphicsVariables()
	{
		if ( !mObjectWindow )
			return;

		mObjectWindow->RemoveEditBox( "GraphicsTraceSampleRate" );
		mObjectWindow->RemoveCheckBox( "GraphicsTraceDownSample" );
		mObjectWindow->RemoveCheckBox( "GraphicsTraceJitter" );
		mObjectWindow->RemoveCheckBox( "GraphicsTraceEdgeBlur" );
		mObjectWindow->RemoveCheckBox( "GraphicsShowSkyDome" );
		mObjectWindow->RemoveCheckBox( "GraphicsShowLight" );
	}
}
#endif