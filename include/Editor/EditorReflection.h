#ifndef BH_EDITOR_REFLECTION_H
#define BH_EDITOR_REFLECTION_H

#include "Core/CoreEssential.h"
#include "Core/Hash.h"
#include "Core/CName.h"
#include "Core/Type.h"
#include "Core/Typing.h"
#include "Core/TypeStringifier.h"
#include "Core/Singleton.h"
#include "Core/Uncopyable.h"

#include "Editor/EditorWindow.h"

#include "Game/GameConfig.h"

#include <unordered_map>

#if GAME_EDITOR_MODE
namespace BH
{
	class ScriptComponent;

	class EditorReflection : private Uncopyable
	{
	public:
		// Typedefs
		typedef std::function< void ( EditorWindow * window, const String & name, 
									 const String & tab_or_group, const String & label, 
									 void * var, bool read_only ) >						EditorReflector;
		typedef std::function< void ( EditorWindow * window, const String & name ) >	EditorRemover;

		typedef std::function< void ( EditorWindow * window, const String & name, 
									 const String & tab_or_group, const String & label, 
									 void * var, bool read_only, 
									 const Type * type, void * obj ) >					EditorChanged;

		typedef EditorReflector															EditorTabReflector;
		typedef EditorReflector															EditorGroupReflector;
		typedef EditorChanged															EditorTabChanged;
		typedef EditorChanged															EditorGroupChanged;

		typedef std::function< void * () >												EditorInsertNew;
		typedef std::function< void ( void * ) >										EditorRemoveDelete;

		typedef std::unordered_map< CName, EditorTabReflector >							TabReflectorList;
		typedef std::unordered_map< CName, EditorGroupReflector >						GroupReflectorList;
		typedef std::unordered_map< CName, EditorTabChanged >							TabChangedList;
		typedef std::unordered_map< CName, EditorGroupChanged >							GroupChangedList;
		typedef std::unordered_map< CName, EditorRemover >								RemoverList;
		typedef std::unordered_map< String, String >									AllocatedStringList;
		typedef std::unordered_map< CName, EditorInsertNew >							InsertElementList;
		typedef std::unordered_map< CName, EditorRemoveDelete >							RemoveElementList;

		// Declare as singleton
		DECLARE_SINGLETON( EditorReflection );

	public:
		// Destructor
		~EditorReflection();

		// Register Types
		void RegisterTypes();

		// Update
		void Update();

		void SetResetCallback( const std::function< void() > & cb );

		// Add editor reflector through name
		void AddEditorReflector( CName & name, const EditorTabReflector & tab_reflector,
								 const EditorGroupReflector & group_reflector,
								 const EditorRemover & remover );

		// Add editor reflector through type pointer
		void AddEditorReflector( const Type * type, const EditorTabReflector & tab_reflector,
								 const EditorGroupReflector & group_reflector,
								 const EditorRemover & remover );

		// Add editor reflector through type
		template< typename T >
		void AddEditorReflector( const EditorTabReflector & tab_reflector,
								 const EditorGroupReflector & group_reflector,
								 const EditorRemover & remover );

		// Get editor reflector through name
		EditorTabReflector GetTabReflector( const CName & name );
		EditorGroupReflector GetGroupReflector( const CName & name );
		EditorRemover GetRemover( const CName & name );

		// Get editor reflector through type pointer
		EditorTabReflector GetTabReflector( const Type * type );
		EditorGroupReflector GetGroupReflector( const Type * type );
		EditorRemover GetRemover( const Type * type );

		// Get editor reflector through type
		template< typename T > 
		EditorTabReflector GetTabReflector();

		template< typename T > 
		EditorGroupReflector GetGroupReflector();

		template< typename T >
		EditorRemover GetRemover();

		// Add editor insert & remove pointer element in container through name
		void AddEditorInsertNew( CName & name, const EditorInsertNew & insert_new,
								 const EditorRemoveDelete & remove_delete );

		// Add editor insert & remove pointer element in container through type pointer
		void AddEditorInsertNew( const Type * type, const EditorInsertNew & insert_new,
								 const EditorRemoveDelete & remove_delete );

		// Add editor insert & rmove pointer element in container through type
		template< typename T >
		void AddEditorInsertNew( const EditorInsertNew & insert_new,
								 const EditorRemoveDelete & remove_delete );

		// Add editor changed through name
		void AddEditorChanged( CName & name, const EditorTabChanged & tab_reflector,
							   const EditorGroupChanged & group_reflector,
							   const EditorRemover & remover );

		// Add editor changed through type pointer
		void AddEditorChanged( const Type * type, const EditorTabChanged & tab_reflector,
							   const EditorGroupChanged & group_reflector,
							   const EditorRemover & remover );

		// Add editor changed through type
		template< typename T >
		void AddEditorChanged( const EditorTabChanged & tab_reflector,
							   const EditorGroupChanged & group_reflector,
							   const EditorRemover & remover );

		// Add object to editor for editing
		void AddToEditor( EditorWindow * window, const Type * type, void * obj );

		// Remove object from editor
		void RemoveFromEditor( EditorWindow * window, const Type * type, void * obj );

	private:
		// Constructor
		EditorReflection();

		// Recursive adding function
		void DoAddToEditor( EditorWindow * window, const Type * root_type, void * root_obj, const Type * type, void * obj, 
							const Field * field, const String & name, const String & label, 
							const String & tab_or_group, bool group );

		// Recursive remove function
		void DoRemoveFromEditor( EditorWindow * window, const Type * type, void * obj, 
								 const String & name, const String & label, 
								 const String & tab_or_group );

	private:
		// Static implementation functions

		// Add Editor Box
		template< typename T >
		static void AddTypeEditBoxImpl( EditorWindow * window, const String & name, const String & tab_name, 
										const String & label, void * var, bool read_only  );
		
		template< typename T >
		static void AddTypeEditBoxToGroupImpl( EditorWindow * window, const String & name, const String & group_name, 
											   const String & label, void * var, bool read_only );

		static void RemoveEditBox( EditorWindow * window, const String & name );

		// Add Check Box
		static void AddCheckBoxImpl( EditorWindow * window, const String & name, const String & tab_name, 
									 const String & label, void * var, bool read_only );
		static void AddCheckBoxToGroupImpl( EditorWindow * window, const String & name, const String & group_name, 
											const String & label, void * var, bool read_only );
		static void RemoveCheckBox( EditorWindow * window, const String & name );

		// Add Colour Picker
		static void AddColourPicker( EditorWindow * window, const String & name, const String & tab_name, 
									 const String & label, void * var, bool read_only );
		static void AddColourPickerToGroup( EditorWindow * window, const String & name, const String & group_name, 
											const String & label, void * var, bool read_only );
		static void RemoveColourPicker( EditorWindow * window, const String & name );

		// Add defined serialiser type for Editor Box
		static void AddSerialiserDefinedImpl( const Type * type, EditorWindow * window, const String & name, 
											  const String & tab_name, const String & label, void * var, bool read_only );
		static void AddSerialiserDefinedToGroupImpl( const Type * type, EditorWindow * window, const String & name, 
													 const String & group_name, const String & label, void * var, bool read_only );
		static void RemoveSerialiserDefineImpl( EditorWindow * window, const String & name );

		static void RefreshCallback( TypeStringifier::FmString fmstr, const Type * type, void * var, void * allocatedString );

		// Add entry callback
		static void InsertCallback( EditorWindow * window, const Type * type, void * obj,					//!< Parameters for removing and adding window
									const Field * field, void * container, void * key, void * value );		//!< Parameters for adding element

		// Remove entry callback
		static void RemoveCallback( EditorWindow * window, const Type * type, void * obj,					//!< Parameters for removing and adding window
									const String & name, const String & label, const String & group,		//!< Parameters for removing group from window
									const Field * field, void * container, void * key, void * value );		//!< Parameters for removing element

	private:
		// Object Component Insert
		static void * ObjectComponentInsertNew();

		// Object Component Delete
		static void ObjectComponentRemoveDelete( void * data );

		// Script type UI
		static void AddAllScriptTypeOption( EditorComboBox * combobox );
		static void AddScriptTypeImpl( EditorWindow * window, const String & name, const String & tab_name, 
									   const String & label, void * var, bool read_only, 
									   const Type * type, void * obj );
		static void AddScriptTypeToGroupImpl( EditorWindow * window, const String & name, const String & group_name, 
											  const String & label, void * var, bool read_only, 
											  const Type * type, void * obj );
		static void RemoveScriptType( EditorWindow * window, const String & name );
		static void ScriptChangedCallback( EditorWindow * window, const Type * type, void * obj, 
										   ScriptComponent * component );
		static void ScriptResetCallback( EditorWindow * window, const Type * type, void * obj, 
										 ScriptComponent * component );

		// Script model UI
		static void AddAllScriptModelOption( EditorComboBox * combobox );
		static void AddScriptModelImpl( EditorWindow * window, const String & name, const String & tab_name, 
									   const String & label, void * var, bool read_only );
		static void AddScriptModelToGroupImpl( EditorWindow * window, const String & name, const String & group_name, 
											  const String & label, void * var, bool read_only );
		static void RemoveScriptModel( EditorWindow * window, const String & name );

		// Script texture UI
		static void AddAllScriptTextureOption( EditorComboBox * combobox );
		static void AddScriptTextureImpl( EditorWindow * window, const String & name, const String & tab_name,
										  const String & label, void * var, bool read_only );
		static void AddScriptTextureToGroupImpl( EditorWindow * window, const String & name, const String & group_name,
												 const String & label, void * var, bool read_only );
		static void RemoveScriptTexture( EditorWindow * window, const String & name );

		// Script material UI
		static void AddAllScriptMaterialOption( EditorComboBox * combobox );
		static void AddScriptMaterialImpl( EditorWindow * window, const String & name, const String & tab_name,
										  const String & label, void * var, bool read_only );
		static void AddScriptMaterialToGroupImpl( EditorWindow * window, const String & name, const String & group_name,
												 const String & label, void * var, bool read_only );
		static void RemoveScriptMaterial( EditorWindow * window, const String & name );

		// Material
		static void AddPhongImpl( EditorWindow * window, const String & name, const String & tab_name,
										  const String & label, void * var, bool read_only );
		static void AddPhongToGroupImpl( EditorWindow * window, const String & name, const String & group_name,
												 const String & label, void * var, bool read_only );
		static void RemovePhong( EditorWindow * window, const String & name );

	private:
		TabReflectorList mTabReflectorList;
		GroupReflectorList mGroupReflectorList;
		TabChangedList mTabChangedList;
		GroupChangedList mGroupChangedList;
		RemoverList mRemoverList;
		InsertElementList mInsertElementList;
		RemoveElementList mRemoveElementList;

		std::function< void() > mResetCallback;

		static AllocatedStringList AllocatedString;

	};
}

#include "Editor/EditorReflection.inl"

#define EDITOR_REFLECTION\
	::BH::EditorReflection::Instance()

#endif	// GAME_EDITOR_MODE

#endif