#include "Core/Assert.h"
#include "Core/Typing.h"

#include "Platform/FileSystem.h"

namespace BH
{
	template< typename DESERIALISER_TYPE >
	void ObjectManager::LoadArcheTypes( const Char * file )
	{
		DESERIALISER_TYPE deserialiser;
		deserialiser.TraverseStream( file );

		ObjectPtr o = nullptr;
		while ( !deserialiser.IsStreamEnded() )
		{
			deserialiser.DeserialisePtr<Object>( o );
			if ( !AddArcheType( o ) )
			{
				ENTITY_MANAGER.Delete( o );
			}

			o = nullptr;
		}
	}
	
	template< typename SERIALISER_TYPE >
	void ObjectManager::SaveArcheTypes( const Char * file )
	{
		SERIALISER_TYPE serialiser;
		serialiser.TraverseStream( file );

		for ( const auto & i : mArcheTypeTable )
			serialiser.Serialise<Object>( *i.second );
	}

	template< typename DESERIALISER_TYPE >
	ObjectPtr ObjectManager::LoadObjectFromFile( const BH::Char * file, const BH::Char * name, bool init )
	{
		//BH_STATIC_ASSERT( false, "Please instaniate serialiser type for this function" ); // Static assert by default.

		DESERIALISER_TYPE deserialiser;
		deserialiser.TraverseStream( file );

		ObjectPtr o = nullptr;
		deserialiser.DeserialisePtr<Object>( o );
		if ( !o )
			return nullptr;

		if ( name )
			o->mName = name;

		if ( !Add( o ) )
		{
			ENTITY_MANAGER.Delete( o );
			return nullptr;
		}
		else if ( init )
		{
			InitObject( o );
		}

		return o;
	}

	// Deserialise an object from a file.
	template< typename DESERIALISER_TYPE >
	void ObjectManager::LoadAllObjectFromFile( const Char * file, bool init )
	{
		DESERIALISER_TYPE deserialiser;
		deserialiser.TraverseStream( file );

		ObjectPtr o = nullptr;
		while ( !deserialiser.IsStreamEnded() )
		{
			deserialiser.DeserialisePtr<Object>( o );
			if ( !Add( o ) )
			{
				ENTITY_MANAGER.Delete( o );
				return;
			}
			else if ( init )
			{
				InitObject( o );
			}

			o = nullptr;
		}
	}

	template< typename SERIALISER_TYPE >
	inline void ObjectManager::SaveObjectToFile( const BH::Char * file, const CName & name ) const
	{
		SaveObjectToFile( file, Get( name ) );
	}

	template< typename SERIALISER_TYPE >
	inline void ObjectManager::SaveObjectToFile( const BH::Char * file, ObjectID id ) const
	{
		SaveObjectToFile( file, Get( id ) );
	}

	template< typename SERIALISER_TYPE >
	void ObjectManager::SaveObjectToFile( const BH::Char * file, ObjectPtr obj, const Char * name ) const
	{
		SERIALISER_TYPE serialiser;
		serialiser.TraverseStream( file );

		Hash originalName = obj->mName;
		if ( name )
			obj->mName = name;

		serialiser.Serialise<Object>( *obj );
		obj->mName = originalName;
	}

	// Save an object to a file using an object pointer.
	template< typename SERIALISER_TYPE >
	void ObjectManager::SaveObjectsToFile( const Char * file, const ObjectList & obj_list ) const
	{
		SERIALISER_TYPE serialiser;
		serialiser.TraverseStream( file );

		for ( const auto & i : obj_list )
			serialiser.Serialise<Object>( *i );
	}

	// Save an object to a file using an object pointer.
	template< typename SERIALISER_TYPE >
	void ObjectManager::SaveAllObjectsToFile( const Char * file ) const
	{
		if ( FileSystem::FileCheckExists( file ) )
			FileSystem::FileDelete( "", file );

		SERIALISER_TYPE serialiser;
		serialiser.TraverseStream( file );

		for ( const auto & i : mObjectList )
			serialiser.Serialise<Object>( *i );
	}

	inline ObjectPtr ObjectManager::Get( const CName & name ) const
	{
		ObjectNameTable::const_iterator findit = mObjectNameTable.find( name );
		return ( findit != mObjectNameTable.end() ) ? mObjectList[findit->second] : nullptr;
	}

	inline ObjectPtr ObjectManager::Get( ObjectID id ) const
	{
		return ( id < mObjectList.size() ) ? mObjectList[id] : nullptr;
	}

	inline void ObjectManager::Destroy( const CName& name )
	{
		ObjectNameTable::const_iterator findit = mObjectNameTable.find( name );
		if ( findit == mObjectNameTable.end() )
		{
			LOGERROR( "Object name doesn't exist: " << name );
			return;
		}
		Destroy( findit->second );
	}

	inline void ObjectManager::Destroy( const ObjectPtr obj )
	{
		Destroy( obj->GetID() );
	}

	inline bool ObjectManager::IsDestroying( const CName & name ) const
	{
		ObjectNameTable::const_iterator findit = mObjectNameTable.find( name );
		return ( findit != mObjectNameTable.end() ) ? IsDestroying( findit->second ) : false;
	}

	inline bool ObjectManager::IsDestroying( ObjectID id ) const
	{
		return std::find( mObjectsToDestroy.begin(), mObjectsToDestroy.end(), id ) != mObjectsToDestroy.end();
	}

	inline bool ObjectManager::IsDestroying( const ObjectPtr obj ) const
	{
		return IsDestroying( obj->GetID() );
	}

	inline bool ObjectManager::Rename( const ObjectPtr obj, const Hash & newName )
	{
		return Rename( obj->mName, newName );
	}
}