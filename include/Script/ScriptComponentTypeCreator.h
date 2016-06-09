#ifndef BH_SCRIPTCOMPONENT_TYPECREATOR_H
#define BH_SCRIPTCOMPONENT_TYPECREATOR_H

#include "Script/ScriptComponent.h"

BEGIN_DEFINE_TYPE( ::BH::ScriptType )
	ENTITY_MANAGER.Types.Create< ::BH::ScriptType >()
						.FIELDM( Type, ::BH::ScriptType );
END_DEFINE_TYPE( ::BH::ScriptType )

BEGIN_DEFINE_TYPE( ::BH::ScriptComponent )
	ENTITY_MANAGER.Types.Create< ::BH::ScriptComponent >()
						.FIELDM( Script, ::BH::ScriptComponent )
						.FIELDM( Data, ::BH::ScriptComponent )
						.Base< ::BH::ObjectComponent >();
END_DEFINE_TYPE( ::BH::ScriptComponent )

#endif