#include "Platform/Platform.h"

#include "Core/CoreEssential.h"
#include "Core/Hash.h"
#include "Core/Game.h"
#include "Core/SystemManager.h"
#include "Core/Renderer.h"
#include "Core/System.h"
#include "Core/InputManager.h"
#include "Core/MaterialManager.h"
#include "Core/ModelManager.h"

#include "Editor/MaterialEditor.h"
#include "Editor/MaterialEditorCamera.h"
#include "Editor/EditorUIManager.h"
#include "Editor/EditorReflection.h"
#include "Editor/EditorListBox.h"
#include "Editor/EditorApplication.h"

#include "Platform/Graphics.h"
#include "Platform/GraphicsPipeline.h"
#include "Platform/MeshLoader.h"

#if GAME_EDITOR_MODE
namespace BH
{
	MaterialEditor::MaterialEditor()
	: mMaterialManager( nullptr )
	, mActive( false )
	, mMaterialWindow( nullptr )
	, mMaterialListBox( nullptr )
	, mCurrentMaterial( nullptr )
	, mCurrentMaterialName( "" )
	, mCtrlPressed( false )
	{
	}

	MaterialEditor::~MaterialEditor()
	{
	}

	void MaterialEditor::Initialise()
	{
		mMaterialManager = SYSTEM_MANAGER.GetGameComponentFromSystem<MaterialManager>();
		BH_ASSERT( mMaterialManager, "Material Manager does not exist!" );

		// Add input callback
		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::F3, Input::Triggered, 
																				Delegate<void()>::FromMethod<E_METHOD( MaterialEditor, ToogleEditorState )>( this ) );
	}

	void MaterialEditor::Shutdown()
	{
		if ( mActive )
			RemoveEditorLayout();

		// Remove input callback
		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::F3, Input::Triggered, 
																				   Delegate<void()>::FromMethod<E_METHOD( MaterialEditor, ToogleEditorState )>( this ) );
	}

	void MaterialEditor::Draw( f32 )
	{
		if ( !mActive )
			return;

		// @todo: Draw Sphere with attached material
		if ( mCurrentMaterial )
		{
			SYSTEM_MANAGER.GetGameComponentFromSystem<Renderer>()->DrawInstance( Matrix4::IDENTITY,
																				 SYSTEM_MANAGER.GetGameComponentFromSystem<ModelManager>()->GetModel( "Sphere" ),
																				 mCurrentMaterial );
		}
	}

	void MaterialEditor::ToogleEditorState()
	{
		if ( mActive )
			RemoveEditorLayout();
		else
			CreateEditorLayout();
	}

	void MaterialEditor::CreateEditorLayout()
	{
		if ( mActive )
			return;

		SYSTEM_MANAGER.PauseObjectComponents();
		SYSTEM_MANAGER.HideObjectComponents();
		SYSTEM_MANAGER.GetGameComponentFromSystem<Renderer>()->HideSkyDome( true );
		SYSTEM_MANAGER.GetGameComponentFromSystem<MaterialEditorCamera>()->SetEditorCameraActive( true );

		// Create window
		mMaterialWindow = EDITOR_UI.CreateEditorWindow( "Material Editor", Vector2f::ZERO, Vector2f( 0.2f, 1.0f ) );
		mActive = true;

		// Add all objects to object list (with callbacks)
		InitMaterialList();

		// Set callback
		mMaterialWindow->SetTabOnChangeCallback( std::bind( &MaterialEditor::TabChangedCallback, this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::LCtrl, Input::Triggered, 
																				Delegate<void()>::FromMethod<E_METHOD( MaterialEditor, ControlPressedCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::LCtrl, Input::Released, 
																				Delegate<void()>::FromMethod<E_METHOD( MaterialEditor, ControlReleaseCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->AddKeyEvent( Key::S, Input::Triggered, 
																				Delegate<void()>::FromMethod<E_METHOD( MaterialEditor, SaveCallback )>( this ) );
	}

	void MaterialEditor::RemoveEditorLayout()
	{
		if ( !mActive )
			return;

		SYSTEM_MANAGER.ResumeObjectComponents();
		SYSTEM_MANAGER.ShowObjectComponents();
		SYSTEM_MANAGER.GetGameComponentFromSystem<Renderer>()->HideSkyDome( false );
		SYSTEM_MANAGER.GetGameComponentFromSystem<MaterialEditorCamera>()->SetEditorCameraActive( false );

		// Remove exisiting material from editor
		DeselectMaterial();

		// Remove all exisiting material from material list
		ClearMaterialList();

		// Destroy window
		if ( mMaterialWindow )
			EDITOR_UI.DestroyEditorWindow( mMaterialWindow );

		mMaterialWindow = nullptr;
		mActive = false;
		mCurrentMaterialName = "";
		mCtrlPressed = false;
		mCurrentMaterialOrigName = "";

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::LCtrl, Input::Triggered, 
																				   Delegate<void()>::FromMethod<E_METHOD( MaterialEditor, ControlPressedCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::LCtrl, Input::Released, 
																				   Delegate<void()>::FromMethod<E_METHOD( MaterialEditor, ControlReleaseCallback )>( this ) );

		SYSTEM_MANAGER.GetGameComponentFromSystem<InputManager>()->RemoveKeyEvent( Key::S, Input::Triggered, 
																				   Delegate<void()>::FromMethod<E_METHOD( MaterialEditor, SaveCallback )>( this ) );
	}

	void MaterialEditor::DeselectMaterial()
	{
		if ( mMaterialWindow && mCurrentMaterial )
		{
			//if ( mMaterialListBox )
			//	mMaterialListBox->Unselect();

			// If name is changed
			if ( mCurrentMaterial->Name != mCurrentMaterialOrigName )
			{
				mMaterialManager->AssignAvailableMaterialName( mCurrentMaterial->Name );
				mMaterialManager->RemoveMaterial( mCurrentMaterialOrigName, false );
				mMaterialManager->SetMaterial( mCurrentMaterial->Name, mCurrentMaterial );
				
				RefreshMaterialList();
			}

			EDITOR_REFLECTION.RemoveFromEditor( mMaterialWindow, ENTITY_MANAGER.Types.Get<Material>(), mCurrentMaterial );
			mCurrentMaterial = nullptr;
			mCurrentMaterialOrigName = "";
		}
	}

	void MaterialEditor::SelectMaterial( Material * material )
	{
		if ( !material || material == mCurrentMaterial )
			return;

		DeselectMaterial();
		EDITOR_REFLECTION.AddToEditor( mMaterialWindow, ENTITY_MANAGER.Types.Get<Material>(), material );
		mCurrentMaterial = material;
		mCurrentMaterialOrigName = material->Name;

		//mObjectWindow->SetSelectedTab( "Material" );
	}

	void MaterialEditor::InitMaterialList()
	{
		if ( !mMaterialWindow )
			return;

		if ( mMaterialListBox )
			return;

		mMaterialListBox = mMaterialWindow->AddListBox( "MaterialListBox", "Material List", "Material List", &mCurrentMaterialName, false );
		for ( const auto & i : mMaterialManager->GetMaterialList() )
		{
			if ( i.second )
				mMaterialListBox->AddItem( i.first );
		}

		mMaterialListBox->SetOnChangeCallback( std::bind( &MaterialEditor::MaterialListChangeCallback, this ) );
		mMaterialListBox->SetOnDblClickCallback( std::bind( &MaterialEditor::MaterialListDoubleClickCallback, this, std::placeholders::_1 ) );

		mMaterialWindow->AddButton( "EditorDeleteSelectedButton", "Material List", "Delete Material", 
								  std::bind( &MaterialEditor::DeleteSelectedMaterial, this ) );

		mMaterialWindow->AddDivider( "MaterialListBoxDivider", "Material List" );

		mMaterialWindow->AddButton( "EditorAddNewButton", "Material List", "Add New Material", 
									std::bind( &MaterialEditor::AddNewMaterial, this ) );

		mMaterialWindow->AddDivider( "MaterialSaveDivider", "Material List" );

		mMaterialWindow->AddButton( "EditorSaveButton", "Material List", "Save Material", 
									std::bind( &MaterialEditor::ButtonSaveCallback, this ) );
	}

	void MaterialEditor::ClearMaterialList()
	{
		if ( !mMaterialWindow )
			return;

		if ( !mMaterialListBox )
			return;

		mMaterialWindow->RemoveListBox( "MaterialListBox" );
		mMaterialWindow->RemoveButton( "EditorDeleteSelectedButton" );
		mMaterialWindow->RemoveDivider( "MaterialListBoxDivider" );
		mMaterialWindow->RemoveButton( "EditorAddNewButton" );
		mMaterialWindow->RemoveDivider( "MaterialSaveDivider" );
		mMaterialWindow->RemoveButton( "EditorSaveButton" );
		mMaterialListBox = nullptr;
	}

	void MaterialEditor::RefreshMaterialList()
	{
		if ( !mMaterialListBox )
			return;

		mMaterialListBox->ClearItems();

		bool nameExist = false;
		for ( const auto & i : mMaterialManager->GetMaterialList() )
		{
			if ( i.second )
			{
				mMaterialListBox->AddItem( i.first );
				nameExist = nameExist ? true : i.first == mCurrentMaterialName;
			}
		}

		// Name does not exist, reset it
		if ( !nameExist )
		{
			mCurrentMaterialName = "";
			mMaterialListBox->Unselect();
		}
		else
			mMaterialListBox->SetSelection( mCurrentMaterialName );
	}

	void MaterialEditor::AddNewMaterial()
	{
		// To add names back into object name table
		DeselectMaterial();

		Material * material = ENTITY_MANAGER.New<Phong>();
		material->Name = "NewMaterial";
		mMaterialManager->AssignAvailableMaterialName( material->Name );
		mMaterialManager->SetMaterial( material->Name, material );

		if ( !material )
			return;

		mCurrentMaterialName = material->Name;
		RefreshMaterialList();
		SelectMaterial( material );
		mMaterialWindow->SetSelectedTab( "Phong" );
	}

	void MaterialEditor::DeleteSelectedMaterial()
	{
		if ( !mCurrentMaterial )
			return;

		Material * deleting = mCurrentMaterial;
		DeselectMaterial();

		mMaterialManager->RemoveMaterial( deleting->Name );

		mCurrentMaterialName = "";
		RefreshMaterialList();
	}

	void MaterialEditor::MaterialListChangeCallback()
	{
		if ( !mCurrentMaterial || mCurrentMaterialName != mCurrentMaterial->Name )
		{
			SelectMaterial( mMaterialManager->GetMaterial( mCurrentMaterialName ) );
		}
	}

	void MaterialEditor::TabChangedCallback()
	{
		String tab = mMaterialWindow->GetSelectedTab();

		if ( tab == "Material List" )
		{
			// Object rename
			if ( mCurrentMaterial && mCurrentMaterial->Name != mCurrentMaterialName )
			{
				mMaterialManager->AssignAvailableMaterialName( mCurrentMaterial->Name );
				mMaterialManager->RemoveMaterial( mCurrentMaterialOrigName, false );
				mMaterialManager->SetMaterial( mCurrentMaterial->Name, mCurrentMaterial );
				mCurrentMaterialOrigName = mCurrentMaterial->Name;
				mCurrentMaterialName = mCurrentMaterial->Name;

				RefreshMaterialList();
				mMaterialWindow->SetSelectedTab( "Material List" );
			}
		}
	}

	void MaterialEditor::SaveMaterialListCallback()
	{
		Material * material = mCurrentMaterial;
		DeselectMaterial();
		mMaterialManager->SaveAllMaterials();
		SelectMaterial( material );
	}

	void MaterialEditor::ButtonSaveCallback()
	{
		mCtrlPressed = true;
		SaveCallback();
		mCtrlPressed = false;
	}

	void MaterialEditor::SaveCallback()
	{
		if ( !mCtrlPressed )
			return;

		String activeTab;
		if ( mMaterialWindow )
			activeTab = mMaterialWindow->GetSelectedTab();

		SaveMaterialListCallback();

		if ( mMaterialWindow )
			mMaterialWindow->SetSelectedTab( activeTab );
	}

	void MaterialEditor::ControlPressedCallback()
	{
		mCtrlPressed = true;
	}

	void MaterialEditor::ControlReleaseCallback()
	{
		mCtrlPressed = false;
	}

	void MaterialEditor::MaterialListDoubleClickCallback( const String & materialName )
	{
		if ( mMaterialWindow && mCurrentMaterial && mCurrentMaterial->Name == materialName )
			mMaterialWindow->SetSelectedTab( "Phong" );
	}
}
#endif	// GAME_EDITOR_MODE