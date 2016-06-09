// Precompiled Header
#include "Script/Stdafx.h"

#include <vector>

#include "Core/Singleton.h"
#include "Core/Hash.h"
#include "Core/TypeDB.h"
#include "Core/Field.h"
#include "Script/Script.h"
#include "Script/ScriptManager.h"

// Dependencies
#include <string>
#include <fstream>
#include <map>
#include <msclr/marshal.h>

namespace BH
{
	struct CSTypeInfo
	{
		const Char * mName;
		u32 mSize;
	};

	// Key = C# type name, value = BH type name
	typedef std::unordered_map< String, CSTypeInfo > CSTypes;
	CSTypes CSTypeMap;
	std::list<String> MarshalStrings;

	// Table: https://msdn.microsoft.com/en-us/library/ya5y69ds.aspx
	void Script::InitialiseTypeMap()
	{
		CSTypeMap["Boolean"]	= { TypeName<bool>(),	sizeof( bool ) };
		CSTypeMap["SByte"]		= { TypeName<s8>(),		sizeof( s8 ) };
		CSTypeMap["Byte"]		= { TypeName<u8>(),		sizeof( u8 ) };
		CSTypeMap["Int16"]		= { TypeName<s16>(),	sizeof( s16 ) };
		CSTypeMap["UInt16"]		= { TypeName<u16>(),	sizeof( u16 ) };
		CSTypeMap["Int32"]		= { TypeName<s32>(),	sizeof( s32 ) };
		CSTypeMap["UInt32"]		= { TypeName<u32>(),	sizeof( u32 ) };
		CSTypeMap["Int64"]		= { TypeName<s64>(),	sizeof( s64 ) };
		CSTypeMap["UInt64"]		= { TypeName<u64>(),	sizeof( u64 ) };
		CSTypeMap["Single"]		= { TypeName<f32>(),	sizeof( f32 ) };
		CSTypeMap["Double"]		= { TypeName<f64>(),	sizeof( f64 ) };
		CSTypeMap["String"]		= { TypeName<String>(),	sizeof( String ) };
	}

	void Script::AddScript( const Char * filename )
	{
		// Extract code
		std::ifstream ifile( filename );
		std::string code;

		ifile.seekg( 0, std::ios::end );

		try
		{
			code.reserve( static_cast< size_t >( ifile.tellg() ) );
		}
		catch ( ... )
		{
			std::cout << "Failed in reserving memmory" << std::endl;
		}

		ifile.seekg( 0, std::ios::beg );

		code.assign( std::istreambuf_iterator<Char>( ifile ),
					 std::istreambuf_iterator<Char>() );

		// Add to compile
		detail::ScriptManager::Instance->mCompiler->AddCodeToCompile( msclr::interop::marshal_as< ::System::String ^ >( code.c_str() ) );
	}

	void Script::BuildScripts( TypeDB & typedb )
	{
		detail::ScriptManager::Instance->mCompiler->CompileAllCode();

		// Update script types to reflection
		UpdateScriptTypesToReflection( typedb );
	}

	void Script::UnBuildScript( TypeDB & typedb )
	{
		// Remove script types from reflection
		RemoveScriptTypesFromReflection( typedb );

		std::cout << "Removing sources..." << std::endl;

		// Recompile code
		detail::ScriptManager::Instance->mCompiler->ClearAllSources();

		std::cout << "Unbuild Complete!" << std::endl;
	}

	void Script::ReBuildScript( TypeDB & typedb )
	{
		detail::ScriptManager::Instance->mCompiler->RecompileAllCode();

		// Update script types to reflection
		UpdateScriptTypesToReflection( typedb );
	}

	ScriptObject Script::CreateScriptObject( const Char * type, const Char * name )
	{
		BHScriptCompiler::CSharpType ^ objType = detail::ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( type ) );

		// If scripted type not found
		if ( objType == nullptr )
			return ScriptObject();

#if 0
		::System::Object ^ obj = objType->Create();
		::System::String ^ manName = msclr::interop::marshal_as< ::System::String ^ >( name );
		detail::ScriptManager::Instance->mObjects.Add( manName, obj );
		return ScriptObject( name, type );
#else
		::System::String ^ manType = msclr::interop::marshal_as< ::System::String ^ >( type );
		::System::String ^ manName = msclr::interop::marshal_as< ::System::String ^ >( name );

		::System::Object ^ obj = detail::ScriptManager::Instance->mCompiler->CreateObject( manType, manName );

		if ( obj )
			return ScriptObject( name, type );
		else
			return ScriptObject();
#endif
	}

	void Script::DestroyScriptObject( ScriptObject & object )
	{
		::System::String ^ manName = msclr::interop::marshal_as< ::System::String ^ >( object.GetName() );
#if 0
		detail::ScriptManager::Instance->mObjects.Remove( manName );
#else
		detail::ScriptManager::Instance->mCompiler->DestroyObject( manName );
#endif
		object.InvalidateObject();
	}

	const Char * Script::GetBuiltInTypeName( const Char * type )
	{
		CSTypes::iterator it = CSTypeMap.find( String( type ) );
		if ( it == CSTypeMap.end() )
			return "";

		return it->second.mName;
	}

	bool Script::IsScriptType( const Char * scriptType )
	{
		return detail::ScriptManager::Instance->mCompiler->ScriptTypeExist( msclr::interop::marshal_as< ::System::String ^ >( scriptType ) );
	}

	std::vector<String> Script::GetAllScriptTypes()
	{
		array< ::System::String ^ > ^ csScriptTypes = detail::ScriptManager::Instance->mCompiler->GetAllScriptType();

		std::vector< String > cstypes;
		u32 size = csScriptTypes->Length;

		msclr::interop::marshal_context ^ context = gcnew msclr::interop::marshal_context();

		for ( u32 i = 0; i < size; ++i )
		{
			cstypes.push_back( context->marshal_as<const Char *>( csScriptTypes[i] ) );
		}

		return cstypes;
	}

	std::vector<String> Script::GetAllEngineScriptTypes()
	{
		array< ::System::String ^ > ^ csScriptTypes = detail::ScriptManager::Instance->mCompiler->GetAllEngineScriptType();

		std::vector< String > cstypes;
		u32 size = csScriptTypes->Length;

		msclr::interop::marshal_context ^ context = gcnew msclr::interop::marshal_context();

		for ( u32 i = 0; i < size; ++i )
		{
			cstypes.push_back( context->marshal_as<const Char *>( csScriptTypes[i] ) );
		}

		return cstypes;
	}

	void * Script::NewReflectedDummy( u32 size, const Type * type  )
	{
		Char * obj = new Char[size];
		std::fill( obj, obj + size, 0 );

		std::stack<const Type::Fields *> fields = type->GetAllFields();
		if ( !fields.empty() )
		{
			for ( const auto & field : *fields.top() )
			{
				if ( field.Type->Name == TypeCName<String>() )
				{
					String * str = reinterpret_cast<String *>( field.GetPtr( obj ) );
					*str = String();
				}
				else if ( field.Container != nullptr )
				{
					// Hard code
					if ( field.Container->ValueType->Name == TypeName<bool>() )
					{
						//std::cout << "std::vector<bool> not supported!" << std::endl;
					}
					else if ( field.Container->ValueType->Name == TypeName<s8>() )
					{
						new ( field.GetPtr( obj ) ) std::vector < s8 > ;
					}
					else if ( field.Container->ValueType->Name == TypeName<u8>() )
					{
						new ( field.GetPtr( obj ) ) std::vector < u8 > ;
					}
					else if ( field.Container->ValueType->Name == TypeName<s16>() )
					{
						new ( field.GetPtr( obj ) ) std::vector < s16 > ;
					}
					else if ( field.Container->ValueType->Name == TypeName<u16>() )
					{
						new ( field.GetPtr( obj ) ) std::vector < u16 > ;
					}
					else if ( field.Container->ValueType->Name == TypeName<s32>() )
					{
						new ( field.GetPtr( obj ) ) std::vector < s32 > ;
					}
					else if ( field.Container->ValueType->Name == TypeName<u32>() )
					{
						new ( field.GetPtr( obj ) ) std::vector < u32 > ;
					}
					else if ( field.Container->ValueType->Name == TypeName<s64>() )
					{
						new ( field.GetPtr( obj ) ) std::vector < s64 > ;
					}
					else if ( field.Container->ValueType->Name == TypeName<u64>() )
					{
						new ( field.GetPtr( obj ) ) std::vector < u64 > ;
					}
					else if ( field.Container->ValueType->Name == TypeName<f32>() )
					{
						new ( field.GetPtr( obj ) ) std::vector < f32 > ;
					}
					else if ( field.Container->ValueType->Name == TypeName<f64>() )
					{
						new ( field.GetPtr( obj ) ) std::vector < f64 > ;
					}
					else if ( field.Container->ValueType->Name == TypeName<String>() )
					{
						new ( field.GetPtr( obj ) ) std::vector < String > ;
					}
					else if ( BHScriptCompiler::CSharpType ^ fieldCSharpType = 
							  detail::ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( field.Container->ValueType->Name.Text ) ) )
					{
						new ( field.GetPtr( obj ) ) std::vector < Char * > ;
					}
					else
					{
						//std::cout << "Container type not supported!" << std::endl;
					}
				}

				if ( !field.IsPointer )
					continue;

				if ( field.Type->Size == 0 )
					continue;

				void ** data = reinterpret_cast<void **>( field.GetPtr( obj ) );
				*data = field.Type->New();
			}
		}

		return obj;
	}

	void Script::DeleteReflectedDummy( void * obj, const Type * type )
	{
		std::stack<const Type::Fields *> fields = type->GetAllFields();
		// There should not be a base type, therefore stack size should be 1
		if ( !fields.empty() )
		{
			for ( const auto & field : *fields.top() )
			{
				if ( field.Container != nullptr )
				{
					if ( BHScriptCompiler::CSharpType ^ fieldCSharpType = 
							  detail::ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( field.Container->ValueType->Name.Text ) ) )
					{
						std::vector < Char * > & vector = *reinterpret_cast< std::vector < Char * > * >( field.GetPtr( obj ) );
						for ( auto & i : vector )
						{
							field.Container->ValueType->Delete( i );
						}

						vector.clear();
					}
				}

				if ( !field.IsPointer )
					continue;

				void ** data = reinterpret_cast<void **>( field.GetPtr( obj ) );
				field.Type->Delete( *data );
			}
		}

		delete[] obj;
	}

	void Script::UpdateScriptTypesToReflection( TypeDB & typedb )
	{
		InitialiseTypeMap();

		// Add all C# script type to reflection
		::System::Collections::Generic::List< ::System::String ^ > ^ types = detail::ScriptManager::Instance->mCompiler->GetAllTypes();
		s32 size = types->Count;
		for ( s32 i = 0; i < size; ++i )
		{
			BHScriptCompiler::CSharpType ^ objType = detail::ScriptManager::Instance->mCompiler->GetType( types[i] );
			msclr::interop::marshal_context ^ context = gcnew msclr::interop::marshal_context();
			const Char * typeName = context->marshal_as<const Char *>( objType->GetTypeName() );

			// Ignore C++ Interface class
			if ( String( typeName ) == "CPlusPlusInterface" )
				continue;

			MarshalStrings.push_back( typeName );
			// Create new runtime type (This runtime type is used for marshalling values for serialisation, editor, etc.)
			Type & type = typedb.Create( MarshalStrings.back().c_str() );

			// Add field info
			::System::Collections::Generic::List< ::System::Reflection::FieldInfo ^ > ^ fields = objType->GetFields();
			u32 totalSize = 0;
			for ( s32 j = 0; j < fields->Count; ++j )
			{
				const Char * fieldType = context->marshal_as<const Char *>( fields[j]->FieldType->Name );
				CSTypes::iterator it = CSTypeMap.find( fieldType );
				
				const Char * fieldName = context->marshal_as<const Char *>( fields[j]->Name );
				MarshalStrings.push_back( fieldName );
				fieldName = MarshalStrings.back().c_str();

				// If base type
				if ( it != CSTypeMap.end() )
				{	
					CName fieldCType( it->second.mName );

					// Add field on run time
					type.Field( fieldName, typedb.Get( fieldCType ), totalSize, false );

					totalSize += CSTypeMap[fieldType].mSize;
				}
				// If C# container List
				else if ( fields[j]->FieldType->IsGenericType && String( fieldType ).find( "List" ) == 0 )
				{
					array< ::System::Type ^ > ^ listType = fields[j]->FieldType->GetGenericArguments();
					
					const Char * listValueType = context->marshal_as<const Char *>( listType[0]->Name );

					CSTypes::iterator lit = CSTypeMap.find( listValueType );

					// Hard code!
					if ( lit == CSTypeMap.end() )
					{
						if ( BHScriptCompiler::CSharpType ^ fieldCSharpType = 
							  detail::ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( listValueType ) ) )
						{
							CName listValueCType( listValueType );
							const Type * listValueTypeInfo = typedb.Get( listValueCType );
							// Create type if C# type not found
							if ( !listValueTypeInfo )
							{
								MarshalStrings.push_back( listValueType );
								listValueCType = CName( MarshalStrings.back().c_str() );
								typedb.Create( listValueCType );
							}

							//const Char * containerFieldName = context->marshal_as<const Char *>( fields[j]->Name );
							const Char * containerFieldName = fieldName;
							type.Field< std::vector<Char *> >( containerFieldName, typedb.Get< std::vector<Char *> >(), totalSize, false );
							Field * field = const_cast<Field *>( type.GetField( containerFieldName ) );
							field->Container->ValueType = typedb.Get( listValueCType );
							totalSize += sizeof( std::vector<Char *> );
						}
						else
						{
							//std::cout << "Container type not supported!" << std::endl;
						}
					}
					else if ( lit->second.mName == TypeName<bool>() )
					{
						//type.Field< std::vector<bool> >( context->marshal_as<const Char *>(fields[j]->Name), typedb.Get< std::vector<bool> >(), totalSize, false );
						//totalSize += sizeof( std::vector<bool> );
						//std::cout << "std::vector<bool> not supported!" << std::endl;
					}
					else if ( lit->second.mName == TypeName<s8>() )
					{
						type.Field< std::vector<s8> >( fieldName, typedb.Get< std::vector<s8> >(), totalSize, false );
						totalSize += sizeof( std::vector<s8> );
					}
					else if ( lit->second.mName == TypeName<u8>() )
					{
						type.Field< std::vector<u8> >( fieldName, typedb.Get< std::vector<u8> >(), totalSize, false );
						totalSize += sizeof( std::vector<u8> );
					}
					else if ( lit->second.mName == TypeName<s16>() )
					{
						type.Field< std::vector<s16> >( fieldName, typedb.Get< std::vector<s16> >(), totalSize, false );
						totalSize += sizeof( std::vector<s16> );
					}
					else if ( lit->second.mName == TypeName<u16>() )
					{
						type.Field< std::vector<u16> >( fieldName, typedb.Get< std::vector<u16> >(), totalSize, false );
						totalSize += sizeof( std::vector<u16> );
					}
					else if ( lit->second.mName == TypeName<s32>() )
					{
						type.Field< std::vector<s32> >( fieldName, typedb.Get< std::vector<s32> >(), totalSize, false );
						totalSize += sizeof( std::vector<s32> );
					}
					else if ( lit->second.mName == TypeName<u32>() )
					{
						type.Field< std::vector<u32> >( fieldName, typedb.Get< std::vector<u32> >(), totalSize, false );
						totalSize += sizeof( std::vector<u32> );
					}
					else if ( lit->second.mName == TypeName<s64>() )
					{
						type.Field< std::vector<s64> >( fieldName, typedb.Get< std::vector<s64> >(), totalSize, false );
						totalSize += sizeof( std::vector<s64> );
					}
					else if ( lit->second.mName == TypeName<u64>() )
					{
						type.Field< std::vector<u64> >( fieldName, typedb.Get< std::vector<u64> >(), totalSize, false );
						totalSize += sizeof( std::vector<u64> );
					}
					else if ( lit->second.mName == TypeName<f32>() )
					{
						type.Field< std::vector<f32> >( fieldName, typedb.Get< std::vector<f32> >(), totalSize, false );
						totalSize += sizeof( std::vector<f32> );
					}
					else if ( lit->second.mName == TypeName<f64>() )
					{
						type.Field< std::vector<f64> >( fieldName, typedb.Get< std::vector<f64> >(), totalSize, false );
						totalSize += sizeof( std::vector<f64> );
					}
					else if ( lit->second.mName == TypeName<String>() )
					{
						type.Field< std::vector<String> >( fieldName, typedb.Get< std::vector<String> >(), totalSize, false );
						totalSize += sizeof( std::vector<String> );
					}
					else
					{
						//std::cout << "Container type not supported!" << std::endl;
					}
				}
				// Else type in C#
				else if( BHScriptCompiler::CSharpType ^ fieldCSharpType = detail::ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( fieldType ) ) )
				{
					CName fieldCType( fieldType );
					const Type * fieldType = typedb.Get( fieldCType );
					// Create type if C# type not found
					if ( !fieldType )
					{
						MarshalStrings.push_back( fieldCType.Text );
						fieldCType = CName( MarshalStrings.back().c_str() );
						typedb.Create( fieldCType );
					}

					// Add field on run time, pointer type
					type.Field( fieldName, typedb.Get( fieldCType ), totalSize, true );

					// Field is pointer type
					totalSize += sizeof( void * );
				}
			}

			totalSize = totalSize == 0 ? 1 : totalSize;
			type.Newer = std::bind( &Script::NewReflectedDummy, totalSize, &type );
			type.Deleter = std::bind( &Script::DeleteReflectedDummy, std::placeholders::_1, &type );
			type.Size = totalSize;
		}
	}

	void Script::RemoveScriptTypesFromReflection( TypeDB & typedb )
	{
		// Add all C# script type to reflection
		::System::Collections::Generic::List< ::System::String ^ > ^ types = detail::ScriptManager::Instance->mCompiler->GetAllTypes();
		s32 size = types->Count;
		for ( s32 i = 0; i < size; ++i )
		{
			BHScriptCompiler::CSharpType ^ objType = detail::ScriptManager::Instance->mCompiler->GetType( types[i] );
			msclr::interop::marshal_context ^ context = gcnew msclr::interop::marshal_context();
			const Char * typeName = context->marshal_as<const Char *>( objType->GetTypeName() );

			// Ignore C++ Interface class
			if ( String( typeName ) == "CPlusPlusInterface" )
				continue;

			// Remove runtime type (This runtime type is used for marshalling values for serialisation, editor, etc.)
			typedb.Remove( typeName );
		}

		MarshalStrings.clear();
	}
}