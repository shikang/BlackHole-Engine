#ifndef BH_OBJECT_BRIDGE_H
#define BH_OBJECT_BRIDGE_H

#include "Core/CoreEssential.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// Get script component (return variable's name)
	__declspec( dllexport ) const BH::Char * GetScriptComponent( BH::Object * object, const BH::Char * type );

#ifdef __cplusplus
}
#endif

#endif