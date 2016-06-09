#if GAME_EDITOR_MODE
namespace BH
{
	template< typename T >
	void EditorReflection::AddEditorReflector( const EditorTabReflector & tab_reflector,
											   const EditorGroupReflector & group_reflector,
											   const EditorRemover & remover )
	{
		mTabReflectorList[TypeCName<T>()] = tab_reflector;
		mGroupReflectorList[TypeCName<T>()] = group_reflector;
		mRemoverList[TypeCName<T>()] = remover;
	}

	template< typename T >
	void EditorReflection::AddEditorInsertNew( const EditorInsertNew & insert_new,
											   const EditorRemoveDelete & remove_delete )
	{
		mInsertElementList[TypeCName<T>()] = insert_new;
		mRemoveElementList[TypeCName<T>()] = remove_delete;
	}

	template< typename T >
	void EditorReflection::AddEditorChanged( const EditorTabChanged & tab_reflector,
										     const EditorGroupChanged & group_reflector,
										     const EditorRemover & remover )
	{
		mTabChangedList[TypeCName<T>()] = tab_reflector;
		mGroupChangedList[TypeCName<T>()] = group_reflector;
		mRemoverList[TypeCName<T>()] = remover;
	}

	template< typename T > 
	EditorReflection::EditorTabReflector EditorReflection::GetTabReflector()
	{
		return mTabReflectorList[TypeCName<T>()];
	}

	template< typename T > 
	EditorReflection::EditorGroupReflector EditorReflection::GetGroupReflector()
	{
		return mGroupReflectorList[TypeCName<T>()];
	}

	template< typename T > 
	EditorReflection::EditorRemover EditorReflection::GetRemover()
	{
		return mRemoverList[TypeCName<T>()];
	}

	template< typename T >
	void EditorReflection::AddTypeEditBoxImpl( EditorWindow * window, const String & name, 
											   const String & tab_name, const String & label, 
											   void * var, bool read_only)
	{
		window->AddEditBox<T>( name, tab_name, label, reinterpret_cast<T *>( var ), read_only );
	}
	
	template< typename T >
	void EditorReflection::AddTypeEditBoxToGroupImpl( EditorWindow * window, const String & name, 
													  const String & group_name, const String & label, 
													  void * var, bool read_only )
	{
		window->AddEditBoxToGroup<T>( name, group_name, label, reinterpret_cast<T *>( var ), read_only );
	}
}
#endif