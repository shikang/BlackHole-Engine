#ifndef BH_SCRIPT_H
#define BH_SCRIPT_H

#include "Core/CoreEssential.h"
#include "Script/ScriptConfig.h"
#include "Script/ScriptObject.h"

namespace BH
{
	class BH_API Script
	{
	public:
		// Add script for compilation
		static void AddScript( const Char * filename );

		// Build all script
		static void BuildScripts( TypeDB & typedb );

		// Create object from script
		static ScriptObject CreateScriptObject( const Char * type, const Char * name );

		// Destroy object from script
		static void DestroyScriptObject( ScriptObject & object );

		// Get C++ type name from C# type name
		static const Char * GetBuiltInTypeName( const Char * type );

		// Check if script type exist
		static bool IsScriptType( const Char * scriptType );

		// Get all type name of script types
		static std::vector<String> GetAllScriptTypes();

		// Get all type name of script types from engine
		static std::vector<String> GetAllEngineScriptTypes();

		// Uncompile or unbuild script
		static void UnBuildScript( TypeDB & typedb );

		// Recompile script
		static void ReBuildScript( TypeDB & typedb );

	private:
		// Initialise type map
		static void InitialiseTypeMap();

		// Update script type to reflection
		static void UpdateScriptTypesToReflection( TypeDB & typedb );

		// Remove script type from reflection
		static void RemoveScriptTypesFromReflection( TypeDB & typedb );

		// Dummy Reflected Script Object New Implementation
		static void * NewReflectedDummy( u32 size, const Type * type );

		// Dummy Reflected Script Object Delete Implementation
		static void DeleteReflectedDummy( void * obj, const Type * type );
	};
}

#endif