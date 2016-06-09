// Precompiled Headers
#include "Core/StdAfx.h"

#include "Core/EntityManager.h"

namespace BH
{
	EntityManager::EntityManager()
	: Types( TypeDB::Instance() )
	{
		TypeRegistrar::RegisterTypes();
	}

	EntityManager::~EntityManager()
	{
	}

	void EntityManager::Delete(EntityPtr ent) const
	{
		const Type * type = ent->mType;
		type->Delete(ent);
	}

	void* EntityManager::New(const Type* type) const
	{
		void * obj = type->New();

		if (obj && type->IsEntity)
			static_cast<EntityPtr>(obj)->mType = type;

		return obj;
	}
}