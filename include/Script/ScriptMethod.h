#ifndef BH_SCRIPT_METHOD_H
#define BH_SCRIPT_METHOD_H

#include "Core/CoreEssential.h"
#include "Script/ScriptConfig.h"

#include <vector>

namespace BH
{
	class ScriptObject;
	class Type;
	class Field;

	namespace detail
	{
		/*
			If I templatize, I have to compile with \clr options for
			those project using it as the cli code is in the header.
			\clr compilation causes too many restriction.

			This class handles calling of method in C# script
			*/
		class BH_API ScriptMethod
		{
		public:
			// Push Params to stack (C-String)
			static void PushParamsToStack( const BH::Char * str );

			// Push Params to stack (built in type)
			static void PushParamsToStack( bool val );
			static void PushParamsToStack( BH::s8 val );
			static void PushParamsToStack( BH::u8 val );
			static void PushParamsToStack( BH::s16 val );
			static void PushParamsToStack( BH::u16 val );
			static void PushParamsToStack( BH::s32 val );
			static void PushParamsToStack( BH::u32 val );
			static void PushParamsToStack( BH::s64 val );
			static void PushParamsToStack( BH::u64 val );
			static void PushParamsToStack( BH::f32 val );
			static void PushParamsToStack( BH::f64 val );

			// Push Params to stack (ScriptObject)
			static void PushParamsToStack( BH::ScriptObject * obj );

			// Call method, empty stack
			static void CallMethod( BH::ScriptObject * obj, const BH::Char * methodName );

		public:
			// Set value at field (C-String)
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const BH::Char * str );

			// Set value at field (built in type)
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, bool val );
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::s8 val );
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::u8 val );
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::s16 val );
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::u16 val );
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::s32 val );
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::u32 val );
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::s64 val );
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::u64 val );
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::f32 val );
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::f64 val );

			// Set value at field (ScriptObject)
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::ScriptObject * val );

			// Set value at Script type field's field (C-String) - first element is the first field name, afterwards, it's recursive
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const BH::Char * str );

			// Set value at Script type field's field (built in type) - first element is the first field name, afterwards, it's recursive
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, bool val );
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::s8 val );
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::u8 val );
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::s16 val );
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::u16 val );
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::s32 val );
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::u32 val );
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::s64 val );
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::u64 val );
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::f32 val );
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::f64 val );

			// Get value at field (C-String)
			static const BH::Char * GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const BH::Char * str );

			// Get value at field (built in type)
			static bool GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, bool val );
			static BH::s8 GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::s8 val );
			static BH::u8 GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::u8 val );
			static BH::s16 GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::s16 val );
			static BH::u16 GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::u16 val );
			static BH::s32 GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::s32 val );
			static BH::u32 GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::u32 val );
			static BH::s64 GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::s64 val );
			static BH::u64 GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::u64 val );
			static BH::f32 GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::f32 val );
			static BH::f64 GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, BH::f64 val );

			// Get value at Script type field's field (C-String) - first element is the first field name, afterwards, it's recursive
			static const BH::Char * GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const BH::Char * str );

			// Get value at Script type field's field (built in type) - first element is the first field name, afterwards, it's recursive
			static bool GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, bool val );
			static BH::s8 GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::s8 val );
			static BH::u8 GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::u8 val );
			static BH::s16 GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::s16 val );
			static BH::u16 GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::u16 val );
			static BH::s32 GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::s32 val );
			static BH::u32 GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::u32 val );
			static BH::s64 GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::s64 val );
			static BH::u64 GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::u64 val );
			static BH::f32 GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::f32 val );
			static BH::f64 GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, BH::f64 val );

		public:
			// Set value at container field (C-String)
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector<BH::String> & strs );

			// Set value at container field (built in type)
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::s8 > & values );
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::u8 > & values );
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::s16 > & values );
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::u16 > & values );
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::s32 > & values );
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::u32 > & values );
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::s64 > & values );
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::u64 > & values );
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::f32 > & values );
			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::f64 > & values );

			// Get value at container field (C-String)
			static std::vector<BH::String> GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector<BH::String> & strs );

			// Get value at container field (built in type)
			static std::vector<BH::s8> GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::s8 > & values );
			static std::vector<BH::u8> GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::u8 > & values );
			static std::vector<BH::s16> GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::s16 > & values );
			static std::vector<BH::u16> GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::u16 > & values );
			static std::vector<BH::s32> GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::s32 > & values );
			static std::vector<BH::u32> GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::u32 > & values );
			static std::vector<BH::s64> GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::s64 > & values );
			static std::vector<BH::u64> GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::u64 > & values );
			static std::vector<BH::f32> GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::f32 > & values );
			static std::vector<BH::f64> GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const std::vector< BH::f64 > & values );

			// Set value at Script type field's container field (C-String) - first element is the first field name, afterwards, it's recursive
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector<BH::String> & strs );

			// Set value at Script type field's container field (built in type) - first element is the first field name, afterwards, it's recursive
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::s8 > & values );
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::u8 > & values );
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::s16 > & values );
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::u16 > & values );
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::s32 > & values );
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::u32 > & values );
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::s64 > & values );
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::u64 > & values );
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::f32 > & values );
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::f64 > & values );

			// Get value at Script type field's field (C-String) - first element is the first field name, afterwards, it's recursive
			static std::vector<BH::String> GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector<BH::String> & strs );

			// Get value at Script type field's field (built in type) - first element is the first field name, afterwards, it's recursive
			static std::vector<BH::s8> GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::s8 > & values );
			static std::vector<BH::u8> GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::u8 > & values );
			static std::vector<BH::s16> GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::s16 > & values );
			static std::vector<BH::u16> GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::u16 > & values );
			static std::vector<BH::s32> GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::s32 > & values );
			static std::vector<BH::u32> GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::u32 > & values );
			static std::vector<BH::s64> GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::s64 > & values );
			static std::vector<BH::u64> GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::u64 > & values );
			static std::vector<BH::f32> GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::f32 > & values );
			static std::vector<BH::f64> GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const std::vector< BH::f64 > & values );

			// Get script type at container field
			static std::vector<BH::Char *> GetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const BH::Field * containerField );
			static std::vector<BH::Char *> GetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const BH::Field * containerField );

			static void SetValue( BH::ScriptObject * obj, const BH::Char * fieldName, const BH::Field * containerField,	std::vector<BH::Char *> & val );
			static void SetValue( BH::ScriptObject * obj, const std::vector<const BH::Char *> & fieldNames, const BH::Field * containerField, std::vector<BH::Char *> & val );
		};
	}
}

#endif