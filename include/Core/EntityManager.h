#ifndef BH_ENTITYMANAGER_H
#define BH_ENTITYMANAGER_H

#include "Core/CoreEssential.h"
#include "Core/Singleton.h"
#include "Core/TypeDB.h"
#include "Core/Entity.h"

// https://bitbucket.org/dwilliamson/reflectabit

namespace BH
{
	// Entity type manager. Handles type registrations, news and deletes.
	class EntityManager : private Uncopyable
	{
	public:
		DECLARE_SINGLETON(EntityManager);

	public:
		TypeDB & Types;	//!< Type Database

	public:
		// Destructor.
		~EntityManager();

		// Create new entity of a specific type.
		template< typename TYPE >
		TYPE* New() const;

		// Create new entity of a specific type by it's name.
		void* New(const CName & type_name) const;

		// New entity implementation.
		void* New(const Type* type) const;

		// Delete an entity.
		void Delete(EntityPtr entity) const;

		// Delete an entity of specified type
		template< typename TYPE >
		void Delete(TYPE * obj) const;

	private:
		// Constructor. Set as private to prevent unwanted consruction.
		EntityManager();

	};
}

#include "EntityManager.inl"

#define ENTITY_MANAGER\
	::BH::EntityManager::Instance()

#endif // BH_ENTITYMANAGER_H