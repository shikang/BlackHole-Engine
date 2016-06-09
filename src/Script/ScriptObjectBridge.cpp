#include "Script/Stdafx.h"

#include "Script/ScriptObjectBridge.h"
#include "Script/ScriptManager.h"

#include <msclr/marshal.h>

#ifdef __cplusplus
extern "C"
{
#endif

	__declspec( dllexport ) ::System::Object ^ GetScriptObject( const BH::Char * name )
	{
		::System::String ^ manName = msclr::interop::marshal_as< ::System::String ^ >( name );
		return BH::detail::ScriptManager::Instance->mObjects.ContainsKey( manName ) ? BH::detail::ScriptManager::Instance->mObjects[manName] : nullptr;
	}

#ifdef __cplusplus
}
#endif