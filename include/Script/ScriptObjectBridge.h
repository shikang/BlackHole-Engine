#ifndef BH_SCRIPTOBJECT_BRIDGE_H
#define BH_SCRIPTOBJECT_BRIDGE_H

#include "Core/CoreEssential.h"

#ifdef __cplusplus
extern "C"
{
#endif
	// Get object pointer
	__declspec( dllexport ) ::System::Object ^ GetScriptObject( const BH::Char * name );

#ifdef __cplusplus
}
#endif

#endif