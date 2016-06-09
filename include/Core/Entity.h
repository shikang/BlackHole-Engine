#ifndef BH_ENTITY_H
#define BH_ENTITY_H

#include "Core/CoreEssential.h"
#include "Core/Type.h"

// Base entity for all entities that require typing.
// https://bitbucket.org/dwilliamson/reflectabit

namespace BH
{
	// Base entity for all serialisable game objects.
	class Entity
	{
	public:
		// Pre-saved options.
		enum OPTIONS
		{
			O_NORMAL,
			O_SKIP,
			O_IGNORE_FIELDS,

			O_COUNT,
		};

	public:
		// Get entity type. Always assumes that mType was set. (since entity manager creates it)
		inline const Type & GetType() const { return *mType; }

	public:
		// Inline constructor.
		inline Entity() : mType( nullptr ) {}

		// Virtual destructor.
		virtual ~Entity() {}

		// Called just before the internal representation of the entity changes. (e.g. Before deserialisation)
		virtual OPTIONS OnPreChanged() = 0;

		// Called when the internal representation of the entity changes. (e.g. After deserialisation)
		virtual void OnChanged() = 0;

		// Called when the entity is about to be saved. (e.g. Before serialisation)
		virtual OPTIONS OnPreSaved() const = 0;

		// Called after the entity is saved. (e.g. After serialisation)
		virtual void OnSaved() const = 0;

	private:
		const Type *	mType;			//!< Pointer to type.
		friend class	EntityManager;	//!< Befriend entity manager.

	};

	// Entity pointer type.
	typedef Entity *		EntityPtr;
	typedef const Entity *	ConstEntityPtr;
}

// Entity type test macro. Used to test for type dependencies at compile-time.
#define ENTITY_TYPETEST( baseType, childType )\
	static_cast< baseType * >( static_cast< childType * >( nullptr ) )

#endif // BH_ENTITY_H
