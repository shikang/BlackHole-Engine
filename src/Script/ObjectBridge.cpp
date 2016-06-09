#include "Script/ScriptComponentStdAfx.h"

#include "Script/ObjectBridge.h"
#include "Script/ScriptComponent.h"

#include "Core/Object.h"
#include "Core/ObjectManager.h"

#include <vector>

#ifdef __cplusplus
extern "C"
{
#endif

	__declspec( dllexport ) const BH::Char * GetScriptComponent( BH::Object * object, const BH::Char * type )
	{
		std::vector<BH::ScriptComponent *> components = object->GetAllComponent<BH::ScriptComponent>();

		for ( const auto & i : components )
		{
			if( i->GetScript().mType == type )
				return i->GetScriptVariableName();
		}

		return "";
	}

#ifdef __cplusplus
}
#endif