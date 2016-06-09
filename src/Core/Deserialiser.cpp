// Precompiled Headers
#include "Core/StdAfx.h"

#include "Core/Deserialiser.h"
#include "Core/Serialiser.h"
#include "Core/LogManager.h"

using namespace BH;

Deserialiser::Deserialiser()
: mStream(nullptr)
{
}

Deserialiser::~Deserialiser()
{
}

bool Deserialiser::Deserialise(const Type* type, void*& obj)
{
	if (!mStream)
	{
		LOGERROR("No stream to serialise to.");
		return false;
	}
	if (!type)
	{
		LOGERROR("No type specified.");
		return false;
	}

	if (obj && type->IsEntity)
		type = &(reinterpret_cast<EntityPtr>(obj)->GetType());

	if (mStream)
	{
		mStream = DoDeserialise(mStream, type, obj, nullptr, nullptr, true);
		return true;
	}

	return false;
}