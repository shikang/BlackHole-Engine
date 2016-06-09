#ifndef BH_OBJECTMANAGER_H
#define BH_OBJECTMANAGER_H

#include "Core/CoreEssential.h"
#include "Core/CName.h"
#include "Core/Object.h"
//#include "Poncho/Core/Services.h"
#include "Core/Hash.h"
#include "Core/Event.h"
#include "Core/Uncopyable.h"
#include "Core/Singleton.h"
#include "Core/Deserialiser.h"
#include "Core/Serialiser.h"

// External Dependencies
#include <map>
#include <unordered_map>

namespace BH
{
	typedef Object * ObjectPtr;

	// Object manager. 
	class ObjectManager : private Uncopyable
	{
	public:
		// Event types
		typedef BH::Event< void( ObjectPtr ) >					ObjectChange;		//!< params: object pointer
		typedef BH::Event< void( const Hash &, const Hash & ) >	ObjectRenamed;		//!< params: old name, new name

		typedef ObjectChange ObjectCreate;
		typedef ObjectChange ObjectDestroy;

		// Archetype types
		typedef std::unordered_map< Hash, ObjectPtr > ArcheTypeTable;

	public:
		static const Char* sDefaultObjectName;
		static const Char* sDefaultArcheTypeName;

	public:
		ObjectCreate	OnObjectCreate;		//!< Triggered when an object is created (before initialisation).
		ObjectDestroy	OnObjectDestroy;	//!< Triggered when an object is destroyed (before shutdown).
		ObjectRenamed	OnObjectRenamed;	//!< Triggered when an object is renamed.

	public:
		typedef std::vector< ObjectPtr >				ObjectList;
		typedef std::vector< u32 >						IDList;
		typedef std::unordered_map< CName, u32 >		ObjectNameTable;

	public:
		// Getters
		GETTER_AUTO_CREF( ObjectList, ObjectList );
		GETTER_AUTO_CREF( ArcheTypeTable, ArcheTypeTable );

	public:
		// Constructor.
		ObjectManager();

		// Destructor.
		~ObjectManager();

		// Load ArcheTypes
		template< typename DESERIALISER_TYPE >
		void LoadArcheTypes( const Char * file );

		// Save ArcheTypes
		template< typename SERIALISER_TYPE >
		void SaveArcheTypes( const Char * file );

		// Free memory used in archetypes
		void UnloadArcheTypes();

		// Create ArcheType
		ObjectPtr CreateArcheType( const Hash & archetype );

		// Get ArcheType
		ObjectPtr GetArcheType( const Hash & archetype );

		// Delete archetype
		void DeleteArcheType( const Hash & archetype );

		// Add numbers behind if archetype exist
		void GetAvailableNewArcheType( Hash & archetype );

		// Create an uninitialised object (used for custom object manipulation before initialisation).
		ObjectPtr Create( const CName& name, bool init = true );

		// Create an uninitialised object (used for custom object manipulation before initialisation).
		ObjectPtr CreateFromArcheType( const Hash & archetype, const CName& name, bool init = true );

		// Rename archetype
		void RenameArcheType( const Hash & oldArchetype, const Hash & newArchetype );

		// Deserialise an object from a file.
		template< typename DESERIALISER_TYPE >
		ObjectPtr LoadObjectFromFile( const Char * file, const Char * name = nullptr, bool init = true );

		template< typename DESERIALISER_TYPE >
		void LoadAllObjectFromFile( const Char * file, bool init = true );

		// Save an object to a file using an object name.
		template< typename SERIALISER_TYPE >
		void SaveObjectToFile( const Char * file, const CName& name ) const;

		// Save an object to a file using an object ID.
		template< typename SERIALISER_TYPE >
		void SaveObjectToFile( const Char * file, ObjectID id ) const;

		// Save an object to a file using an object pointer.
		template< typename SERIALISER_TYPE >
		void SaveObjectToFile( const Char * file, ObjectPtr obj, const Char * name = nullptr ) const;

		template< typename SERIALISER_TYPE >
		void SaveObjectsToFile( const Char * file, const ObjectList & obj_list ) const;

		template< typename SERIALISER_TYPE >
		void SaveAllObjectsToFile( const Char * file ) const;

		// Get an existing object by its name.
		ObjectPtr Get( const CName & name ) const;

		// Get an existing object by its name.
		ObjectPtr Get( ObjectID id ) const;

		// Destroy an existing object by its hashed name. Logs an error if object does not exist.
		void Destroy( const CName & name );

		// Destroy an existing object by its ID. Logs an error if object does not exist.
		void Destroy( ObjectID id );

		// Destroy an object by its pointer. Logs an error if object does not exist.
		void Destroy( const ObjectPtr obj );

		// Destroy ALL objects.
		void DestroyAll();

		// Check if an object is to be destroyed on next purge.
		bool IsDestroying( const CName & name ) const;

		// Check if an object is to be destroyed on next purge.
		bool IsDestroying( ObjectID id ) const;

		// Check if an object is to be destroyed on next purge.
		bool IsDestroying( const ObjectPtr obj ) const;

		// Rename object.
		bool Rename( const Hash & oldName, const Hash & newName );

		// Rename object.
		bool Rename( const ObjectPtr obj, const Hash & newName );

		// Perform the actual destruction on objects to be destroyed.
		void Purge();

		// Force a full object purge (REMOVES ALL OBJECTS FROM OBJECT MANAGER).
		void ForceFullPurge();

		// Get an available object name.
		void AssignAvailableObjectName( Hash & name ) const;

		// Deserialises a single object from a dserialiser.
		template< typename OBJECT_TYPE >
		OBJECT_TYPE* DeserialiseObject( Deserialiser & deserialiser, bool init = true );

	public:
		/*
			Initialise an object and its components. Does nothing if already initialised.
			Components will be initialised even if object has been initialised.
		*/
		static void InitObject( ObjectPtr object );

		// Add an externally constructed object to the object table.
		bool Add( ObjectPtr obj );

		// Called when an object has changed internally.
		void PreRename( ObjectPtr obj );

		// Called when an object has changed internally.
		void PostRename( ObjectPtr obj );

		// Add to archetype, return false if archetype has already been added
		bool AddArcheType( ObjectPtr obj );

	private:
		bool				mDestroyAll;		//!< Destroy all objects?
		ObjectList			mObjectList;		//!< Full object list.
		ObjectNameTable		mObjectNameTable;	//!< Object name to ID table.
		IDList				mUnusedIDs;			//!< Unused object ID list.
		IDList				mObjectsToDestroy;	//!< Objects to destroy.
		ArcheTypeTable		mArcheTypeTable;	//!< Table that stores archetype info

		friend class		Game;
		friend class		Object;

	};

}

#include "Core/ObjectManager.inl"

#endif // BH_OBJECTMANAGER_H