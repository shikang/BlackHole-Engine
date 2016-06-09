#ifndef BH_SCRIPT_INTEROP_H
#define BH_SCRIPT_INTEROP_H

#include "Core/CoreEssential.h"
#include "Script/ScriptConfig.h"
#include "Script/ScriptMethod.h"

#include <vector>

namespace BH
{
	class ScriptObject;
	class Type;
	class Field;

	namespace detail
	{
		class BH_API ScriptInterOp
		{
		public:
			// Get Value from script
			template< typename T >
			static void GetValueFromScript( void * data, const BH::Field * field, ScriptObject * scriptObj );

			// Set Value to script
			template< typename T >
			static void SetValueToScript( void * data, const BH::Field * field, ScriptObject * scriptObj );

			// Get Script type value from script
			static void GetScriptTypeValueFromScript( void * data, const BH::Field * field, std::vector<const BH::Char *> & fieldNames, ScriptObject * scriptObj );

			// Set Script type value to script
			static void SetScriptTypeValueFromScript( void * data, const BH::Field * field, std::vector<const BH::Char *> & fieldNames, ScriptObject * scriptObj );

		private:
			// Set value to script (fieldNames is the recurring fields)
			template< typename T >
			static void SetValueFromScript( void * data, const BH::Field * field, const std::vector<const BH::Char *> & fieldNames, ScriptObject * scriptObj );

			// Get value from script (fieldNames is the recurring fields)
			template< typename T >
			static void GetValueFromScript( void * data, const BH::Field * field, const std::vector<const BH::Char *> & fieldNames, ScriptObject * scriptObj );

		};

		template<>
		BH_API void ScriptInterOp::GetValueFromScript<BH::String>( void * data, const BH::Field * field, ScriptObject * scriptObj );

		template<>
		BH_API void ScriptInterOp::GetValueFromScript<BH::String>( void * data, const BH::Field * field, const std::vector<const BH::Char *> & fieldNames, ScriptObject * scriptObj );

		template<>
		BH_API void ScriptInterOp::SetValueToScript<BH::String>( void * data, const BH::Field * field, ScriptObject * scriptObj );

		template<>
		BH_API void ScriptInterOp::SetValueFromScript<BH::String>( void * data, const BH::Field * field, const std::vector<const BH::Char *> & fieldNames, ScriptObject * scriptObj );

		template<>
		BH_API void ScriptInterOp::GetValueFromScript< std::vector<BH::Char * > >( void * data, const BH::Field * field, ScriptObject * scriptObj );

		template<>
		BH_API void ScriptInterOp::GetValueFromScript< std::vector<BH::Char * > >( void * data, const BH::Field * field, const std::vector<const BH::Char *> & fieldNames, ScriptObject * scriptObj );

		template<>
		BH_API void ScriptInterOp::SetValueToScript< std::vector<BH::Char * > >( void * data, const BH::Field * field, ScriptObject * scriptObj );

		template<>
		BH_API void ScriptInterOp::SetValueFromScript< std::vector<BH::Char * > >( void * data, const BH::Field * field, const std::vector<const BH::Char *> & fieldNames, ScriptObject * scriptObj );
	}
}

#include "Script/ScriptInterOp.inl"

#endif