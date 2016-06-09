// Precompiled Header
#include "Script/Stdafx.h"

#include "Script/ScriptInterOp.h"
#include "Script/ScriptObject.h"
#include "Script/ScriptManager.h"

#include "Core/Hash.h"

namespace BH
{
	namespace detail
	{
		template<>
		void ScriptInterOp::SetValueFromScript<BH::String>( void * data, const BH::Field * field, const std::vector<const BH::Char *> & fieldNames, ScriptObject * scriptObj )
		{
			BH::String * val = reinterpret_cast<BH::String *>( field->GetPtr( data ) );
			detail::ScriptMethod::SetValue( scriptObj, fieldNames, val->c_str() );
		}

		template<>
		void ScriptInterOp::SetValueToScript<BH::String>( void * data, const Field * field, ScriptObject * scriptObj )
		{
			BH::String * val = reinterpret_cast<BH::String *>( field->GetPtr( data ) );
			detail::ScriptMethod::SetValue( scriptObj, field->Name.Text, val->c_str() );
		}

		template<>
		void ScriptInterOp::GetValueFromScript<String>( void * data, const BH::Field * field, const std::vector<const BH::Char *> & fieldNames, ScriptObject * scriptObj )
		{
			BH::String * val = reinterpret_cast<String *>( field->GetPtr( data ) );
			const Char * str = detail::ScriptMethod::GetValue( scriptObj, fieldNames, "" );
			*val = str;
		}

		template<>
		void ScriptInterOp::GetValueFromScript<BH::String>( void * data, const BH::Field * field, ScriptObject * scriptObj )
		{
			BH::String * val = reinterpret_cast<BH::String *>( field->GetPtr( data ) );
			*val = detail::ScriptMethod::GetValue( scriptObj, field->Name.Text, static_cast< const BH::Char * >( nullptr ) );
		}

		template<>
		void ScriptInterOp::GetValueFromScript< std::vector<BH::Char * > >( void * data, const BH::Field * field, ScriptObject * scriptObj )
		{
			std::vector<BH::Char * > * val = reinterpret_cast<std::vector<BH::Char * > *>( field->GetPtr( data ) );
			*val = detail::ScriptMethod::GetValue( scriptObj, field->Name.Text, field );
		}

		template<>
		void ScriptInterOp::GetValueFromScript< std::vector<BH::Char * > >( void * data, const BH::Field * field, const std::vector<const BH::Char *> & fieldNames, ScriptObject * scriptObj )
		{
			std::vector<BH::Char * > * val = reinterpret_cast<std::vector<BH::Char * > *>( field->GetPtr( data ) );
			*val = detail::ScriptMethod::GetValue( scriptObj, fieldNames, field );
		}

		template<>
		void ScriptInterOp::SetValueToScript< std::vector<BH::Char * > >( void * data, const BH::Field * field, ScriptObject * scriptObj )
		{
			std::vector<BH::Char * > * val = reinterpret_cast<std::vector<BH::Char * > *>( field->GetPtr( data ) );
			detail::ScriptMethod::SetValue( scriptObj, field->Name.Text, field, *val );
		}

		template<>
		void ScriptInterOp::SetValueFromScript< std::vector<BH::Char * > >( void * data, const BH::Field * field, const std::vector<const BH::Char *> & fieldNames, ScriptObject * scriptObj )
		{
			std::vector<BH::Char * > * val = reinterpret_cast<std::vector<BH::Char * > *>( field->GetPtr( data ) );
			detail::ScriptMethod::SetValue( scriptObj, fieldNames, field, *val );
		}

		void ScriptInterOp::GetScriptTypeValueFromScript( void * data, const Field * field, std::vector<const BH::Char *> & fieldNames, ScriptObject * scriptObj )
		{
			std::stack<const Type::Fields *> fields = field->Type->GetAllFields();
			void ** val = reinterpret_cast< void ** >( field->GetPtr( data ) );

			if ( !fields.empty() )
			{
				// There should not be a base type, therefore stack size should be 1
				for ( const auto & tfield : *fields.top() )
				{
					fieldNames.push_back( tfield.Name.Text );

					if ( tfield.Type->Name == TypeCName<String>() )
					{
						GetValueFromScript<String>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Type->Name == TypeCName<bool>() )
					{
						GetValueFromScript<bool>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Type->Name == TypeCName<s8>() )
					{
						GetValueFromScript<s8>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Type->Name == TypeCName<u8>() )
					{
						GetValueFromScript<u8>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Type->Name == TypeCName<s16>() )
					{
						GetValueFromScript<s16>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Type->Name == TypeCName<u16>() )
					{
						GetValueFromScript<u16>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Type->Name == TypeCName<s32>() )
					{
						GetValueFromScript<s32>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Type->Name == TypeCName<u32>() )
					{
						GetValueFromScript<u32>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Type->Name == TypeCName<s64>() )
					{
						GetValueFromScript<s64>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Type->Name == TypeCName<u64>() )
					{
						GetValueFromScript<u64>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Type->Name == TypeCName<f32>() )
					{
						GetValueFromScript<f32>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Type->Name == TypeCName<f64>() )
					{
						GetValueFromScript<f64>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Container )
					{
						if ( tfield.Container->ValueType->Name == TypeCName<String>() )
						{
							GetValueFromScript< std::vector<String> >( *val, &tfield, fieldNames, scriptObj );
						}
						else if ( tfield.Container->ValueType->Name == TypeCName<s8>() )
						{
							GetValueFromScript< std::vector<s8> >( *val, &tfield, fieldNames, scriptObj );
						}
						else if ( tfield.Container->ValueType->Name == TypeCName<u8>() )
						{
							GetValueFromScript< std::vector<u8> >( *val, &tfield, fieldNames, scriptObj );
						}
						else if ( tfield.Container->ValueType->Name == TypeCName<s16>() )
						{
							GetValueFromScript< std::vector<s16> >( *val, &tfield, fieldNames, scriptObj );
						}
						else if ( tfield.Container->ValueType->Name == TypeCName<u16>() )
						{
							GetValueFromScript< std::vector<u16> >( *val, &tfield, fieldNames, scriptObj );
						}
						else if ( tfield.Container->ValueType->Name == TypeCName<s32>() )
						{
							GetValueFromScript< std::vector<s32> >( *val, &tfield, fieldNames, scriptObj );
						}
						else if ( tfield.Container->ValueType->Name == TypeCName<u32>() )
						{
							GetValueFromScript< std::vector<u32> >( *val, &tfield, fieldNames, scriptObj );
						}
						else if ( tfield.Container->ValueType->Name == TypeCName<s64>() )
						{
							GetValueFromScript< std::vector<s64> >( *val, &tfield, fieldNames, scriptObj );
						}
						else if ( tfield.Container->ValueType->Name == TypeCName<u64>() )
						{
							GetValueFromScript< std::vector<u64> >( *val, &tfield, fieldNames, scriptObj );
						}
						else if ( tfield.Container->ValueType->Name == TypeCName<f32>() )
						{
							GetValueFromScript< std::vector<f32> >( *val, &tfield, fieldNames, scriptObj );
						}
						else if ( tfield.Container->ValueType->Name == TypeCName<f64>() )
						{
							GetValueFromScript< std::vector<f64> >( *val, &tfield, fieldNames, scriptObj );
						}
						else
						{
							GetValueFromScript< std::vector<Char *> >( *val, &tfield, fieldNames, scriptObj );
						}
					}
					else
					{
						GetScriptTypeValueFromScript( *val, &tfield, fieldNames, scriptObj );
					}

					fieldNames.pop_back();
				}
			}
		}

		void ScriptInterOp::SetScriptTypeValueFromScript( void * data, const Field * field, std::vector<const BH::Char *> & fieldNames, ScriptObject * scriptObj )
		{
			std::stack<const Type::Fields *> fields = field->Type->GetAllFields();
			void ** val = reinterpret_cast< void ** >( field->GetPtr( data ) );
	
			// There should not be a base type, therefore stack size should be 1
			if ( !fields.empty() )
			{
				for ( const auto & tfield : *fields.top() )
				{
					//std::cout << "Pushing back field names: " << tfield.Name.Text << std::endl;
					fieldNames.push_back( tfield.Name.Text );
	
					if ( tfield.Type->Name == TypeCName<String>() )
					{
						//std::cout << "String type!" << std::endl;
						SetValueFromScript<String>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Type->Name == TypeCName<bool>() )
					{
						//std::cout << "bool type!" << std::endl;
						SetValueFromScript<bool>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Type->Name == TypeCName<s8>() )
					{
						//std::cout << "s8 type!" << std::endl;
						SetValueFromScript<s8>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Type->Name == TypeCName<u8>() )
					{
						//std::cout << "u8 type!" << std::endl;
						SetValueFromScript<u8>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Type->Name == TypeCName<s16>() )
					{
						//std::cout << "s16 type!" << std::endl;
						SetValueFromScript<s16>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Type->Name == TypeCName<u16>() )
					{
						//std::cout << "u16 type!" << std::endl;
						SetValueFromScript<u16>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Type->Name == TypeCName<s32>() )
					{
						//std::cout << "s32 type!" << std::endl;
						SetValueFromScript<s32>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Type->Name == TypeCName<u32>() )
					{
						//std::cout << "u32 type!" << std::endl;
						SetValueFromScript<u32>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Type->Name == TypeCName<s64>() )
					{
						//std::cout << "s64 type!" << std::endl;
						SetValueFromScript<s64>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Type->Name == TypeCName<u64>() )
					{
						//std::cout << "u64 type!" << std::endl;
						SetValueFromScript<u64>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Type->Name == TypeCName<f32>() )
					{
						//std::cout << "f32 type!" << std::endl;
						SetValueFromScript<f32>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Type->Name == TypeCName<f64>() )
					{
						//std::cout << "f64 type!" << std::endl;
						SetValueFromScript<f64>( *val, &tfield, fieldNames, scriptObj );
					}
					else if ( tfield.Container )
					{
						//std::cout << "Container type!" << std::endl;
						if ( tfield.Container->ValueType->Name == TypeCName<String>() )
						{
							//std::cout << "Container<String> type!" << std::endl;
							SetValueFromScript< std::vector<String> >( *val, &tfield, fieldNames, scriptObj );
						}
						else if ( tfield.Container->ValueType->Name == TypeCName<s8>() )
						{
							//std::cout << "Container<s8> type!" << std::endl;
							SetValueFromScript< std::vector<s8> >( *val, &tfield, fieldNames, scriptObj );
						}
						else if ( tfield.Container->ValueType->Name == TypeCName<u8>() )
						{
							//std::cout << "Container<u8> type!" << std::endl;
							SetValueFromScript< std::vector<u8> >( *val, &tfield, fieldNames, scriptObj );
						}
						else if ( tfield.Container->ValueType->Name == TypeCName<s16>() )
						{
							//std::cout << "Container<s16> type!" << std::endl;
							SetValueFromScript< std::vector<s16> >( *val, &tfield, fieldNames, scriptObj );
						}
						else if ( tfield.Container->ValueType->Name == TypeCName<u16>() )
						{
							//std::cout << "Container<u16> type!" << std::endl;
							SetValueFromScript< std::vector<u16> >( *val, &tfield, fieldNames, scriptObj );
						}
						else if ( tfield.Container->ValueType->Name == TypeCName<s32>() )
						{
							//std::cout << "Container<s32> type!" << std::endl;
							SetValueFromScript< std::vector<s32> >( *val, &tfield, fieldNames, scriptObj );
						}
						else if ( tfield.Container->ValueType->Name == TypeCName<u32>() )
						{
							//std::cout << "Container<u32> type!" << std::endl;
							SetValueFromScript< std::vector<u32> >( *val, &tfield, fieldNames, scriptObj );
						}
						else if ( tfield.Container->ValueType->Name == TypeCName<s64>() )
						{
							//std::cout << "Container<s64> type!" << std::endl;
							SetValueFromScript< std::vector<s64> >( *val, &tfield, fieldNames, scriptObj );
						}
						else if ( tfield.Container->ValueType->Name == TypeCName<u64>() )
						{
							//std::cout << "Container<u64> type!" << std::endl;
							SetValueFromScript< std::vector<u64> >( *val, &tfield, fieldNames, scriptObj );
						}
						else if ( tfield.Container->ValueType->Name == TypeCName<f32>() )
						{
							//std::cout << "Container<f32> type!" << std::endl;
							SetValueFromScript< std::vector<f32> >( *val, &tfield, fieldNames, scriptObj );
						}
						else if ( tfield.Container->ValueType->Name == TypeCName<f64>() )
						{
							//std::cout << "Container<f64> type!" << std::endl;
							SetValueFromScript< std::vector<f64> >( *val, &tfield, fieldNames, scriptObj );
						}
						else
						{
							//std::cout << "Container<C# type> type!" << std::endl;
							SetValueFromScript< std::vector<Char *> >( *val, &tfield, fieldNames, scriptObj );
						}
					}
					else
					{
						//std::cout << "C# type!" << std::endl;
						SetScriptTypeValueFromScript( *val, &tfield, fieldNames, scriptObj );
					}
	
					fieldNames.pop_back();
				}
			}
		}
	}
}