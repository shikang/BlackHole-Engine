// Precompiled Headers
#include "Core/StdAfx.h"

#include "Core/Hash.h"
#include "Core/Assert.h"

using namespace BH;

void Hash::ToString(String & str, const Type *, const void * obj, const FieldAttributes *)
{
	str = reinterpret_cast<const Hash*>(obj)->GetText();
}

void Hash::FromString(const String & str, const Type *, void * obj, const FieldAttributes *)
{
	*( reinterpret_cast<Hash*>(obj) ) = str;
}

Hash::~Hash()
{
}

Hash& Hash::operator = (const Char* name)
{
	mText = name ? name : "";
	mHash = name ? Generate( mText ) : 0;
	return *this;
}

Hash& Hash::operator = (const String& name)
{
	mText = name;
	mHash = Generate( mText );
	return *this;
}