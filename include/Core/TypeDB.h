#ifndef BH_TYPEDB_H
#define BH_TYPEDB_H

#include "Core/CoreEssential.h"
#include "Core/Hash.h"
#include "Core/Uncopyable.h"
#include "Core/Type.h"
#include "Core/ObjectAllocator.h"

// External Libraries
#include <unordered_map>

// Type information database.
// https://bitbucket.org/dwilliamson/reflectabit

namespace BH
{
	// Forward Declarations
	class Type;

	// Type database.
	class TypeDB : private Uncopyable
	{
	public:
		// Entity new function signature.
		template< typename TYPE >
		static void * NewEntity();

		// Entity delete function signature.
		template< typename TYPE >
		static void DeleteEntity( void * entity );

	public:
		DECLARE_SINGLETON( TypeDB );

	public:
		// Constructor.
		TypeDB();

		// Destructor.
		~TypeDB();

		// Create new type (returns existing type if already exists).
		template< typename TYPE >
		Type & Create();

		// Gets an existing type.
		template< typename TYPE >
		const Type * Get() const;

		// Gets an existing type.
		const Type * Get( const CName & name ) const;

	public:
		// Create type in runtime
		Type & Create( const CName & name );

		// Remove existing type
		void Remove( const CName & name );

		// Print all existing type
		void PrintAllTypes() const;

	private:
		// Create type implementation.
		template< typename TYPE >
		Type* CreateImpl();

		// New implementation
		void * NewEntityImpl();
		void * NewObjectComponentImpl();

	private:
		typedef std::unordered_map< CName, Type * >	TypeMap;			//!< Type map.

	public:
		GETTER_AUTO_CREF(TypeMap, Types);

	private:
		TypeMap			mTypes;				//!< Type map.
	};
}

#include "TypeDB.inl"

#endif // BH_TYPEDB_H