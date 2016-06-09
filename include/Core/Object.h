#ifndef BH_OBJECT_H
#define BH_OBJECT_H

#include "Core/CoreEssential.h"
#include "Core/Hash.h"
#include "Core/Entity.h"
#include "Core/Uncopyable.h"

#include <vector>

namespace BH
{
	typedef u32 ObjectID;

	class Object :public Entity, private Uncopyable
	{
	public:
		typedef std::vector<ObjectComponent *> ComponentList;	//!< List of components

	public:
		GETTER_AUTO(ObjectID, ID);								//!< Get object's ID

		// Get object's name.
		inline const String & GetName() const { return mName.GetText(); }

		// Get object's hashed name value.
		inline HashValue GetHashValue() const { return mName.GetHash(); }

	public:
		// Constructor (Only object manager creates this)
		Object();

		// Destructor
		~Object();

		// Add a new component type to the table.
		template<typename COMPONENT_TYPE>
		COMPONENT_TYPE * AddComponent();

		// Add a new component type to the table.
		ObjectComponent * AddComponent( const CName & type );

		// Get an existing component from the table. (Prone to race condition and may not be synchronize)
		template<typename COMPONENT_TYPE>
		COMPONENT_TYPE * GetComponent() const;

		// Get an existing component from the table. (Prone to race condition and may not be synchronize)
		ObjectComponent * GetComponent( const CName & type ) const;

		// Get all of the existing component from the table (Prone to race condition and may not be synchronize)
		template<typename COMPONENT_TYPE>
		std::vector<COMPONENT_TYPE *> GetAllComponent() const;

		// Get all of the existing component from the table (Prone to race condition and may not be synchronize)
		std::vector<ObjectComponent *> GetAllComponent( const CName & type ) const;

		// Removes an existing component from the table.
		template<typename COMPONENT_TYPE>
		void RemoveComponent();

		// Removes an existing component from the table.
		void RemoveComponent( const CName & type );

		// Remove component by pointer
		void RemoveComponent( ObjectComponent * component );

		// Assign components to this object
		void ReassignComponent();

	private:
		// Initialise all components
		void Initialise();

		// SHutdown all components
		void Shutdown();

		// Called just before the internal representation of the entity changes. (e.g. Before deserialisation)
		virtual Entity::OPTIONS OnPreChanged();

		// Called when the internal representation of the entity changes. (e.g. After deserialisation)
		virtual void OnChanged();

		// Called when the entity is about to be saved. (e.g. Before serialisation)
		virtual Entity::OPTIONS OnPreSaved() const;

		// Called after the entity is saved. (e.g. After serialisation)
		virtual void OnSaved() const;

	private:
		ComponentList mComponents;								//!< Components
		Hash mName;												//!< Object name
		Hash mType;												//!< Archetype
		ObjectID mID;											//!< Object ID
		ObjectManager * mObjectManager;							//!< Pointer to manager that create this object
		bool mInitialised;										//!< True if initialise

		friend class TypeRegistrar;
		friend class TypeDB;
		friend class EntityManager;
		friend class ObjectManager;

	public:
		// @hack: Current object on changed (stack not needed as there is only 1 object changing at a time)
		static Object * ChangingObject;

	};
}

#include "Core/Object.inl"

// Declare Object Type
DECLARE_TYPE(::BH::Object, Object);

#endif