// Precompiled Header
#include "Script/Stdafx.h"

#include "Script/ScriptObject.h"

namespace BH
{
	const Char * ScriptObject::InvalidTypeName = "[InvalidType]";
	const Char * ScriptObject::InvalidObjectName = "[InvalidName]";

	ScriptObject::ScriptObject( const Char * name, const Char * type )
	: mName( name )
	, mType( type )
	{
	}

	void ScriptObject::CallMethod( const Char * method )
	{
		detail::ScriptMethod::CallMethod( this, method );
	}
}