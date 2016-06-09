#ifndef BH_SCRIPT_MANAGER_H
#define BH_SCRIPT_MANAHER_H

#include "Core/Assert.h"
#include "Script/ScriptObject.h"

namespace BH
{
	namespace detail
	{
		// Managed class for calling C# functions
		public ref class ScriptManager
		{
		private:
			// CLI class need to be gcnew
			ScriptManager::ScriptManager()
			: mCompiler( gcnew BHScriptCompiler::CSharpRTCompiler() )
			{
			}

		public:
			BHScriptCompiler::CSharpRTCompiler ^ mCompiler;				//!< Compiler

			static ScriptManager ^ Instance = gcnew ScriptManager();	//!< Singleton instance

		};

	}
}

#endif