// Precompiled Header
#include "Script/Stdafx.h"

#include "Script/ScriptMethod.h"
#include "Script/ScriptInterOp.h"
#include "Script/ScriptObject.h"
#include "Script/ScriptManager.h"

#include "Core/Hash.h"

#include <msclr/marshal.h>

namespace BH
{
	namespace detail
	{
		// Singleton instance that create stack
		public ref class ScriptParamsStack
		{
		public:
			typedef ::System::Collections::Generic::List< ::System::Object ^ > ParamsStack;

		private:
			// Singleton
			ScriptParamsStack()
			: mStack( gcnew ParamsStack() )
			{}

		public:
			static ScriptParamsStack ^ Instance = gcnew ScriptParamsStack();
			ParamsStack ^ mStack;

		};

		void ScriptMethod::PushParamsToStack( const BH::Char * str )
		{
			ScriptParamsStack::Instance->mStack->Add( msclr::interop::marshal_as< ::System::String ^ >( str ) );
		}
		
		void ScriptMethod::PushParamsToStack( bool val )
		{
			ScriptParamsStack::Instance->mStack->Add( val );
		}

		void ScriptMethod::PushParamsToStack( BH::s8 val )
		{
			ScriptParamsStack::Instance->mStack->Add( val );
		}

		void ScriptMethod::PushParamsToStack( BH::u8 val )
		{
			ScriptParamsStack::Instance->mStack->Add( val );
		}

		void ScriptMethod::PushParamsToStack( BH::s16 val )
		{
			ScriptParamsStack::Instance->mStack->Add( val );
		}

		void ScriptMethod::PushParamsToStack( BH::u16 val )
		{
			ScriptParamsStack::Instance->mStack->Add( val );
		}

		void ScriptMethod::PushParamsToStack( BH::s32 val )
		{
			ScriptParamsStack::Instance->mStack->Add( val );
		}

		void ScriptMethod::PushParamsToStack( BH::u32 val )
		{
			ScriptParamsStack::Instance->mStack->Add( val );
		}

		void ScriptMethod::PushParamsToStack( BH::s64 val )
		{
			ScriptParamsStack::Instance->mStack->Add( val );
		}

		void ScriptMethod::PushParamsToStack( BH::u64 val )
		{
			ScriptParamsStack::Instance->mStack->Add( val );
		}

		void ScriptMethod::PushParamsToStack( BH::f32 val )
		{
			ScriptParamsStack::Instance->mStack->Add( val );
		}

		void ScriptMethod::PushParamsToStack( BH::f64 val )
		{
			ScriptParamsStack::Instance->mStack->Add( val );
		}

		void ScriptMethod::PushParamsToStack( BH::ScriptObject * obj )
		{
			ScriptParamsStack::Instance->mStack->Add( ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) ) );
		}
		
		void ScriptMethod::CallMethod( BH::ScriptObject * obj, const BH::Char * methodName )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Call Method
			objType->CallMethod( o, msclr::interop::marshal_as< ::System::String ^ >( methodName ), ScriptParamsStack::Instance->mStack->ToArray() );

			ScriptParamsStack::Instance->mStack->Clear();
		}

		// ***************************************************** Set Built int Values ***************************************************
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const BH::Char * str )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), msclr::interop::marshal_as< ::System::String ^ >( str ) );
		}
		
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, bool val )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), val );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::s8 val )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), val );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::u8 val )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), val );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::s16 val )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), val );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::u16 val )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), val );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::s32 val )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), val );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::u32 val )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), val );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::s64 val )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), val );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::u64 val )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), val );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::f32 val )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), val );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::f64 val )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), val );
		}
		
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::ScriptObject * val )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			::System::Object ^ oval = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( val->GetName() ) );
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), oval );
		}

		template < typename T >
		void ScriptMethodObjectSetValueImpl( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, T val )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );
			
			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );
			
			::System::Object ^ oval = nullptr;
			::System::String ^ fieldName = nullptr;
			u32 fieldNum = fieldNames.size();
			msclr::interop::marshal_context ^ context = gcnew msclr::interop::marshal_context();
			for ( u32 i = 0; i < fieldNum; ++i )
			{
				fieldName = msclr::interop::marshal_as< ::System::String ^ >( fieldNames[i] );
				::System::String ^ typeName = objType->GetFieldType( fieldName )->FieldType->Name;
			
				oval = objType->GetValueFromField( o, fieldName );
			
				const BH::Char * fieldType = BH::Script::GetBuiltInTypeName( context->marshal_as<const BH::Char *>( typeName ) );
				// Type wanted
				if ( fieldType == String( BH::TypeName<T>() ) )
				{
					// Set Value
					objType->SetValueToField( o, fieldName, val );
					return;
				}
				// Other built in type
				else if ( fieldType != String( "" ) )
				{
					return;
				}
				// Possible of C# script type
				else
				{
					o = oval;
					objType = ScriptManager::Instance->mCompiler->GetType( typeName );
				}
			}
			
			// Set Value
			objType->SetValueToField( o, fieldName, val );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const BH::Char * str )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );
			
			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );
			
			::System::Object ^ oval = nullptr;
			::System::String ^ fieldName = nullptr;
			u32 fieldNum = fieldNames.size();
			msclr::interop::marshal_context ^ context = gcnew msclr::interop::marshal_context();
			for ( u32 i = 0; i < fieldNum; ++i )
			{
				fieldName = msclr::interop::marshal_as< ::System::String ^ >( fieldNames[i] );
				::System::String ^ typeName = objType->GetFieldType( fieldName )->FieldType->Name;
			
				oval = objType->GetValueFromField( o, fieldName );
			
				const BH::Char * fieldType = BH::Script::GetBuiltInTypeName( context->marshal_as<const BH::Char *>( typeName ) );
				// Type wanted
				if ( fieldType == String( BH::TypeName<String>() ) )
				{
					// Set Value
					objType->SetValueToField( o, fieldName, msclr::interop::marshal_as< ::System::String ^ >( str ) );
					return;
				}
				// Other built in type
				else if ( fieldType != String( "" ) )
				{
					return;
				}
				// Possible of C# script type
				else
				{
					o = oval;
					objType = ScriptManager::Instance->mCompiler->GetType( typeName );
				}
			}
			
			// Set Value
			objType->SetValueToField( o, fieldName, msclr::interop::marshal_as< ::System::String ^ >( str ) );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, bool val )
		{
			ScriptMethodObjectSetValueImpl( obj, fieldNames, val );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::s8 val )
		{
			ScriptMethodObjectSetValueImpl( obj, fieldNames, val );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::u8 val )
		{
			ScriptMethodObjectSetValueImpl( obj, fieldNames, val );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::s16 val )
		{
			ScriptMethodObjectSetValueImpl( obj, fieldNames, val );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::u16 val )
		{
			ScriptMethodObjectSetValueImpl( obj, fieldNames, val );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::s32 val )
		{
			ScriptMethodObjectSetValueImpl( obj, fieldNames, val );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::u32 val )
		{
			ScriptMethodObjectSetValueImpl( obj, fieldNames, val );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::s64 val )
		{
			ScriptMethodObjectSetValueImpl( obj, fieldNames, val );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::u64 val )
		{
			ScriptMethodObjectSetValueImpl( obj, fieldNames, val );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::f32 val )
		{
			ScriptMethodObjectSetValueImpl( obj, fieldNames, val );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::f64 val )
		{
			ScriptMethodObjectSetValueImpl( obj, fieldNames, val );
		}

		// ***************************************************** Get Built int Values ***************************************************
		template< typename T >
		T ScriptMethodGetValueImpl( BH::ScriptObject * obj, const BH::Char * fieldName )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Get value
			if ( o )
			{
				::System::Object ^ val = objType->GetValueFromField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ) );
				return safe_cast<T>(val);
			}
			else
			{
				return T();
			}
		}

		const BH::Char * ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const BH::Char * )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Get value
			::System::Object ^ val = objType->GetValueFromField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ) );
			::System::String ^ str = safe_cast< ::System::String ^ >( val );
			msclr::interop::marshal_context ^ context = gcnew msclr::interop::marshal_context();
			return context->marshal_as<const BH::Char *>( str );
		}
		
		bool ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, bool )
		{
			return ScriptMethodGetValueImpl<bool>( obj, fieldName );
		}

		BH::s8 ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::s8 )
		{
			return ScriptMethodGetValueImpl<BH::s8>( obj, fieldName );
		}
		
		BH::u8 ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::u8 )
		{
			return ScriptMethodGetValueImpl<BH::u8>( obj, fieldName );
		}
		
		BH::s16 ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::s16 )
		{
			return ScriptMethodGetValueImpl<BH::s16>( obj, fieldName );
		}
		
		BH::u16 ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::u16 )
		{
			return ScriptMethodGetValueImpl<BH::u16>( obj, fieldName );
		}
		
		BH::s32 ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::s32 )
		{
			return ScriptMethodGetValueImpl<BH::s32>( obj, fieldName );
		}
		
		BH::u32 ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::u32 )
		{
			return ScriptMethodGetValueImpl<BH::u32>( obj, fieldName );
		}
		
		BH::s64 ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::s64 )
		{
			return ScriptMethodGetValueImpl<BH::s64>( obj, fieldName );
		}
		
		BH::u64 ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::u64 )
		{
			return ScriptMethodGetValueImpl<BH::u64>( obj, fieldName );
		}
		
		BH::f32 ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::f32 )
		{
			return ScriptMethodGetValueImpl<BH::f32>( obj, fieldName );
		}
		
		BH::f64 ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::f64 )
		{
			return ScriptMethodGetValueImpl<BH::f64>( obj, fieldName );
		}

		template< typename T >
		T ScriptMethodObjectGetValueImpl( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Object is null, impossible to get values
			if ( !o )
				return T();

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			u32 fieldNum = fieldNames.size();
			::System::Object ^ val = nullptr;
			msclr::interop::marshal_context ^ context = gcnew msclr::interop::marshal_context();
			for ( u32 i = 0; i < fieldNum; ++i )
			{
				::System::String ^ fieldName = msclr::interop::marshal_as< ::System::String ^ >( fieldNames[i] );

				// Get value
				val = objType->GetValueFromField( o, fieldName );

				::System::String ^ typeName = objType->GetFieldType( fieldName )->FieldType->Name;

				const BH::Char * fieldType = BH::Script::GetBuiltInTypeName( context->marshal_as<const BH::Char *>( typeName ) );

				// Type wanted
				if ( fieldType == String( BH::TypeName<T>() ) )
				{
					return safe_cast<T>( val );
				}
				// Other built in type
				else if ( fieldType != String( "" ) )
				{
					return T();
				}
				// Possible of C# script type
				else
				{
					o = val;
					objType = ScriptManager::Instance->mCompiler->GetType( typeName );

					if ( !objType )
					{
						return T();
					}

					// Object is null, impossible to get values
					if ( !o )
					{
						return T();
					}
				}
			}

			return safe_cast<T>( val );
		}

		const BH::Char * ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const BH::Char * )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			u32 fieldNum = fieldNames.size();
			::System::Object ^ val = nullptr;
			msclr::interop::marshal_context ^ context = gcnew msclr::interop::marshal_context();
			for ( u32 i = 0; i < fieldNum; ++i )
			{
				::System::String ^ fieldName = msclr::interop::marshal_as< ::System::String ^ >( fieldNames[i] );

				// Get value
				val = objType->GetValueFromField( o, fieldName );
				::System::String ^ typeName = objType->GetFieldType( fieldName )->FieldType->Name;

				const BH::Char * fieldType = BH::Script::GetBuiltInTypeName( context->marshal_as<const BH::Char *>( typeName ) );
				// Type wanted
				if ( fieldType == String( BH::TypeName<String>() ) )
				{
					::System::String ^ str = safe_cast<::System::String ^>( val );
					const BH::Char * cstr = context->marshal_as<const BH::Char *>( str );
					return cstr;
				}
				// Other built in type
				else if ( fieldType != String( "" ) )
				{
					return nullptr;
				}
				// Possible of C# script type
				else
				{
					o = val;
					objType = ScriptManager::Instance->mCompiler->GetType( typeName );
				}
			}

			::System::String ^ str = safe_cast<::System::String ^>( val );
			return context->marshal_as<const BH::Char *>( str );
		}
		
		bool ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, bool )
		{
			return ScriptMethodObjectGetValueImpl<bool>( obj, fieldNames );
		}

		BH::s8 ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::s8 )
		{
			return ScriptMethodObjectGetValueImpl<BH::s8>( obj, fieldNames );
		}

		BH::u8 ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::u8 )
		{
			return ScriptMethodObjectGetValueImpl<BH::u8>( obj, fieldNames );
		}

		BH::s16 ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::s16 )
		{
			return ScriptMethodObjectGetValueImpl<BH::s16>( obj, fieldNames );
		}

		BH::u16 ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::u16 )
		{
			return ScriptMethodObjectGetValueImpl<BH::u16>( obj, fieldNames );
		}

		BH::s32 ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::s32 )
		{
			return ScriptMethodObjectGetValueImpl<BH::s32>( obj, fieldNames );
		}

		BH::u32 ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::u32 )
		{
			return ScriptMethodObjectGetValueImpl<BH::u32>( obj, fieldNames );
		}

		BH::s64 ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::s64 )
		{
			return ScriptMethodObjectGetValueImpl<BH::s64>( obj, fieldNames );
		}

		BH::u64 ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::u64 )
		{
			return ScriptMethodObjectGetValueImpl<BH::u64>( obj, fieldNames );
		}

		BH::f32 ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::f32 )
		{
			return ScriptMethodObjectGetValueImpl<BH::f32>( obj, fieldNames );
		}

		BH::f64 ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::f64 )
		{
			return ScriptMethodObjectGetValueImpl<BH::f64>( obj, fieldNames );
		}

		// ******************************************************* Container ***************************************************
		template< typename T >
		::System::Collections::Generic::List<T> ^ MarshalVector( const std::vector<T> & values )
		{
			size_t size = values.size();
			::System::Collections::Generic::List<T> ^ list = gcnew ::System::Collections::Generic::List<T>();
		
			// Because marshal copy function is not template..., lots of if check is needed...
			for ( size_t i = 0; i < size; ++i )
			{
				list->Add( values[i] );
			}
		
			return list;
		}
		
		::System::Collections::Generic::List< ::System::String ^ > ^ MarshalStrVector( const std::vector<String> & values )
		{
			size_t size = values.size();
			::System::Collections::Generic::List< ::System::String ^ > ^ list = gcnew ::System::Collections::Generic::List< ::System::String ^ >();
		
			// Because marshal copy function is not template..., lots of if check is needed...
			for ( size_t i = 0; i < size; ++i )
				list->Add( msclr::interop::marshal_as< ::System::String ^ >( values[i].c_str() ) );
		
			return list;
		}

		template< typename T >
		std::vector<T>  MarshalList( ::System::Collections::Generic::List<T> ^ list )
		{
			size_t size = list->Count;
		
			std::vector<T> values;

			// Because marshal copy function is not template..., lots of if check is needed...
			for ( size_t i = 0; i < size; ++i )
				values.push_back( list[i] );
		
			return values;
		}
		
		std::vector<String> MarshalStrList( ::System::Collections::Generic::List< ::System::String ^ > ^ list )
		{
			size_t size = list->Count;
		
			std::vector<String> values;

			msclr::interop::marshal_context ^ context = gcnew msclr::interop::marshal_context();

			// Because marshal copy function is not template..., lots of if check is needed...
			for ( size_t i = 0; i < size; ++i )
				values.push_back( context->marshal_as<const BH::Char *>( list[i] ) );
		
			return values;
		}

		void GetValueFromBaseType( void * data, const BH::Type * datatype, ::System::Object ^ obj )
		{
			if ( datatype->Name == BH::TypeCName<BH::String>() )
			{
				BH::String * val = reinterpret_cast<BH::String *>( data );
				::System::String ^ str = safe_cast< ::System::String ^ >( obj );
				msclr::interop::marshal_context ^ context = gcnew msclr::interop::marshal_context();
				*val = context->marshal_as<const BH::Char *>( str );
			}
			else if ( datatype->Name == BH::TypeCName<bool>() )
			{
				bool * val = reinterpret_cast<bool *>( data );
				*val = safe_cast<bool>( obj );
			}
			else if ( datatype->Name == BH::TypeCName<BH::s8>() )
			{
				BH::s8 * val = reinterpret_cast<BH::s8 *>( data );
				*val = safe_cast<BH::s8>( obj );
			}
			else if ( datatype->Name == BH::TypeCName<BH::u8>() )
			{
				BH::u8 * val = reinterpret_cast<BH::u8 *>( data );
				*val = safe_cast<BH::u8>( obj );
			}
			else if ( datatype->Name == BH::TypeCName<BH::s16>() )
			{
				BH::s16 * val = reinterpret_cast<BH::s16 *>( data );
				*val = safe_cast<BH::s16>( obj );
			}
			else if ( datatype->Name == BH::TypeCName<BH::u16>() )
			{
				BH::u16 * val = reinterpret_cast<BH::u16 *>( data );
				*val = safe_cast<BH::u16>( obj );
			}
			else if ( datatype->Name == BH::TypeCName<BH::s32>() )
			{
				BH::s32 * val = reinterpret_cast<BH::s32 *>( data );
				*val = safe_cast<BH::s32>( obj );
			}
			else if ( datatype->Name == BH::TypeCName<BH::u32>() )
			{
				BH::u32 * val = reinterpret_cast<BH::u32 *>( data );
				*val = safe_cast<BH::u32>( obj );
			}
			else if ( datatype->Name == BH::TypeCName<BH::s64>() )
			{
				BH::s64 * val = reinterpret_cast<BH::s64 *>( data );
				*val = safe_cast<BH::s64>( obj );
			}
			else if ( datatype->Name == BH::TypeCName<BH::u64>() )
			{
				BH::u64 * val = reinterpret_cast<BH::u64 *>( data );
				*val = safe_cast<BH::u64>( obj );
			}
			else if ( datatype->Name == BH::TypeCName<BH::f32>() )
			{
				BH::f32 * val = reinterpret_cast<BH::f32 *>( data );
				*val = safe_cast<BH::f32>( obj );
			}
			else if ( datatype->Name == BH::TypeCName<BH::f64>() )
			{
				BH::f64 * val = reinterpret_cast<BH::f64 *>( data );
				*val = safe_cast<BH::f64>( obj );
			}
			//else
			//{
			//	std::cout << "Not base type!" << std::endl;
			//}
		}

		void GetValueFromCSharp( void * data, const BH::Type * datatype, ::System::Object ^ obj, ::System::Type ^ objtype );

		void GetContainerValuesFromScriptType( void * data, const BH::Field * containerField, ::System::Object ^ obj, ::System::Type ^ objtype )
		{
			std::vector<BH::Char *> * vec = reinterpret_cast< std::vector<BH::Char *> * >( data );

			//::System::Collections::Generic::List< ::System::Object ^ > ^ list = safe_cast< ::System::Collections::Generic::List< ::System::Object ^ > ^ >( obj );
			//BH::s32 listSize = list->Count;
			array< ::System::Type ^ > ^ listType = objtype->GetGenericArguments();

			::System::Reflection::PropertyInfo ^ sizeProperty = objtype->GetProperty( "Count" );
			BH::s32 listSize = safe_cast<BH::s32>( sizeProperty->GetValue( obj, nullptr ) );
			sizeProperty = objtype->GetProperty( "Item" );

			vec->clear();
			vec->reserve( listSize );

			for ( BH::s32 i = 0; i < listSize; ++i )
			{
				vec->push_back( reinterpret_cast<BH::Char *>( containerField->Container->ValueType->New() ) );
				::System::Object ^ elem = sizeProperty->GetValue( obj, gcnew array< ::System::Object ^ >{ i } );
				GetValueFromCSharp( &vec->back(), containerField->Container->ValueType, elem, listType[0] );
			}
		}

		void GetValueFromCSharp( void * data, const BH::Type * datatype, ::System::Object ^ obj, ::System::Type ^ objtype )
		{
			std::stack<const Type::Fields *> fields = datatype->GetAllFields();
			void ** val = reinterpret_cast< void ** >( data );

			// There should not be a base type, therefore stack size should be 1
			for ( const auto & tfield : *fields.top() )
			{
				void * fieldVal = tfield.GetPtr( *val );
				// If base type, done
				if ( tfield.Type->Name == TypeCName<BH::String>()	||
					 tfield.Type->Name == TypeCName<bool>()			||
					 tfield.Type->Name == TypeCName<BH::s8>()		||
					 tfield.Type->Name == TypeCName<BH::u8>()		||
					 tfield.Type->Name == TypeCName<BH::s16>()		||
					 tfield.Type->Name == TypeCName<BH::u16>()		||
					 tfield.Type->Name == TypeCName<BH::s32>()		||
					 tfield.Type->Name == TypeCName<BH::u32>()		||
					 tfield.Type->Name == TypeCName<BH::s64>()		||
					 tfield.Type->Name == TypeCName<BH::u64>()		||
					 tfield.Type->Name == TypeCName<BH::f32>()		||
					 tfield.Type->Name == TypeCName<BH::f64>() )
				{
					::System::Reflection::FieldInfo ^ objField = objtype->GetField( msclr::interop::marshal_as< ::System::String ^ >( tfield.Name.Text ) );
					GetValueFromBaseType( fieldVal, tfield.Type, objField->GetValue( obj ) );
				}
				// Else, iterate through every element and recursively call
				else if ( tfield.Container )
				{
					::System::Reflection::FieldInfo ^ objField = objtype->GetField( msclr::interop::marshal_as< ::System::String ^ >( tfield.Name.Text ) );
					if ( tfield.Container->ValueType->Name == TypeCName<BH::String>() )
					{
						*reinterpret_cast< std::vector<BH::String> * >( fieldVal ) = 
							MarshalStrList( safe_cast< ::System::Collections::Generic::List< ::System::String ^ > ^ >( objField->GetValue( obj ) ) );
					}
					else if ( tfield.Container->ValueType->Name == TypeCName<BH::s8>() )
					{
						*reinterpret_cast< std::vector<BH::s8> * >( fieldVal ) = 
							MarshalList( safe_cast< ::System::Collections::Generic::List< BH::s8 > ^ >( objField->GetValue( obj ) ) );
					}
					else if ( tfield.Container->ValueType->Name == TypeCName<BH::u8>() )
					{
						*reinterpret_cast< std::vector<BH::u8> * >( fieldVal ) = 
							MarshalList( safe_cast< ::System::Collections::Generic::List< BH::u8 > ^ >( objField->GetValue( obj ) ) );
					}
					else if ( tfield.Container->ValueType->Name == TypeCName<BH::s16>() )
					{
						*reinterpret_cast< std::vector<BH::s16> * >( fieldVal ) = 
							MarshalList( safe_cast< ::System::Collections::Generic::List< BH::s16 > ^ >( objField->GetValue( obj ) ) );
					}
					else if ( tfield.Container->ValueType->Name == TypeCName<BH::u16>() )
					{
						*reinterpret_cast< std::vector<BH::u16> * >( fieldVal ) = 
							MarshalList( safe_cast< ::System::Collections::Generic::List< BH::u16 > ^ >( objField->GetValue( obj ) ) );
					}
					else if ( tfield.Container->ValueType->Name == TypeCName<BH::s32>() )
					{
						*reinterpret_cast< std::vector<BH::s32> * >( fieldVal ) = 
							MarshalList( safe_cast< ::System::Collections::Generic::List< BH::s32 > ^ >( objField->GetValue( obj ) ) );
					}
					else if ( tfield.Container->ValueType->Name == TypeCName<BH::u32>() )
					{
						*reinterpret_cast< std::vector<BH::u32> * >( fieldVal ) = 
							MarshalList( safe_cast< ::System::Collections::Generic::List< BH::u32 > ^ >( objField->GetValue( obj ) ) );
					}
					else if ( tfield.Container->ValueType->Name == TypeCName<BH::s64>() )
					{
						*reinterpret_cast< std::vector<BH::s64> * >( fieldVal ) = 
							MarshalList( safe_cast< ::System::Collections::Generic::List< BH::s64 > ^ >( objField->GetValue( obj ) ) );
					}
					else if ( tfield.Container->ValueType->Name == TypeCName<BH::u64>() )
					{
						*reinterpret_cast< std::vector<BH::u64> * >( fieldVal ) = 
							MarshalList( safe_cast< ::System::Collections::Generic::List< BH::u64 > ^ >( objField->GetValue( obj ) ) );
					}
					else if ( tfield.Container->ValueType->Name == TypeCName<BH::f32>() )
					{
						*reinterpret_cast< std::vector<BH::f32> * >( fieldVal ) = 
							MarshalList( safe_cast< ::System::Collections::Generic::List< BH::f32 > ^ >( objField->GetValue( obj ) ) );
					}
					else if ( tfield.Container->ValueType->Name == TypeCName<BH::f64>() )
					{
						*reinterpret_cast< std::vector<BH::f64> * >( fieldVal ) = 
							MarshalList( safe_cast< ::System::Collections::Generic::List< BH::f64 > ^ >( objField->GetValue( obj ) ) );
					}
					// C# type
					else
					{
						GetContainerValuesFromScriptType( fieldVal, &tfield, objField->GetValue( obj ), objField->FieldType );
					}
				}
				// C# type, recursive call
				else
				{
					::System::Reflection::FieldInfo ^ objField = objtype->GetField( msclr::interop::marshal_as< ::System::String ^ >( tfield.Name.Text ) );
					GetValueFromCSharp( fieldVal, tfield.Type, objField->GetValue( obj ), objField->FieldType );
				}
			}
		}

		void SetValueToBaseType( void * data, const BH::Type * datatype, ::System::Object ^ obj, ::System::Reflection::FieldInfo ^ objField )
		{
			if ( datatype->Name == BH::TypeCName<BH::String>() )
			{
				BH::String * val = reinterpret_cast<BH::String *>( data );
				//::System::String ^ str = safe_cast< ::System::String ^ >( obj );
				//str = msclr::interop::marshal_as< ::System::String ^ >( val->c_str() );
				objField->SetValue( obj, msclr::interop::marshal_as< ::System::String ^ >( val->c_str() ) );
			}
			else if ( datatype->Name == BH::TypeCName<bool>() )
			{
				bool * val = reinterpret_cast<bool *>( data );
				objField->SetValue( obj, *val );
			}
			else if ( datatype->Name == BH::TypeCName<BH::s8>() )
			{
				BH::s8 * val = reinterpret_cast<BH::s8 *>( data );
				objField->SetValue( obj, *val );
			}
			else if ( datatype->Name == BH::TypeCName<BH::u8>() )
			{
				BH::u8 * val = reinterpret_cast<BH::u8 *>( data );
				objField->SetValue( obj, *val );
			}
			else if ( datatype->Name == BH::TypeCName<BH::s16>() )
			{
				BH::s16 * val = reinterpret_cast<BH::s16 *>( data );
				objField->SetValue( obj, *val );
			}
			else if ( datatype->Name == BH::TypeCName<BH::u16>() )
			{
				BH::u16 * val = reinterpret_cast<BH::u16 *>( data );
				objField->SetValue( obj, *val );
			}
			else if ( datatype->Name == BH::TypeCName<BH::s32>() )
			{
				BH::s32 * val = reinterpret_cast<BH::s32 *>( data );
				objField->SetValue( obj, *val );
			}
			else if ( datatype->Name == BH::TypeCName<BH::u32>() )
			{
				BH::u32 * val = reinterpret_cast<BH::u32 *>( data );
				objField->SetValue( obj, *val );
			}
			else if ( datatype->Name == BH::TypeCName<BH::s64>() )
			{
				BH::s64 * val = reinterpret_cast<BH::s64 *>( data );
				objField->SetValue( obj, *val );
			}
			else if ( datatype->Name == BH::TypeCName<BH::u64>() )
			{
				BH::u64 * val = reinterpret_cast<BH::u64 *>( data );
				objField->SetValue( obj, *val );
			}
			else if ( datatype->Name == BH::TypeCName<BH::f32>() )
			{
				BH::f32 * val = reinterpret_cast<BH::f32 *>( data );
				objField->SetValue( obj, *val );
			}
			else if ( datatype->Name == BH::TypeCName<BH::f64>() )
			{
				BH::f64 * val = reinterpret_cast<BH::f64 *>( data );
				objField->SetValue( obj, *val );
			}
			//else
			//{
			//	std::cout << "Not base type!" << std::endl;
			//}
		}

		void SetValueToCSharp( void * data, const BH::Type * datatype, ::System::Object ^ obj, ::System::Type ^ objtype );

		void SetContainerValuesToScriptType( void * data, const BH::Field * containerField, ::System::Object ^ obj, ::System::Type ^ objtype )
		{
			std::vector<BH::Char *> & vec = *reinterpret_cast< std::vector<BH::Char *> * >( data );

			array< ::System::Type ^ > ^ listType = objtype->GetGenericArguments();
			array< ::System::Type ^ > ^ noArgs = gcnew array < ::System::Type ^ > {};
			::System::Reflection::MethodInfo ^ methodInfo = objtype->GetMethod( msclr::interop::marshal_as< ::System::String ^ >( "Add" ) );
			::System::Reflection::MethodInfo ^ clearMethodInfo = objtype->GetMethod( msclr::interop::marshal_as< ::System::String ^ >( "Clear" ) );
			::System::Reflection::ConstructorInfo ^ constructorInfo = listType[0]->GetConstructor( noArgs );

			u32 vecSize = vec.size();
			clearMethodInfo->Invoke( obj, noArgs );
			for ( u32 i = 0; i < vecSize; ++i )
			{
				::System::Object ^ val = constructorInfo->Invoke( noArgs );
				SetValueToCSharp( vec.data() + i, containerField->Container->ValueType, val, listType[0] );

				methodInfo->Invoke( obj, gcnew array< ::System::Object ^ >{ val } );
			}
		}

		void SetValueToCSharp( void * data, const BH::Type * datatype, ::System::Object ^ obj, ::System::Type ^ objtype )
		{
			std::stack<const Type::Fields *> fields = datatype->GetAllFields();

			if ( fields.empty() || fields.top() == nullptr )
				return;

			void ** val = reinterpret_cast< void ** >( data );

			// There should not be a base type, therefore stack size should be 1
			for ( const auto & tfield : *fields.top() )
			{
				void * fieldVal = tfield.GetPtr( *val );
				// If base type, done
				if ( tfield.Type->Name == TypeCName<BH::String>()	||
					 tfield.Type->Name == TypeCName<bool>()			||
					 tfield.Type->Name == TypeCName<BH::s8>()		||
					 tfield.Type->Name == TypeCName<BH::u8>()		||
					 tfield.Type->Name == TypeCName<BH::s16>()		||
					 tfield.Type->Name == TypeCName<BH::u16>()		||
					 tfield.Type->Name == TypeCName<BH::s32>()		||
					 tfield.Type->Name == TypeCName<BH::u32>()		||
					 tfield.Type->Name == TypeCName<BH::s64>()		||
					 tfield.Type->Name == TypeCName<BH::u64>()		||
					 tfield.Type->Name == TypeCName<BH::f32>()		||
					 tfield.Type->Name == TypeCName<BH::f64>() )
				{
					::System::Reflection::FieldInfo ^ objField = objtype->GetField( msclr::interop::marshal_as< ::System::String ^ >( tfield.Name.Text ) );
					SetValueToBaseType( fieldVal, tfield.Type, obj, objField );
				}
				// Else, iterate through every element and recursively call
				else if ( tfield.Container )
				{
					::System::Reflection::FieldInfo ^ objField = objtype->GetField( msclr::interop::marshal_as< ::System::String ^ >( tfield.Name.Text ) );
					if ( tfield.Container->ValueType->Name == TypeCName<BH::String>() )
					{
						//::System::Object ^ list = objField->GetValue( obj );
						//list = MarshalStrVector( *reinterpret_cast< std::vector<BH::String> * >( fieldVal ) );
						objField->SetValue( obj, MarshalStrVector( *reinterpret_cast< std::vector<BH::String> * >( fieldVal ) ) );
					}
					else if ( tfield.Container->ValueType->Name == TypeCName<BH::s8>() )
					{
						//::System::Object ^ list = objField->GetValue( obj );
						//list = MarshalVector( *reinterpret_cast< std::vector<BH::s8> * >( fieldVal ) );
						objField->SetValue( obj, MarshalVector( *reinterpret_cast< std::vector<BH::s8> * >( fieldVal ) ) );
					}
					else if ( tfield.Container->ValueType->Name == TypeCName<BH::u8>() )
					{
						//::System::Object ^ list = objField->GetValue( obj );
						//list = MarshalVector( *reinterpret_cast< std::vector<BH::u8> * >( fieldVal ) );
						objField->SetValue( obj, MarshalVector( *reinterpret_cast< std::vector<BH::u8> * >( fieldVal ) ) );
					}
					else if ( tfield.Container->ValueType->Name == TypeCName<BH::s16>() )
					{
						//::System::Object ^ list = objField->GetValue( obj );
						//list = MarshalVector( *reinterpret_cast< std::vector<BH::s16> * >( fieldVal ) );
						objField->SetValue( obj, MarshalVector( *reinterpret_cast< std::vector<BH::s16> * >( fieldVal ) ) );
					}
					else if ( tfield.Container->ValueType->Name == TypeCName<BH::u16>() )
					{
						//::System::Object ^ list = objField->GetValue( obj );
						//list = MarshalVector( *reinterpret_cast< std::vector<BH::u16> * >( fieldVal ) );
						objField->SetValue( obj, MarshalVector( *reinterpret_cast< std::vector<BH::u16> * >( fieldVal ) ) );
					}
					else if ( tfield.Container->ValueType->Name == TypeCName<BH::s32>() )
					{
						//::System::Object ^ list = objField->GetValue( obj );
						//list = MarshalVector( *reinterpret_cast< std::vector<BH::s32> * >( fieldVal ) );
						objField->SetValue( obj, MarshalVector( *reinterpret_cast< std::vector<BH::s32> * >( fieldVal ) ) );
					}
					else if ( tfield.Container->ValueType->Name == TypeCName<BH::u32>() )
					{
						//::System::Object ^ list = objField->GetValue( obj );
						//list = MarshalVector( *reinterpret_cast< std::vector<BH::u32> * >( fieldVal ) );
						objField->SetValue( obj, MarshalVector( *reinterpret_cast< std::vector<BH::u32> * >( fieldVal ) ) );
					}
					else if ( tfield.Container->ValueType->Name == TypeCName<BH::s64>() )
					{
						//::System::Object ^ list = objField->GetValue( obj );
						//list = MarshalVector( *reinterpret_cast< std::vector<BH::s64> * >( fieldVal ) );
						objField->SetValue( obj, MarshalVector( *reinterpret_cast< std::vector<BH::s64> * >( fieldVal ) ) );
					}
					else if ( tfield.Container->ValueType->Name == TypeCName<BH::u64>() )
					{
						//::System::Object ^ list = objField->GetValue( obj );
						//list = MarshalVector( *reinterpret_cast< std::vector<BH::u64> * >( fieldVal ) );
						objField->SetValue( obj, MarshalVector( *reinterpret_cast< std::vector<BH::u64> * >( fieldVal ) ) );
					}
					else if ( tfield.Container->ValueType->Name == TypeCName<BH::f32>() )
					{
						//::System::Object ^ list = objField->GetValue( obj );
						//list = MarshalVector( *reinterpret_cast< std::vector<BH::f32> * >( fieldVal ) );
						objField->SetValue( obj, MarshalVector( *reinterpret_cast< std::vector<BH::f32> * >( fieldVal ) ) );
					}
					else if ( tfield.Container->ValueType->Name == TypeCName<BH::f64>() )
					{
						//::System::Object ^ list = objField->GetValue( obj );
						//list = MarshalVector( *reinterpret_cast< std::vector<BH::f64> * >( fieldVal ) );
						objField->SetValue( obj, MarshalVector( *reinterpret_cast< std::vector<BH::f64> * >( fieldVal ) ) );
					}
					// C# type
					else
					{
						SetContainerValuesToScriptType( fieldVal, &tfield, objField->GetValue( obj ), objField->FieldType );
					}
				}
				// C# type, recursive call
				else
				{
					::System::Reflection::FieldInfo ^ objField = objtype->GetField( msclr::interop::marshal_as< ::System::String ^ >( tfield.Name.Text ) );
					SetValueToCSharp( fieldVal, tfield.Type, objField->GetValue( obj ), objField->FieldType );
				}
			}
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector<BH::String> & strs )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), MarshalStrVector( strs ) );
		}
		
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::s8 > & values )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), MarshalVector( values ) );
		}
		
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::u8 > & values )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), MarshalVector( values ) );
		}
		
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::s16 > & values )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), MarshalVector( values ) );
		}
		
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::u16 > & values )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), MarshalVector( values ) );
		}
		
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::s32 > & values )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), MarshalVector( values ) );
		}
		
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::u32 > & values )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), MarshalVector( values ) );
		}
		
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::s64 > & values )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), MarshalVector( values ) );
		}
		
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::u64 > & values )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), MarshalVector( values ) );
		}
		
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::f32 > & values )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), MarshalVector( values ) );
		}
		
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::f64 > & values )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			// Set Value
			objType->SetValueToField( o, msclr::interop::marshal_as< ::System::String ^ >( fieldName ), MarshalVector( values ) );
		}

		std::vector<BH::String> ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector<BH::String> & )
		{
			return MarshalStrList( ScriptMethodGetValueImpl< ::System::Collections::Generic::List< ::System::String ^ > ^ >( obj, fieldName ) );
		}
		
		std::vector<BH::s8> ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::s8 > & )
		{
			return MarshalList( ScriptMethodGetValueImpl< ::System::Collections::Generic::List< BH::s8 > ^ >( obj, fieldName ) );
		}
		
		std::vector<BH::u8> ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::u8 > & )
		{
			return MarshalList( ScriptMethodGetValueImpl< ::System::Collections::Generic::List< BH::u8 > ^ >( obj, fieldName ) );
		}
		
		std::vector<BH::s16> ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::s16 > & )
		{
			return MarshalList( ScriptMethodGetValueImpl< ::System::Collections::Generic::List< BH::s16 > ^ >( obj, fieldName ) );
		}
		
		std::vector<BH::u16> ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::u16 > & )
		{
			return MarshalList( ScriptMethodGetValueImpl< ::System::Collections::Generic::List< BH::u16 > ^ >( obj, fieldName ) );
		}
		
		std::vector<BH::s32> ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::s32 > & )
		{
			return MarshalList( ScriptMethodGetValueImpl< ::System::Collections::Generic::List< BH::s32 > ^ >( obj, fieldName ) );
		}
		
		std::vector<BH::u32> ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::u32 > & )
		{
			return MarshalList( ScriptMethodGetValueImpl< ::System::Collections::Generic::List< BH::u32 > ^ >( obj, fieldName ) );
		}
		
		std::vector<BH::s64> ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::s64 > & )
		{
			return MarshalList( ScriptMethodGetValueImpl< ::System::Collections::Generic::List< BH::s64 > ^ >( obj, fieldName ) );
		}
		
		std::vector<BH::u64> ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::u64 > & )
		{
			return MarshalList( ScriptMethodGetValueImpl< ::System::Collections::Generic::List< BH::u64 > ^ >( obj, fieldName ) );
		}
		
		std::vector<BH::f32> ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::f32 > & )
		{
			return MarshalList( ScriptMethodGetValueImpl< ::System::Collections::Generic::List< BH::f32 > ^ >( obj, fieldName ) );
		}
		
		std::vector<BH::f64> ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::f64 > & )
		{
			return MarshalList( ScriptMethodGetValueImpl< ::System::Collections::Generic::List< BH::f64 > ^ >( obj, fieldName ) );
		}
		
		template< typename T >
		std::vector<T> ScriptMethodObjectGetContainerImpl( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			u32 fieldNum = fieldNames.size();
			::System::Object ^ val = nullptr;
			msclr::interop::marshal_context ^ context = gcnew msclr::interop::marshal_context();
			for ( u32 i = 0; i < fieldNum; ++i )
			{
				::System::String ^ fieldName = msclr::interop::marshal_as< ::System::String ^ >( fieldNames[i] );

				// Get value
				val = objType->GetValueFromField( o, fieldName );
				::System::Reflection::FieldInfo ^ fieldInfo = objType->GetFieldType( fieldName );
				::System::String ^ typeName = fieldInfo->FieldType->Name;
				bool isContainer = objType->IsFieldContainer( fieldName );

				const BH::Char * fieldType = BH::Script::GetBuiltInTypeName( context->marshal_as<const BH::Char *>( typeName ) );
				// If Container
				if ( isContainer )
				{
					array <::System::Type ^ > ^ gTypes = fieldInfo->FieldType->GetGenericArguments();
					const BH::Char * valueType = BH::Script::GetBuiltInTypeName( context->marshal_as<const BH::Char *>( gTypes[0]->Name ) );

					if ( valueType == String( BH::TypeName<T>() ) )
						return MarshalList<T>( safe_cast< ::System::Collections::Generic::List<T> ^ >( val ) );
				}
				// Other built in type
				else if ( fieldType != String( "" ) )
				{
					return std::vector<T>();
				}
				// Possible of C# script type
				else
				{
					o = val;
					objType = ScriptManager::Instance->mCompiler->GetType( typeName );
				}
			}

			return std::vector<T>();
		}

		std::vector<String> ScriptMethodObjectGetStrContainerImpl( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			u32 fieldNum = fieldNames.size();
			::System::Object ^ val = nullptr;
			msclr::interop::marshal_context ^ context = gcnew msclr::interop::marshal_context();
			for ( u32 i = 0; i < fieldNum; ++i )
			{
				::System::String ^ fieldName = msclr::interop::marshal_as< ::System::String ^ >( fieldNames[i] );

				// Get value
				val = objType->GetValueFromField( o, fieldName );
				::System::Reflection::FieldInfo ^ fieldInfo = objType->GetFieldType( fieldName );
				::System::String ^ typeName = fieldInfo->FieldType->Name;
				bool isContainer = objType->IsFieldContainer( fieldName );

				const BH::Char * fieldType = BH::Script::GetBuiltInTypeName( context->marshal_as<const BH::Char *>( typeName ) );
				// If Container
				if ( isContainer )
				{
					array <::System::Type ^ > ^ gTypes = fieldInfo->FieldType->GetGenericArguments();
					const BH::Char * valueType = BH::Script::GetBuiltInTypeName( context->marshal_as<const BH::Char *>( gTypes[0]->Name ) );

					if ( valueType == String( BH::TypeName<String>() ) )
						return MarshalStrList( safe_cast< ::System::Collections::Generic::List< ::System::String ^ > ^ >( val ) );
				}
				// Other built in type
				else if ( fieldType != String( "" ) )
				{
					return std::vector<String>();
				}
				// Possible of C# script type
				else
				{
					o = val;
					objType = ScriptManager::Instance->mCompiler->GetType( typeName );
				}
			}

			return std::vector<String>();
		}

		template < typename T >
		void ScriptMethodObjectSetContainerImpl( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector<T> & val )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );
			
			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );
			
			::System::Object ^ oval = nullptr;
			::System::String ^ fieldName = nullptr;
			u32 fieldNum = fieldNames.size();
			msclr::interop::marshal_context ^ context = gcnew msclr::interop::marshal_context();
			for ( u32 i = 0; i < fieldNum; ++i )
			{
				fieldName = msclr::interop::marshal_as< ::System::String ^ >( fieldNames[i] );
				::System::Reflection::FieldInfo ^ fieldInfo = objType->GetFieldType( fieldName );
				::System::String ^ typeName = objType->GetFieldType( fieldName )->FieldType->Name;
				bool isContainer = objType->IsFieldContainer( fieldName );

				oval = objType->GetValueFromField( o, fieldName );
			
				const BH::Char * fieldType = BH::Script::GetBuiltInTypeName( context->marshal_as<const BH::Char *>( typeName ) );
				// Type wanted
				if ( isContainer )
				{
					array <::System::Type ^ > ^ gTypes = fieldInfo->FieldType->GetGenericArguments();
					const BH::Char * valueType = BH::Script::GetBuiltInTypeName( context->marshal_as<const BH::Char *>( gTypes[0]->Name ) );

					// Set Value
					if ( valueType == String( BH::TypeName<T>() ) )
						objType->SetValueToField( o, fieldName, MarshalVector( val ) );

					return;
				}
				// Other built in type
				else if ( fieldType != String( "" ) )
				{
					return;
				}
				// Possible of C# script type
				else
				{
					o = oval;
					objType = ScriptManager::Instance->mCompiler->GetType( typeName );
				}
			}
			
			// Set Value
			objType->SetValueToField( o, fieldName, MarshalVector( val ) );
		}

		void ScriptMethodObjectSetStrContainerImpl( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector<String> & str )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );
			
			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );
			
			::System::Object ^ oval = nullptr;
			::System::String ^ fieldName = nullptr;
			u32 fieldNum = fieldNames.size();
			msclr::interop::marshal_context ^ context = gcnew msclr::interop::marshal_context();
			for ( u32 i = 0; i < fieldNum; ++i )
			{
				fieldName = msclr::interop::marshal_as< ::System::String ^ >( fieldNames[i] );
				::System::Reflection::FieldInfo ^ fieldInfo = objType->GetFieldType( fieldName );
				::System::String ^ typeName = objType->GetFieldType( fieldName )->FieldType->Name;
				bool isContainer = objType->IsFieldContainer( fieldName );

				oval = objType->GetValueFromField( o, fieldName );
			
				const BH::Char * fieldType = BH::Script::GetBuiltInTypeName( context->marshal_as<const BH::Char *>( typeName ) );
				// Type wanted
				if ( isContainer )
				{
					array <::System::Type ^ > ^ gTypes = fieldInfo->FieldType->GetGenericArguments();
					const BH::Char * valueType = BH::Script::GetBuiltInTypeName( context->marshal_as<const BH::Char *>( gTypes[0]->Name ) );

					// Set Value
					if ( valueType == String( BH::TypeName<String>() ) )
						objType->SetValueToField( o, fieldName, MarshalStrVector( str ) );

					return;
				}
				// Other built in type
				else if ( fieldType != String( "" ) )
				{
					return;
				}
				// Possible of C# script type
				else
				{
					o = oval;
					objType = ScriptManager::Instance->mCompiler->GetType( typeName );
				}
			}
			
			// Set Value
			objType->SetValueToField( o, fieldName, MarshalStrVector( str ) );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector<BH::String> & strs )
		{
			ScriptMethodObjectSetStrContainerImpl( obj, fieldNames, strs );
		}
		
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::s8 > & values )
		{
			ScriptMethodObjectSetContainerImpl( obj, fieldNames, values );
		}
		
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::u8 > & values )
		{
			ScriptMethodObjectSetContainerImpl( obj, fieldNames, values );
		}
		
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::s16 > & values )
		{
			ScriptMethodObjectSetContainerImpl( obj, fieldNames, values );
		}
		
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::u16 > & values )
		{
			ScriptMethodObjectSetContainerImpl( obj, fieldNames, values );
		}
		
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::s32 > & values )
		{
			ScriptMethodObjectSetContainerImpl( obj, fieldNames, values );
		}
		
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::u32 > & values )
		{
			ScriptMethodObjectSetContainerImpl( obj, fieldNames, values );
		}
		
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::s64 > & values )
		{
			ScriptMethodObjectSetContainerImpl( obj, fieldNames, values );
		}
		
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::u64 > & values )
		{
			ScriptMethodObjectSetContainerImpl( obj, fieldNames, values );
		}
		
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::f32 > & values )
		{
			ScriptMethodObjectSetContainerImpl( obj, fieldNames, values );
		}
		
		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::f64 > & values )
		{
			ScriptMethodObjectSetContainerImpl( obj, fieldNames, values );
		}

		std::vector<BH::String> ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector<BH::String> & )
		{
			return ScriptMethodObjectGetStrContainerImpl( obj, fieldNames );
		}
		
		std::vector<BH::s8> ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::s8 > & )
		{
			return ScriptMethodObjectGetContainerImpl<BH::s8>( obj, fieldNames );
		}
		
		std::vector<BH::u8> ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::u8 > & )
		{
			return ScriptMethodObjectGetContainerImpl<BH::u8>( obj, fieldNames );
		}
		
		std::vector<BH::s16> ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::s16 > & )
		{
			return ScriptMethodObjectGetContainerImpl<BH::s16>( obj, fieldNames );
		}
		
		std::vector<BH::u16> ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::u16 > & )
		{
			return ScriptMethodObjectGetContainerImpl<BH::u16>( obj, fieldNames );
		}
		
		std::vector<BH::s32> ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::s32 > & )
		{
			return ScriptMethodObjectGetContainerImpl<BH::s32>( obj, fieldNames );
		}
		
		std::vector<BH::u32> ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::u32 > & )
		{
			return ScriptMethodObjectGetContainerImpl<BH::u32>( obj, fieldNames );
		}
		
		std::vector<BH::s64> ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::s64 > & )
		{
			return ScriptMethodObjectGetContainerImpl<BH::s64>( obj, fieldNames );
		}
		
		std::vector<BH::u64> ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::u64 > & )
		{
			return ScriptMethodObjectGetContainerImpl<BH::u64>( obj, fieldNames );
		}
		
		std::vector<BH::f32> ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::f32 > & )
		{
			return ScriptMethodObjectGetContainerImpl<BH::f32>( obj, fieldNames );
		}
		
		std::vector<BH::f64> ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::f64 > & )
		{
			return ScriptMethodObjectGetContainerImpl<BH::f64>( obj, fieldNames );
		}
		
		std::vector<BH::Char *> ScriptMethod::GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const BH::Field * containerField )
		{
			std::vector<BH::Char *> val;
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			::System::Reflection::FieldInfo ^ fieldInfo = objType->GetFieldType( msclr::interop::marshal_as< ::System::String ^ >( fieldName ) );

			GetContainerValuesFromScriptType( &val, containerField, fieldInfo->GetValue( o ), fieldInfo->FieldType );
			return val;
		}

		std::vector<BH::Char *> ScriptMethod::GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const BH::Field * containerField )
		{
			std::vector<BH::Char *> val;

			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			u32 fieldNum = fieldNames.size();
			::System::Object ^ oval = nullptr;
			::System::Reflection::FieldInfo ^ fieldInfo = nullptr;
			for ( u32 i = 0; i < fieldNum; ++i )
			{
				::System::String ^ fieldName = msclr::interop::marshal_as< ::System::String ^ >( fieldNames[i] );

				// Get value
				oval = objType->GetValueFromField( o, fieldName );
				fieldInfo = objType->GetFieldType( fieldName );
				::System::String ^ typeName = fieldInfo->FieldType->Name;
					
				o = oval;
				objType = ScriptManager::Instance->mCompiler->GetType( typeName );
			}

			GetContainerValuesFromScriptType( &val, containerField, fieldInfo->GetValue( o ), fieldInfo->FieldType );
			return val;
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const BH::Field * containerField,	std::vector<BH::Char *> & val )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			::System::Reflection::FieldInfo ^ fieldInfo = objType->GetFieldType( msclr::interop::marshal_as< ::System::String ^ >( fieldName ) );

			SetContainerValuesToScriptType( &val, containerField, fieldInfo->GetValue( o ), fieldInfo->FieldType );
		}

		void ScriptMethod::SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const BH::Field * containerField, std::vector<BH::Char *> & val )
		{
			// Get object
			::System::Object ^ o = ScriptManager::Instance->mCompiler->GetObject( msclr::interop::marshal_as< ::System::String ^ >( obj->GetName() ) );

			// Get Type
			BHScriptCompiler::CSharpType ^ objType = ScriptManager::Instance->mCompiler->GetType( msclr::interop::marshal_as< ::System::String ^ >( obj->GetType() ) );

			u32 fieldNum = fieldNames.size();
			::System::Object ^ oval = nullptr;
			::System::Reflection::FieldInfo ^ fieldInfo = nullptr;
			for ( u32 i = 0; i < fieldNum; ++i )
			{
				::System::String ^ fieldName = msclr::interop::marshal_as< ::System::String ^ >( fieldNames[i] );

				// Get value
				oval = objType->GetValueFromField( o, fieldName );
				fieldInfo = objType->GetFieldType( fieldName );
				::System::String ^ typeName = fieldInfo->FieldType->Name;
					
				o = oval;
				objType = ScriptManager::Instance->mCompiler->GetType( typeName );
			}

			SetContainerValuesToScriptType( &val, containerField, fieldInfo->GetValue( o ), fieldInfo->FieldType );
		}
	}
}