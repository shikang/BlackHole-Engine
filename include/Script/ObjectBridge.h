#ifndef BH_OBJECT_BRIDGE_H
#define BH_OBJECT_BRIDGE_H

#include "Platform/Platform.h"
#include "Core/CoreEssential.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// Get script component (return variable's name)
	BH_DLL_EXPORT const BH::Char * GetScriptComponent( BH::Object * object, const BH::Char * type );

#ifdef __cplusplus
}
#endif

#endif