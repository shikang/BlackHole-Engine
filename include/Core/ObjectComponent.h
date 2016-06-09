#ifndef BH_OBJECT_COMPONENT_H
#define BH_OBJECT_COMPONENT_H

#include "Core/CoreEssential.h"
#include "Core/Entity.h"
#include "Core/Object.h"
#include "Core/SystemManager.h"
#include "Core/Game.h"

namespace BH
{
	// Interface class for object component;
	class ObjectComponent : public Entity
	{
	public:
		// Constructor
		ObjectComponent();

		// Destructor
		~ObjectComponent();

		// Intialise (Called when object is initialised)
		virtual void Initialise();

		// Shutdown (Called when object is destroyed)
		virtual void Shutdown();

		// Called when the holding object has been renamed.
		virtual void OnObjectRenamed(const CName& oldName, const CName& newName);

		// Called just before the internal representation of the entity changes. (e.g. Before deserialisation)
		virtual Entity::OPTIONS OnPreChanged();

		// Called when the internal representation of the entity changes. (e.g. After deserialisation)
		virtual void OnChanged();

		// Called when the entity is about to be saved. (e.g. Before serialisation)
		virtual Entity::OPTIONS OnPreSaved() const;

		// Called after the entity is saved. (e.g. After serialisation)
		virtual void OnSaved() const;

	public:
		// Types
		typedef void( * AddToSystem )( ObjectComponent * component );
		typedef void( * RemoveFromSystem )( ObjectComponent * component );

		// Helper function to add to system
		template< typename ObjectComponentType >
		static void AddToAssignedSystem( ObjectComponent * component )
		{
			SYSTEM_MANAGER.AddObjectComponentToSystem< ObjectComponentType >( component );
		}

		// Helper function to remove from system
		template< typename ObjectComponentType >
		static void RemoveFromAssignedSystem( ObjectComponent * component )
		{
			SYSTEM_MANAGER.RemoveObjectComponentFromSystem< ObjectComponentType >(component);
		}

	public:
		AddToSystem mAddToSystem;			//!< Function pointer to add to system
		RemoveFromSystem mRemoveFromSystem;	//!< Function pointer to remove from system

	protected:
		Object * mObject;					//!< This component belong to this object
		ObjectID mObjectID;					//!< The Object ID
		System * mSystem;					//!< Pointer to the system it belongs to

		friend class System;				//!< Befriend system
		friend class Object;				//!< Befriend object
		friend class ObjectManager;			//!< Befriend Object Manager
		friend class TypeDB;				//!< Befriend type DB.
		friend class EntityManager;			//!< Befriend the entity manager.

	};
}

#define BH_OBJECT_COMPONENT_FRIENDS\
	friend class ::BH::TypeDB

// Decale Object Component Type
DECLARE_TYPE(::BH::ObjectComponent, ObjectComponent);

#endif