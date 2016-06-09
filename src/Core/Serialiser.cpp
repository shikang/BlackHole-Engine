// Precompiled Headers
#include "Core/StdAfx.h"

#include "Core/LogManager.h"
#include "Core/Serialiser.h"

namespace BH
{
	Serialiser::Serialiser()
		: mStream(nullptr)
	{
	}

	Serialiser::~Serialiser()
	{
	}

	Serialiser& Serialiser::Serialise(const Type* type, const void* obj)
	{
		if (!mStream)
		{
			LOGERROR("No stream to serialise to.");
			return *this;
		}
		if (!type)
		{
			LOGERROR("No type specified.");
			return *this;
		}
		if (!obj)
		{
			LOGERROR("No object to serialise to.");
			return *this;
		}

		if (type->IsEntity)
			type = &(reinterpret_cast<ConstEntityPtr>(obj)->GetType());

		mStream = DoSerialise(mStream, type, obj, nullptr, nullptr);

		return *this;
	}
}