#include "Core/Hash.h"

namespace BH
{
	namespace detail
	{
		template< typename T >
		void ScriptInterOp::GetValueFromScript( void * data, const BH::Field * field, ScriptObject * scriptObj )
		{
			T * val = reinterpret_cast<T *>( field->GetPtr( data ) );
			*val = detail::ScriptMethod::GetValue( scriptObj, field->Name.Text, T() );
		}

		template< typename T >
		void ScriptInterOp::GetValueFromScript( void * data, const BH::Field * field, const std::vector<const BH::Char *> & fieldNames, ScriptObject * scriptObj )
		{
			T * val = reinterpret_cast<T *>( field->GetPtr( data ) );
			*val = detail::ScriptMethod::GetValue( scriptObj, fieldNames, T() );
		}

		template< typename T >
		void ScriptInterOp::SetValueToScript( void * data, const BH::Field * field, ScriptObject * scriptObj )
		{
			T * val = reinterpret_cast<T *>( field->GetPtr( data ) );
			detail::ScriptMethod::SetValue( scriptObj, field->Name.Text, *val );
		}

		template< typename T >
		void ScriptInterOp::SetValueFromScript( void * data, const BH::Field * field, const std::vector<const BH::Char *> & fieldNames, ScriptObject * scriptObj )
		{
			T * val = reinterpret_cast<T *>( field->GetPtr( data ) );
			detail::ScriptMethod::SetValue( scriptObj, fieldNames, *val );
		}
	}
}