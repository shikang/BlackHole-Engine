#include "Editor/EditorReflection.h"
#include "Editor/EditorEditBox.h"
#include "Editor/EditorComboBox.h"

#include "Game/ScriptLoader.h"

#include "Core/TypeDB.h"
#include "Core/LogManager.h"
#include "Core/EntityManager.h"
#include "Core/TypeRegistrar.h"
#include "Core/EntityManager.h"
#include "Core/ObjectComponent.h"
#include "Core/MeshManager.h"
#include "Core/TextureManager.h"
#include "Core/MaterialManager.h"

#include "Script/ScriptComponent.h"

#include "Editor/EditorApplication.h"

#define CREATE_GROUP_FOR_CONTAINER 0

#if GAME_EDITOR_MODE
namespace BH
{
	EditorReflection::AllocatedStringList EditorReflection::AllocatedString;

	void EditorReflection::RegisterTypes()
	{
		// Base type
		AddEditorReflector<s8> ( AddTypeEditBoxImpl<s8>,  AddTypeEditBoxToGroupImpl<s8>,  RemoveEditBox );
		AddEditorReflector<s16>( AddTypeEditBoxImpl<s16>, AddTypeEditBoxToGroupImpl<s16>, RemoveEditBox );
		AddEditorReflector<s32>( AddTypeEditBoxImpl<s32>, AddTypeEditBoxToGroupImpl<s32>, RemoveEditBox );
		AddEditorReflector<s64>( AddTypeEditBoxImpl<s64>, AddTypeEditBoxToGroupImpl<s64>, RemoveEditBox );
		AddEditorReflector<u8> ( AddTypeEditBoxImpl<u8>,  AddTypeEditBoxToGroupImpl<u8>,  RemoveEditBox );
		AddEditorReflector<u16>( AddTypeEditBoxImpl<u16>, AddTypeEditBoxToGroupImpl<u16>, RemoveEditBox );
		AddEditorReflector<u32>( AddTypeEditBoxImpl<u32>, AddTypeEditBoxToGroupImpl<u32>, RemoveEditBox );
		AddEditorReflector<u64>( AddTypeEditBoxImpl<u64>, AddTypeEditBoxToGroupImpl<u64>, RemoveEditBox );
		AddEditorReflector<f32>( AddTypeEditBoxImpl<f32>, AddTypeEditBoxToGroupImpl<f32>, RemoveEditBox );
		AddEditorReflector<f64>( AddTypeEditBoxImpl<f64>, AddTypeEditBoxToGroupImpl<f64>, RemoveEditBox );
		AddEditorReflector<String>( AddTypeEditBoxImpl<String>, AddTypeEditBoxToGroupImpl<String>, RemoveEditBox );
		AddEditorReflector<Phong>( AddPhongImpl, AddPhongToGroupImpl, RemovePhong );
		AddEditorReflector<bool>( AddCheckBoxImpl, AddCheckBoxToGroupImpl, RemoveCheckBox );
		AddEditorChanged<ScriptType>( AddScriptTypeImpl, AddScriptTypeToGroupImpl, RemoveScriptType );
		CName meshName( "BHMesh" ); CName textureName( "BHTexture" ); CName materialName( "BHMaterial" );
		AddEditorReflector( meshName, AddScriptMeshImpl, AddScriptMeshToGroupImpl, RemoveScriptMesh );
		AddEditorReflector( textureName, AddScriptTextureImpl, AddScriptTextureToGroupImpl, RemoveScriptTexture );
		AddEditorReflector( materialName, AddScriptMaterialImpl, AddScriptMaterialToGroupImpl, RemoveScriptMaterial );

		// Loop through all type in data base
		for( const auto & i : ENTITY_MANAGER.Types.GetTypes() )
		{
			// If serialiser specified for this type
			if ( TYPE_STRINGIFIER.GetFromString( i.second ) && TYPE_STRINGIFIER.GetToString( i.second ) )
			{
				// And types has not yet registered to editor reflection, add it
				if ( mTabReflectorList.find(i.second->Name) == mTabReflectorList.end() )
				{
					AddEditorReflector( i.second,
										std::bind( &EditorReflection::AddSerialiserDefinedImpl,
												   i.second, std::placeholders::_1, std::placeholders::_2,
												   std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6 ),
										std::bind( &EditorReflection::AddSerialiserDefinedToGroupImpl,
												   i.second, std::placeholders::_1, std::placeholders::_2,
												   std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6 ),
										RemoveSerialiserDefineImpl );
				}
			}
		}

		// Rest of the engine specific stuff
		AddEditorInsertNew<ObjectComponent>( ObjectComponentInsertNew, ObjectComponentRemoveDelete );
		AddEditorInsertNew<ScriptComponent>( ObjectComponentInsertNew, ObjectComponentRemoveDelete );
	}

	EditorReflection::EditorReflection()
	{

	}

	EditorReflection::~EditorReflection()
	{

	}

	void EditorReflection::Update()
	{
		if ( !mResetCallback )
			return;

		mResetCallback();
		mResetCallback = std::function < void() >();
	}

	void EditorReflection::SetResetCallback( const std::function< void() > & cb )
	{
		mResetCallback = cb;
	}

	void EditorReflection::AddEditorReflector( CName & name, const EditorTabReflector & tab_reflector,
											   const EditorGroupReflector & group_reflector,
											   const EditorRemover & remover )
	{
		mTabReflectorList[name] = tab_reflector;
		mGroupReflectorList[name] = group_reflector;
		mRemoverList[name] = remover;
	}
	
	void EditorReflection::AddEditorReflector( const Type * type, const EditorTabReflector & tab_reflector,
											   const EditorGroupReflector & group_reflector,
											   const EditorRemover & remover )
	{
		mTabReflectorList[type->Name] = tab_reflector;
		mGroupReflectorList[type->Name] = group_reflector;
		mRemoverList[type->Name] = remover;
	}

	void EditorReflection::AddEditorInsertNew( CName & name, const EditorInsertNew & insert_new,
											   const EditorRemoveDelete & remove_delete )
	{
		mInsertElementList[name] = insert_new;
		mRemoveElementList[name] = remove_delete;
	}
	
	void EditorReflection::AddEditorInsertNew( const Type * type, const EditorInsertNew & insert_new,
											   const EditorRemoveDelete & remove_delete )
	{
		mInsertElementList[type->Name] = insert_new;
		mRemoveElementList[type->Name] = remove_delete;
	}

	void EditorReflection::AddEditorChanged( CName & name, const EditorTabChanged & tab_reflector,
											 const EditorGroupChanged & group_reflector,
											 const EditorRemover & remover)
	{
		mTabChangedList[name] = tab_reflector;
		mGroupChangedList[name] = group_reflector;
		mRemoverList[name] = remover;
	}

	// Add editor changed through type pointer
	void EditorReflection::AddEditorChanged( const Type * type, const EditorTabChanged & tab_reflector,
											 const EditorGroupChanged & group_reflector,
											 const EditorRemover & remover )
	{
		mTabChangedList[type->Name] = tab_reflector;
		mGroupChangedList[type->Name] = group_reflector;
		mRemoverList[type->Name] = remover;
	}
	
	EditorReflection::EditorTabReflector EditorReflection::GetTabReflector( const CName & name )
	{
		return mTabReflectorList[name];
	}

	EditorReflection::EditorGroupReflector EditorReflection::GetGroupReflector( const CName & name )
	{
		return mGroupReflectorList[name];
	}

	EditorReflection::EditorRemover EditorReflection::GetRemover( const CName & name )
	{
		return mRemoverList[name];
	}
	
	EditorReflection::EditorTabReflector EditorReflection::GetTabReflector( const Type * type )
	{
		return mTabReflectorList[type->Name];
	}

	EditorReflection::EditorGroupReflector EditorReflection::GetGroupReflector( const Type * type )
	{
		return mGroupReflectorList[type->Name];
	}

	EditorReflection::EditorRemover EditorReflection::GetRemover( const Type * type )
	{
		return mRemoverList[type->Name];
	}

	void EditorReflection::RemoveEditBox( EditorWindow * window, const String & name )
	{
		window->RemoveEditBox( name );
	}

	void EditorReflection::RemoveCheckBox( EditorWindow * window, const String & name )
	{
		window->RemoveCheckBox( name );
	}

	void EditorReflection::RemoveColourPicker( EditorWindow * window, const String & name )
	{
		window->RemoveColourPicker( name );
	}

	void EditorReflection::AddCheckBoxImpl( EditorWindow * window, const String & name, const String & tab_name, 
											const String & label, void * var, bool read_only )
	{
		window->AddCheckBox( name, tab_name, label, reinterpret_cast<bool *>( var ), read_only );
	}

	void EditorReflection::AddCheckBoxToGroupImpl( EditorWindow * window, const String & name, const String & group_name, 
												   const String & label, void * var, bool read_only )
	{
		window->AddCheckBoxToGroup( name, group_name, label, reinterpret_cast<bool *>( var ), read_only );
	}

	void EditorReflection::AddColourPicker( EditorWindow * window, const String & name, const String & tab_name, 
											const String & label, void * var, bool read_only )
	{
		window->AddColourPicker( name, tab_name, label, reinterpret_cast<Color *>( var ), read_only );
	}

	void EditorReflection::AddColourPickerToGroup( EditorWindow * window, const String & name, const String & group_name, 
												   const String & label, void * var, bool read_only )
	{
		window->AddColourPickerToGroup( name, group_name, label, reinterpret_cast<Color *>( var ), read_only );
	}

	void EditorReflection::AddSerialiserDefinedImpl( const Type * type, EditorWindow * window, const String & name, 
													 const String & tab_name, const String & label, void * var, bool read_only )
	{
		AllocatedStringList::iterator it = AllocatedString.find( name );
		if ( it != AllocatedString.end() )
		{
			LOGWARNING( "Add to editor UI - Repeated names: " << name );
			return;
		}

		TYPE_STRINGIFIER.GetToString( type )( AllocatedString[name], type, var, nullptr );
		EditorEditBox * editbox = window->AddEditBox( name, tab_name, label, &AllocatedString[name], read_only );
		std::function< void( void * ) > refreshCallback = std::bind( &EditorReflection::RefreshCallback, 
																	 TYPE_STRINGIFIER.GetFromString( type ), 
																	 type, var, std::placeholders::_1 );
		editbox->AddRefreshCallback( refreshCallback );
	}

	void EditorReflection::AddSerialiserDefinedToGroupImpl( const Type * type, EditorWindow * window, const String & name, 
															const String & group_name, const String & label, void * var, bool read_only )
	{
		AllocatedStringList::iterator it = AllocatedString.find( name );
		if ( it != AllocatedString.end() )
		{
			LOGWARNING( "Add to editor UI - Repeated names: " << name );
			return;
		}

		TYPE_STRINGIFIER.GetToString( type )( AllocatedString[name], type, var, nullptr );
		EditorEditBox * editbox = window->AddEditBoxToGroup( name, group_name, label, &AllocatedString[name], read_only );
		std::function< void( void * ) > refreshCallback = std::bind( &EditorReflection::RefreshCallback, 
																	 TYPE_STRINGIFIER.GetFromString( type ), 
																	 type, var, std::placeholders::_1 );
		editbox->AddRefreshCallback( refreshCallback );
	}

	void EditorReflection::RemoveSerialiserDefineImpl( EditorWindow * window, const String & name )
	{
		AllocatedStringList::iterator it = AllocatedString.find( name );
		if ( it == AllocatedString.end() )
		{
			LOGWARNING( "Remove from editor UI - no such names: " << name );
			return;
		}

		window->RemoveEditBox( name );
		AllocatedString.erase( it );
	}

	void EditorReflection::AddAllScriptTypeOption( EditorComboBox * combobox )
	{
		combobox->AddItem( "[InvalidType]" );

		for ( const auto & i : ScriptComponent::GetAllScriptTypes() )
		{
			combobox->AddItem( i );
		}
	}

	void EditorReflection::AddScriptTypeImpl( EditorWindow * window, const String & name, const String & tab_name, 
											  const String & label, void * var, bool read_only, 
											  const Type * type, void * obj )
	{
		ScriptType * st = reinterpret_cast<ScriptType *>( var );
		String * svar = &( st->mType );
		read_only = read_only ? read_only : ( *svar != "[InvalidType]" );

		EditorComboBox * combobox = window->AddComboBox( name, tab_name, label, svar, read_only );
		AddAllScriptTypeOption( combobox );
		combobox->SetOnChangeCallback( std::bind( &EditorReflection::ScriptChangedCallback, window, type, obj, st->mParent ) );
	}

	void EditorReflection::AddScriptTypeToGroupImpl( EditorWindow * window, const String & name, const String & group_name, 
													 const String & label, void * var, bool read_only, 
													 const Type * type, void * obj )
	{
		ScriptType * st = reinterpret_cast<ScriptType *>( var );
		String * svar = &( st->mType );
		read_only = read_only ? read_only : ( *svar != "[InvalidType]" );
		
		EditorComboBox * combobox = window->AddComboBoxToGroup( name, group_name, label, svar, read_only );
		AddAllScriptTypeOption( combobox );
		combobox->SetOnChangeCallback( std::bind( &EditorReflection::ScriptChangedCallback, window, type, obj, st->mParent ) );
	}

	void EditorReflection::RemoveScriptType( EditorWindow * window, const String & name )
	{
		window->RemoveComboBox( name );
	}

	void EditorReflection::ScriptChangedCallback( EditorWindow * window, const Type * type, void * obj, 
												  ScriptComponent * component )
	{
		EDITOR_REFLECTION.SetResetCallback( std::bind( &EditorReflection::ScriptResetCallback, window,type, obj, component ) );
	}

	void EditorReflection::ScriptResetCallback( EditorWindow * window, const Type * type, void * obj, 
												ScriptComponent * component )
	{
		String scriptType = component->GetScript().mType;
		component->SetScriptType( "[InvalidType]" );
		EDITOR_REFLECTION.RemoveFromEditor( window, type, obj );
		component->SetScriptType( scriptType );
		component->Initialise();
		EDITOR_REFLECTION.AddToEditor( window, type, obj );
		window->SetSelectedTab( type->Name.Text );

		SYSTEM_MANAGER.GetGameComponentFromSystem<EditorSystem>()->RepickGizmo();
	}

	void EditorReflection::AddAllScriptMeshOption( EditorComboBox * combobox )
	{
		for ( const auto & i : SYSTEM_MANAGER.GetGameComponentFromSystem<MeshManager>()->GetMeshList() )
		{
			combobox->AddItem( i.first );
		}
	}

	void EditorReflection::AddScriptMeshImpl( EditorWindow * window, const String & name, const String & tab_name, 
											  const String & label, void * var, bool read_only )
	{
		const Type * scriptMeshType = ENTITY_MANAGER.Types.Get( "BHMesh" );
		String * svar = reinterpret_cast<String *>( scriptMeshType->GetField( "Mesh" )->GetPtr( var ) );

		EditorComboBox * combobox = window->AddComboBox( name, tab_name, label, svar, read_only );
		AddAllScriptMeshOption( combobox );
	}

	void EditorReflection::AddScriptMeshToGroupImpl( EditorWindow * window, const String & name, const String & group_name, 
													 const String & label, void * var, bool read_only )
	{
		const Type * scriptMeshType = ENTITY_MANAGER.Types.Get( "BHMesh" );
		String * svar = reinterpret_cast<String *>( scriptMeshType->GetField( "Mesh" )->GetPtr( var ) );
		
		EditorComboBox * combobox = window->AddComboBoxToGroup( name, group_name, label, svar, read_only );
		AddAllScriptMeshOption( combobox );
	}

	void EditorReflection::RemoveScriptMesh( EditorWindow * window, const String & name )
	{
		window->RemoveComboBox( name );
	}

	void EditorReflection::AddAllScriptTextureOption( EditorComboBox * combobox )
	{
		for ( const auto & i : SYSTEM_MANAGER.GetGameComponentFromSystem<TextureManager>()->GetTextureList() )
		{
			combobox->AddItem( i.first );
		}
	}

	void EditorReflection::AddScriptTextureImpl( EditorWindow * window, const String & name, const String & tab_name, 
												 const String & label, void * var, bool read_only )
	{
		const Type * scriptTextreType = ENTITY_MANAGER.Types.Get( "BHTexture" );
		String * svar = reinterpret_cast<String *>( scriptTextreType->GetField( "Texture" )->GetPtr( var ) );

		EditorComboBox * combobox = window->AddComboBox( name, tab_name, label, svar, read_only );
		AddAllScriptTextureOption( combobox );
	}

	void EditorReflection::AddScriptTextureToGroupImpl( EditorWindow * window, const String & name, const String & group_name, 
														const String & label, void * var, bool read_only )
	{
		const Type * scriptTextreType = ENTITY_MANAGER.Types.Get( "BHTexture" );
		String * svar = reinterpret_cast<String *>( scriptTextreType->GetField( "Texture" )->GetPtr( var ) );
		
		EditorComboBox * combobox = window->AddComboBoxToGroup( name, group_name, label, svar, read_only );
		AddAllScriptTextureOption( combobox );
	}

	void EditorReflection::RemoveScriptTexture( EditorWindow * window, const String & name )
	{
		window->RemoveComboBox( name );
	}

	void EditorReflection::AddAllScriptMaterialOption( EditorComboBox * combobox )
	{
		for ( const auto & i : SYSTEM_MANAGER.GetGameComponentFromSystem<MaterialManager>()->GetMaterialList() )
		{
			combobox->AddItem( i.first );
		}
	}

	void EditorReflection::AddScriptMaterialImpl( EditorWindow * window, const String & name, const String & tab_name, 
												  const String & label, void * var, bool read_only )
	{
		const Type * scriptMaterialType = ENTITY_MANAGER.Types.Get( "BHMaterial" );
		String * svar = reinterpret_cast<String *>( scriptMaterialType->GetField( "Material" )->GetPtr( var ) );

		EditorComboBox * combobox = window->AddComboBox( name, tab_name, label, svar, read_only );
		AddAllScriptMaterialOption( combobox );
	}

	void EditorReflection::AddScriptMaterialToGroupImpl( EditorWindow * window, const String & name, const String & group_name, 
														const String & label, void * var, bool read_only )
	{
		const Type * scriptMaterialType = ENTITY_MANAGER.Types.Get( "BHMaterial" );
		String * svar = reinterpret_cast<String *>( scriptMaterialType->GetField( "Material" )->GetPtr( var ) );
		
		EditorComboBox * combobox = window->AddComboBoxToGroup( name, group_name, label, svar, read_only );
		AddAllScriptMaterialOption( combobox );
	}

	void EditorReflection::RemoveScriptMaterial( EditorWindow * window, const String & name )
	{
		window->RemoveComboBox( name );
	}

	void EditorReflection::AddPhongImpl( EditorWindow * window, const String & name, const String & tab_name,
										 const String & , void * var, bool read_only )
	{
		Phong * p = reinterpret_cast<Phong *>( var );
		const Type * phongType = ENTITY_MANAGER.Types.Get<Phong>();

		window->AddEditBox<String>( name + "Name", tab_name, "Name", &( p->Name ), read_only );

		window->AddColourPicker( name + "Specular", tab_name, "Specular", 
								 reinterpret_cast<Vector3f *>( phongType->GetField( "Specular" )->GetPtr( p ) ),
								 read_only );

		window->AddEditBox<f32>( name + "Smoothness", tab_name, "Smoothness", 
								 reinterpret_cast<f32 *>( phongType->GetField( "Roughness" )->GetPtr( p ) ), 
								 read_only );

		EditorComboBox * combobox = window->AddComboBox( name + "Texture", tab_name, "Texture", 
														 reinterpret_cast<String *>( phongType->GetField( "Texture" )->GetPtr( p ) ), 
														 read_only );

		for ( const auto & i : SYSTEM_MANAGER.GetGameComponentFromSystem<TextureManager>()->GetTextureList() )
		{
			combobox->AddItem( i.first );
		}

		//window->AddEditBox<String>( name + "Texture", tab_name, "Texture", 
		//							reinterpret_cast<String *>( phongType->GetField( "Texture" )->GetPtr( p ) ), 
		//							read_only );

		//EditorComboBox * combobox = window->AddComboBox( name, tab_name, label, svar, read_only );
		//AddAllScriptTypeOption( combobox );
	}
		 
	void EditorReflection::AddPhongToGroupImpl( EditorWindow * window, const String & name, const String & group_name,
												const String & , void * var, bool read_only )
	{
		Phong * p = reinterpret_cast<Phong *>( var );
		const Type * phongType = ENTITY_MANAGER.Types.Get<Phong>();

		window->AddEditBoxToGroup<String>( name + "Name", group_name, "Name", &( p->Name ), read_only );

		window->AddColourPickerToGroup( name + "Specular", group_name, "Specular", 
										reinterpret_cast<Vector3f *>( phongType->GetField( "Specular" )->GetPtr( p ) ),
										read_only );

		window->AddEditBoxToGroup<f32>( name + "Smoothness", group_name, "Smoothness", 
										reinterpret_cast<f32 *>( phongType->GetField( "Roughness" )->GetPtr( p ) ), 
										read_only );

		EditorComboBox * combobox = window->AddComboBox( name + "Texture", group_name, "Texture", 
														 reinterpret_cast<String *>( phongType->GetField( "Texture" )->GetPtr( p ) ), 
														 read_only );

		for ( const auto & i : SYSTEM_MANAGER.GetGameComponentFromSystem<TextureManager>()->GetTextureList() )
		{
			combobox->AddItem( i.first );
		}

		//window->AddEditBoxToGroup<String>( name + "Texture", group_name, "Texture", 
		//								   reinterpret_cast<String *>( phongType->GetField( "Texture" )->GetPtr( p ) ), 
		//								   read_only );

		//EditorComboBox * combobox = window->AddComboBox( name, tab_name, label, svar, read_only );
		//AddAllScriptTypeOption( combobox );
	}

	void EditorReflection::RemovePhong( EditorWindow * window, const String & name )
	{
		window->RemoveEditBox( name + "Name" );
		window->RemoveColourPicker( name + "Specular" );
		window->RemoveEditBox( name + "Smoothness" );
		window->RemoveComboBox( name + "Texture" );
	}

	void EditorReflection::RefreshCallback( TypeStringifier::FmString fmstr, const Type * type, void * var,void * allocatedString )
	{
		fmstr( *reinterpret_cast<String *>( allocatedString ), type, var, nullptr );
	}

	void EditorReflection::InsertCallback( EditorWindow * window, const Type * type, void * obj,
										   const Field * field, void * container, void * key, void * value )
	{
		//std::vector<void *> * c = reinterpret_cast< std::vector<void *> * >( container );
		void * v = nullptr;

		// If there is a specific way to allocate it
		InsertElementList::iterator it = EDITOR_REFLECTION.mInsertElementList.find( field->Container->ValueType->Name );
		if ( it != EDITOR_REFLECTION.mInsertElementList.end() )
		{
			v = it->second();
			value = &v;
		}
		// Else if it is a C# type
		else if ( v = ScriptComponent::GetDefaultConstructedScriptData( field->Container->ValueType->Name.Text, 
																		String( field->Name.Text ) + field->Container->ValueType->Name.Text + "NewTemporaryElement" ) )
		{
			value = &v;
		}

		field->Container->Insert( container, key, value );
		EDITOR_REFLECTION.RemoveFromEditor( window, type, obj );
		EDITOR_REFLECTION.AddToEditor( window, type, obj );
		window->SetSelectedTab( type->Name.Text );

		// Hack
		SYSTEM_MANAGER.GetGameComponentFromSystem<EditorSystem>()->RepickGizmo();
	}

	void EditorReflection::RemoveCallback( EditorWindow * window, const Type * type, void * obj,
										   const String & name, const String & label, const String &,
										   const Field * field, void * container, void * key, void * value )
	{
		const Type * valueEntryType = nullptr;
		String copyName = name;
		String copyLabel = label;

		// Remove UI
		{
			const Type * keyType = field->Container->KeyType;
			const Type * valType = field->Container->ValueType;

			bool keyEntity = keyType ? keyType->IsEntity : false;
			bool valEntity = valType->IsEntity;

			const Type * keyEntryType = nullptr,
					   * valEntryType = nullptr;

			// If container has key
			if ( keyType ) 
			{
				// Get key type
				keyEntryType = ( keyEntity && &reinterpret_cast<ConstEntityPtr>( key )->GetType() ) 
							   ? &reinterpret_cast<ConstEntityPtr>( key )->GetType() 
							   : keyType;
			}

			// We remove the last element instead. As the function will do the rest
			BH::FieldReadIterator* it = NEW_FIELD_READIT( field->Container, container );
			u32 count = 0;

			// Finding the point where the iterator is and start removing it from there
			for ( ; it->IsValid(); it->IncNext() ) 
			{
				// If container has key
				if ( keyType ) 
				{
					// Get key pointer (Data)
					void * keyPtr = field->Container->IsKeyPointer 
									? const_cast<void *>( *reinterpret_cast<const void * const *>( it->GetKey() ) )
									: const_cast<void *>( it->GetKey() );

					if ( key == keyPtr )
					{
						void * valPtr = field->Container->IsValuePointer 
								? const_cast<void *>( *reinterpret_cast<const void * const *>( it->GetValue() ) ) 
								: const_cast<void *>( it->GetValue() );

						valueEntryType = ( valEntity && &reinterpret_cast<ConstEntityPtr>( valPtr )->GetType() ) 
										 ? &reinterpret_cast<ConstEntityPtr>( valPtr )->GetType()
										 : valType;
						break;
					}
				}

				// Get value pointer (Data)
				void * valPtr = field->Container->IsValuePointer 
								? const_cast<void *>( *reinterpret_cast<const void * const *>( it->GetValue() ) ) 
								: const_cast<void *>( it->GetValue() );

				if ( value == valPtr )
				{
					valueEntryType = ( valEntity && &reinterpret_cast<ConstEntityPtr>( valPtr )->GetType() ) 
									 ? &reinterpret_cast<ConstEntityPtr>( valPtr )->GetType()
									 : valType;

					break;
				}

				++count;
			}

			void * keyPtr = nullptr,
				 * valPtr = nullptr;

			// Remove from this element onwards!
			for ( ; it->IsValid(); it->IncNext() )
			{
				// If container has key
				if ( keyType ) 
				{
					// Get key pointer (Data)
					keyPtr = field->Container->IsKeyPointer 
							 ? const_cast<void *>( *reinterpret_cast<const void * const *>( it->GetKey() ) )
							 : const_cast<void *>( it->GetKey() );

					// Get key type
					keyEntryType = ( keyEntity && &reinterpret_cast<ConstEntityPtr>( keyPtr )->GetType() ) 
								   ? &reinterpret_cast<ConstEntityPtr>( keyPtr )->GetType() 
								   : keyType;
				}

				// Get value pointer (Data)
				valPtr = field->Container->IsValuePointer 
						 ? const_cast<void *>( *reinterpret_cast<const void * const *>( it->GetValue() ) ) 
						 : const_cast<void *>( it->GetValue() );
				
				// Get value type
				valEntryType = ( valEntity && &reinterpret_cast<ConstEntityPtr>( valPtr )->GetType() ) 
							   ? &reinterpret_cast<ConstEntityPtr>( valPtr )->GetType()
							   : valType;

				std::ostringstream ss;
				ss << " " << count;
				if ( keyType ) 
				{
					//void * keynode = DoSerialise(cont_current, keyEntryType, keyPtr, &field.Attributes, &field);
					//DoSerialise(keynode, valEntryType, valPtr, &field.Attributes, &field);

					String group_name = field->Name.Text + ss.str();

					// Add remove entry button
					String remove_button_name = copyName + copyLabel + " " + group_name;
					window->RemoveButton( remove_button_name );

					// Group for key
					String key_group_name = copyLabel + " " + group_name + " Key";
					EDITOR_REFLECTION.DoRemoveFromEditor( window, keyEntryType, keyPtr, copyName + key_group_name, "", key_group_name );

					// Group for value
					String val_group_name = copyLabel + " " + group_name + " Value";
					EDITOR_REFLECTION.DoRemoveFromEditor( window, valEntryType, valPtr, copyName + val_group_name, "", val_group_name );
				}
				else
				{
					// If entity, use it's name instead. ( Getting the name of inherited type )
					String group_name = valEntryType->IsEntity ? valEntryType->Name.Text + ss.str() : field->Name.Text + ss.str();

					EDITOR_REFLECTION.DoRemoveFromEditor( window, valEntryType, valPtr, copyName + copyLabel + " " + field->Name.Text + ss.str() , "", group_name );

					// Add remove entry button
					String remove_button_name = copyName + copyLabel + " " + group_name;
					window->RemoveButton( remove_button_name );
				}

				++count;
			}

			DEL_FIELD_IT( field->Container, it );
		}

		// Remove value from container
		if ( valueEntryType )	//!< If type null, means no ui removed!
		{
			void * v = value;
			RemoveElementList::iterator rit = EDITOR_REFLECTION.mRemoveElementList.find( valueEntryType->Name );
			if ( rit != EDITOR_REFLECTION.mRemoveElementList.end() )
			{
				rit->second( value );
				v = &value;
			}
			else if ( ScriptComponent::IsScriptType( valueEntryType->Name.Text ) )
			{
				v = &value;
			}

			field->Container->Remove( container, key, v );

			if ( rit == EDITOR_REFLECTION.mRemoveElementList.end() && ScriptComponent::IsScriptType( valueEntryType->Name.Text ) )
			{
				valueEntryType->Delete( value );
			}
		}

		EDITOR_REFLECTION.RemoveFromEditor( window, type, obj );
		EDITOR_REFLECTION.AddToEditor( window, type, obj );
		window->SetSelectedTab( type->Name.Text );

		SYSTEM_MANAGER.GetGameComponentFromSystem<EditorSystem>()->RepickGizmo();
	}

	void * EditorReflection::ObjectComponentInsertNew()
	{
		ObjectComponent * component = static_cast<ObjectComponent *>( ENTITY_MANAGER.New<ScriptComponent>() );

		// Hackery!
		component->mAddToSystem = &ObjectComponent::AddToAssignedSystem < ScriptComponent >;
		component->mRemoveFromSystem = &ObjectComponent::RemoveFromAssignedSystem < ScriptComponent >;

		//component->Initialise();
		//component->mAddToSystem( component );

		return component;
	}
	
	void EditorReflection::ObjectComponentRemoveDelete( void * data )
	{
		ObjectComponent * component = static_cast<ObjectComponent *>( data );

		component->mRemoveFromSystem( component );
		component->Shutdown();
		ENTITY_MANAGER.Delete( component );
	}

	void EditorReflection::AddToEditor( EditorWindow * window, const Type * type, void * obj )
	{
		if ( !type )
		{
			LOGERROR( "EditorReflection::AddToEditor - Adding unregistered or invalid type" );
			return;
		}

		if ( !obj )
		{
			LOGERROR( "EditorReflection::AddToEditor - No object to add" );
			return;
		}

		if ( !window )
		{
			LOGERROR( "EditorReflection::AddToEditor - No window to add on" );
			return;
		}

		if ( type->IsEntity )
			type = &( reinterpret_cast<ConstEntityPtr>( obj )->GetType() );

		DoAddToEditor( window, type, obj, type, obj, nullptr, type->Name.Text, "", type->Name.Text, false );
	}

	void EditorReflection::RemoveFromEditor( EditorWindow * window, const Type * type, void * obj )
	{
		if ( !type )
		{
			LOGERROR( "EditorReflection::AddToEditor - Adding unregistered or invalid type" );
			return;
		}

		if ( !obj )
		{
			LOGERROR( "EditorReflection::AddToEditor - No object to add" );
			return;
		}

		if ( !window )
		{
			LOGERROR( "EditorReflection::AddToEditor - No window to add on" );
			return;
		}

		if ( type->IsEntity )
			type = &( reinterpret_cast<ConstEntityPtr>( obj )->GetType() );

		DoRemoveFromEditor( window, type, obj, type->Name.Text, "", type->Name.Text );
	}

	void EditorReflection::DoAddToEditor( EditorWindow * window, const Type * root_type, void * root_obj, const Type * type, void * obj, 
										  const Field * field, const String & name, const String & label, 
										  const String & tab_or_group, bool group )
	{
		Entity::OPTIONS opt = Entity::O_NORMAL;

		if ( type->IsEntity )
		{
			opt = reinterpret_cast<EntityPtr>( obj )->OnPreChanged();

			if ( opt == Entity::O_SKIP )
				return;
		}

		// If built in type or serialiser for type is specified, we add it to editor, done!
		if ( group )
		{
			// Add to group
			GroupReflectorList::iterator it = mGroupReflectorList.find( type->Name );
			if ( it != mGroupReflectorList.end() )
			{
				if ( it->second )
				{
					// Label = Value when the first pass in base type
					String finalLabel = ( label == "" ) ? "Value" : label;
					it->second( window, name + label, tab_or_group, finalLabel, obj, field ? field->Attributes.ReadOnly == 1 : false );
					return;
				}
			}

			GroupChangedList::iterator cit = mGroupChangedList.find( type->Name );
			if ( cit != mGroupChangedList.end() )
			{
				if ( cit->second )
				{
					// Label = Value when the first pass in base type
					String finalLabel = ( label == "" ) ? "Value" : label;
					cit->second( window, name + label, tab_or_group, finalLabel, obj, 
								 field ? field->Attributes.ReadOnly == 1 : false, root_type, root_obj );
					return;
				}
			}
		}
		else
		{
			// Add to tab
			TabReflectorList::iterator it = mTabReflectorList.find( type->Name );
			if ( it != mTabReflectorList.end() )
			{
				if ( it->second )
				{
					// Label = Value when the first pass in base type
					String finalLabel = ( label == "" ) ? "Value" : label;
					it->second( window, name + label, tab_or_group, finalLabel, obj, field ? field->Attributes.ReadOnly == 1 : false );
					return;
				}
			}

			TabChangedList::iterator cit = mTabChangedList.find( type->Name );
			if ( cit != mTabChangedList.end() )
			{
				if ( cit->second )
				{
					// Label = Value when the first pass in base type
					String finalLabel = ( label == "" ) ? "Value" : label;
					cit->second( window, name + label, tab_or_group, finalLabel, obj, 
								 field ? field->Attributes.ReadOnly == 1 : false, root_type, root_obj );
					return;
				}
			}
		}

		// Get all fields from current type
		std::stack<const Type::Fields*> fieldStack = type->GetAllFields();

		while ( !fieldStack.empty() && ( opt != Entity::O_IGNORE_FIELDS ) )
		{
			for ( const auto & field : ( *fieldStack.top() ) )
			{
				// If field not serialisable, thus not editable
				if ( field.Attributes.Transient != 0 )
					continue;

				// Get a pointer to the data member.
				void * dataPtr = field.IsPointer ? *reinterpret_cast<void **>( field.GetPtr( obj ) ) : field.GetPtr( obj );

				// Try to add to editor if container
				if ( field.Container ) 
				{ 
					const Type * keyType = field.Container->KeyType;
					const Type * valType = field.Container->ValueType;

					bool keyEntity = keyType ? keyType->IsEntity : false;
					bool valEntity = valType->IsEntity;

					void * keyPtr = nullptr,
						 * valPtr = nullptr;

					const Type * keyEntryType = nullptr,
							   * valEntryType = nullptr;

					// Work on the container.
					{
						// Create iterator to iterate through fields
						BH::FieldReadIterator* it = NEW_FIELD_READIT( field.Container, dataPtr );

						//const Type * ftype = ( field.Type->IsEntity && &reinterpret_cast<ConstEntityPtr>( dataPtr )->GetType() )
						//					 ? &reinterpret_cast<ConstEntityPtr>( dataPtr )->GetType()
						//					 : field.Type;

#if CREATE_GROUP_FOR_CONTAINER
						// Create group for container
						String cont_group_name = field.Name.Text;
						if ( group )
							window->AddGroupToGroup( cont_group_name, tab_or_group );
						else
							window->AddGroup( cont_group_name, tab_or_group );
#endif
						// Add button
#if CREATE_GROUP_FOR_CONTAINER
						String button_name = field.Name.Text + String( "Button" );
						String button_label = String( "Add to " ) + field.Name.Text;

						window->AddButtonToGroup( button_name, cont_group_name, button_label,
												  std::function<void()>() )
#else
						String button_name = tab_or_group + " " + field.Name.Text + String("Button");
						String button_label = String( "Add to " ) + field.Name.Text;

						// @todo: Implement for add element
						if ( group )
							window->AddButtonToGroup( button_name, tab_or_group, button_label,
													  std::bind( &EditorReflection::InsertCallback, window, root_type, root_obj, &field, dataPtr, nullptr, nullptr ) );
						else
							window->AddButton( button_name, tab_or_group, button_label,
											   std::bind( &EditorReflection::InsertCallback, window, root_type, root_obj, &field, dataPtr, nullptr, nullptr ) );
#endif

						u32 count = 0;
						for ( ; it->IsValid(); it->IncNext() ) 
						{
							// If container has key
							if ( keyType ) 
							{
								// Get key pointer (Data)
								keyPtr = field.Container->IsKeyPointer 
										 ? const_cast<void *>( *reinterpret_cast<const void * const *>( it->GetKey() ) )
										 : const_cast<void *>( it->GetKey() );

								// Get key type
								keyEntryType = ( keyEntity && &reinterpret_cast<ConstEntityPtr>( keyPtr )->GetType() ) 
											   ? &reinterpret_cast<ConstEntityPtr>( keyPtr )->GetType() 
											   : keyType;
							}

							// Get value pointer (Data)
							valPtr = field.Container->IsValuePointer 
									 ? const_cast<void *>( *reinterpret_cast<const void * const *>( it->GetValue() ) ) 
									 : const_cast<void *>( it->GetValue() );
						
							// Get value type
							valEntryType = ( valEntity && &reinterpret_cast<ConstEntityPtr>( valPtr )->GetType() ) 
										   ? &reinterpret_cast<ConstEntityPtr>( valPtr )->GetType()
										   : valType;

							std::ostringstream ss;
							ss << " " << count;
							if ( keyType ) 
							{
								//void * keynode = DoSerialise(cont_current, keyEntryType, keyPtr, &field.Attributes, &field);
								//DoSerialise(keynode, valEntryType, valPtr, &field.Attributes, &field);

								String group_name = field.Name.Text + ss.str();
#if CREATE_GROUP_FOR_CONTAINER
								window->AddGroupToGroup( group_name, cont_group_name );
#else
								if ( group )
									window->AddGroupToGroup( group_name, tab_or_group );
								else
									window->AddGroup( group_name, tab_or_group );
#endif

								// @todo: Implement callback for remove element
								// Add remove entry button
								String remove_button_name = name + label + " " + group_name;
								window->AddButtonToGroup( remove_button_name, group_name, "Remove " + group_name,
														  std::bind( &EditorReflection::RemoveCallback, window, root_type, root_obj, 
																	 name, label, group_name,
																	 &field, dataPtr, keyPtr, valPtr ) );

								// Group for key
								String key_group_name = label + " " + group_name + " Key";
								window->AddGroupToGroup( key_group_name, group_name );
								DoAddToEditor( window, root_type, root_obj, keyEntryType, keyPtr, &field, name + key_group_name, "", key_group_name, true );

								// Group for value
								String val_group_name = label + " " + group_name + " Value";
								window->AddGroupToGroup( val_group_name, group_name );
								DoAddToEditor( window, root_type, root_obj,  valEntryType, valPtr, &field, name + val_group_name, "", val_group_name, true );
							}
							else
							{
								// If entity, use it's name instead. ( Getting the name of inherited type )
								String group_name = valEntryType->IsEntity ? valEntryType->Name.Text + ss.str() : field.Name.Text + ss.str();
#if CREATE_GROUP_FOR_CONTAINER
								window->AddGroupToGroup( group_name, cont_group_name );
#else
								if ( group )
									window->AddGroupToGroup( group_name, tab_or_group );
								else
									window->AddGroup( group_name, tab_or_group );
#endif
								DoAddToEditor( window, root_type, root_obj,  valEntryType, valPtr, &field, name + label + " " + field.Name.Text + ss.str() , "", group_name, true );

								// Add remove entry button
								String remove_button_name = name + label + " " + group_name;
								window->AddButtonToGroup( remove_button_name, group_name, "Remove " + group_name,
														  std::bind( &EditorReflection::RemoveCallback, window, root_type, root_obj, 
																	 name, label, group_name,
																	 &field, dataPtr, nullptr, valPtr ) );
							}

							++count;
						} 
					
						DEL_FIELD_IT( field.Container, it );

						// Add divider
#if CREATE_GROUP_FOR_CONTAINER
						String divider_name = field.Name.Text + String( "Divider" );

						window->AddDividerToGroup( divider_name, cont_group_name );
#else
						String divider_name = tab_or_group + " " + field.Name.Text + String("Divider");

						if ( group )
							window->AddDividerToGroup( divider_name, tab_or_group );
						else
							window->AddDivider( divider_name, tab_or_group );
#endif
					}
				}
				else 
				{
					const Type* ftype = ( field.Type->IsEntity && &reinterpret_cast<ConstEntityPtr>( dataPtr )->GetType()  )
										? &reinterpret_cast<ConstEntityPtr>( dataPtr )->GetType() 
										: field.Type;

					// Label = oldLabel + " " + fieldName
					// Should really check for empty string, if empty, no space in between
					DoAddToEditor( window, root_type, root_obj,  ftype, dataPtr, &field, name, label + " " + field.Name.Text, tab_or_group, group );
				}
			}

			fieldStack.pop();
		}
	}

	void EditorReflection::DoRemoveFromEditor( EditorWindow * window, const Type * type, void * obj, 
											   const String & name, const String & label, 
											   const String & tab_or_group )
	{
		Entity::OPTIONS opt = Entity::O_NORMAL;

		if ( type->IsEntity )
		{
			opt = reinterpret_cast<EntityPtr>( obj )->OnPreChanged();

			if ( opt == Entity::O_SKIP )
				return;
		}

		// If built in type or serialiser for type is specified, we remove from editor, done!
		RemoverList::iterator it = mRemoverList.find( type->Name );
		if ( it != mRemoverList.end() )
		{
			if ( it->second )
			{
				it->second( window, name + label );
			}
		}

		// Get all fields from current type
		std::stack<const Type::Fields*> fieldStack = type->GetAllFields();

		while ( !fieldStack.empty() && ( opt != Entity::O_IGNORE_FIELDS ) )
		{
			for ( const auto & field : ( *fieldStack.top() ) )
			{
				// If field not serialisable, thus not editable
				if ( field.Attributes.Transient != 0 )
					continue;

				// Get a pointer to the data member.
				void * dataPtr = field.IsPointer ? *reinterpret_cast<void **>( field.GetPtr( obj ) ) : field.GetPtr( obj );

				// Try to add to editor if container
				if ( field.Container ) 
				{ 
					const Type * keyType = field.Container->KeyType;
					const Type * valType = field.Container->ValueType;

					bool keyEntity = keyType ? keyType->IsEntity : false;
					bool valEntity = valType->IsEntity;

					void * keyPtr = nullptr,
						 * valPtr = nullptr;

					const Type * keyEntryType = nullptr,
							   * valEntryType = nullptr;

					// Work on the container.
					{
						// Create iterator to iterate through fields
						BH::FieldReadIterator* it = NEW_FIELD_READIT( field.Container, dataPtr );

						//const Type * ftype = ( field.Type->IsEntity && &reinterpret_cast<ConstEntityPtr>( dataPtr )->GetType() )
						//					 ? &reinterpret_cast<ConstEntityPtr>( dataPtr )->GetType()
						//					 : field.Type;

						// Remove button
						String button_name = tab_or_group + " " + field.Name.Text + String("Button");
						window->RemoveButton( button_name );

						u32 count = 0;
						for ( ; it->IsValid(); it->IncNext() ) 
						{
							// If container has key
							if ( keyType ) 
							{
								// Get key pointer (Data)
								keyPtr = field.Container->IsKeyPointer 
										 ? const_cast<void *>( *reinterpret_cast<const void * const *>( it->GetKey() ) )
										 : const_cast<void *>( it->GetKey() );

								// Get key type
								keyEntryType = ( keyEntity && &reinterpret_cast<ConstEntityPtr>( keyPtr )->GetType() ) 
											   ? &reinterpret_cast<ConstEntityPtr>( keyPtr )->GetType() 
											   : keyType;
							}

							// Get value pointer (Data)
							valPtr = field.Container->IsValuePointer 
									 ? const_cast<void *>( *reinterpret_cast<const void * const *>( it->GetValue() ) ) 
									 : const_cast<void *>( it->GetValue() );
						
							// Get value type
							valEntryType = ( valEntity && &reinterpret_cast<ConstEntityPtr>( valPtr )->GetType() ) 
										   ? &reinterpret_cast<ConstEntityPtr>( valPtr )->GetType()
										   : valType;

							std::ostringstream ss;
							ss << " " << count;
							if ( keyType ) 
							{
								//void * keynode = DoSerialise(cont_current, keyEntryType, keyPtr, &field.Attributes, &field);
								//DoSerialise(keynode, valEntryType, valPtr, &field.Attributes, &field);

								String group_name = field.Name.Text + ss.str();

								// Add remove entry button
								String remove_button_name = name + label + " " + group_name;
								window->RemoveButton( remove_button_name );

								// Group for key
								String key_group_name = label + " " + group_name + " Key";
								DoRemoveFromEditor( window, keyEntryType, keyPtr, name + key_group_name, "", key_group_name );

								// Group for value
								String val_group_name = label + " " + group_name + " Value";
								DoRemoveFromEditor( window, valEntryType, valPtr, name + val_group_name, "", val_group_name );
							}
							else
							{
								// If entity, use it's name instead. ( Getting the name of inherited type )
								String group_name = valEntryType->IsEntity ? valEntryType->Name.Text + ss.str() : field.Name.Text + ss.str();

								DoRemoveFromEditor( window, valEntryType, valPtr, name + label + " " + field.Name.Text + ss.str() , "", group_name );

								// Add remove entry button
								String remove_button_name = name + label + " " + group_name;
								window->RemoveButton( remove_button_name );
							}

							++count;
						} 
					
						DEL_FIELD_IT(field.Container, it);

						// Remove divider
						String divider_name = tab_or_group + " " + field.Name.Text + String("Divider");
						window->RemoveDivider( divider_name );
					}
				}
				else 
				{
					const Type* ftype = ( field.Type->IsEntity && &reinterpret_cast<ConstEntityPtr>( dataPtr )->GetType()  )
										? &reinterpret_cast<ConstEntityPtr>( dataPtr )->GetType() 
										: field.Type;

					// Label = oldLabel + " " + fieldName
					// Should really check for empty string, if empty, no space in between
					DoRemoveFromEditor( window, ftype, dataPtr, name, label + " " + field.Name.Text, tab_or_group );
				}
			}

			fieldStack.pop();
		}

		if ( type->IsEntity )
			reinterpret_cast<EntityPtr>( obj )->OnChanged();
	}
}
#endif