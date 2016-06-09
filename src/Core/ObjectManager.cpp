// Precompiled Headers
#include "Core/StdAfx.h"

#include "Core/ObjectManager.h"
#include "Core/EntityManager.h"
#include "Core/Assert.h"
#include "Core/LogManager.h"
#include "Platform/FileSystem.h"

namespace BH
{

	const Char* ObjectManager::sDefaultObjectName = "NewObject";
	const Char* ObjectManager::sDefaultArcheTypeName = "NewArcheType";

	ObjectManager::ObjectManager()
	: mDestroyAll( false )
	{
	}

	ObjectManager::~ObjectManager()
	{
		ForceFullPurge();
		UnloadArcheTypes();
	}

	void ObjectManager::UnloadArcheTypes()
	{
		// Clear ArcheTypes
		for ( auto & i : mArcheTypeTable )
		{
			//for ( auto & j : i.second->mComponents )
			//{
			//	j->Shutdown();
			//}

			ENTITY_MANAGER.Delete( static_cast<EntityPtr>( i.second ) );
		}

		mArcheTypeTable.clear();
	}

	ObjectPtr ObjectManager::CreateArcheType( const Hash & archetype )
	{
		// ArcheType already exist
		if ( mArcheTypeTable.find( archetype ) != mArcheTypeTable.end() )
		{
			LOGWARNING( "ObjectManager::CreateArcheType - Editing existing archetype" );
		}
		else
		{
			LOG( "ObjectManager::CreateArcheType - Creating new archetype" );
			mArcheTypeTable[archetype] = ENTITY_MANAGER.New<Object>();
			mArcheTypeTable[archetype]->mType = archetype;
			mArcheTypeTable[archetype]->mName = archetype.GetText() + "ArcheType";
			mArcheTypeTable[archetype]->mObjectManager = this;
		}

		return mArcheTypeTable[archetype];
	}

	ObjectPtr ObjectManager::GetArcheType( const Hash & archetype )
	{
		ArcheTypeTable::iterator it = mArcheTypeTable.find( archetype );
		if ( it == mArcheTypeTable.end() )
			return nullptr;

		return it->second;
	}

	void ObjectManager::DeleteArcheType( const Hash & archetype )
	{
		// If archetype exist
		if ( mArcheTypeTable.find( archetype ) != mArcheTypeTable.end() )
		{
			ENTITY_MANAGER.Delete( static_cast<EntityPtr>( mArcheTypeTable[archetype] ) );
			mArcheTypeTable.erase( archetype );
		}
	}

	void ObjectManager::GetAvailableNewArcheType( Hash & archetype )
	{
		ArcheTypeTable::const_iterator findit = mArcheTypeTable.find( archetype );
		// If no available name found
		if ( findit == mArcheTypeTable.end() && ( archetype.GetText().length() > 0 ) )
			return;

		String origName = ( archetype.GetText().length() > 0 ) ? archetype.GetText() : sDefaultObjectName;
		s32 postfix = 0;

		// If there are numbers at the back
		if ( origName.back() >= '0' && origName.back() <= '9' )
		{
			// Points at last character
			const Char * endPt = origName.c_str() + origName.length() - 1;

			// Get the point where the object's number starts
			while ( endPt >= origName.c_str() - 1 )	// While end pt does does point to one before the first
			{
				// If endpoint is numeric, move back
				if ( endPt >= origName.c_str() && *endPt >= '0' && *endPt <= '9' )
				{
					--endPt;
				}
				// Finally reach a non numeric character
				else
				{
					// Postfix = accquire number + 1
					std::istringstream ss( endPt + 1 );
					ss >> postfix;
					++postfix;
					origName = String( origName.c_str(), endPt + 1 - origName.c_str() );
					break;
				}
			}
		}

		// Make sure new name does not have duplicates
		while ( findit != mArcheTypeTable.end() )
		{
			std::stringstream newName;
			newName << origName << postfix++;
			archetype = newName.str().c_str();
			findit = mArcheTypeTable.find( archetype );
		}
	}

	void ObjectManager::RenameArcheType( const Hash & oldArchetype, const Hash & newArchetype )
	{
		ArcheTypeTable::iterator oit = mArcheTypeTable.find( oldArchetype );
		ArcheTypeTable::iterator nit = mArcheTypeTable.find( newArchetype );
		if ( oit == mArcheTypeTable.end() || nit != mArcheTypeTable.end() )
			return;

		Object * obj = oit->second;
		mArcheTypeTable.erase( oit );
		obj->mType = newArchetype;
		obj->mName = newArchetype.GetText() + "ArcheType";
		AddArcheType( obj );
	}

	ObjectPtr ObjectManager::Create( const CName & name, bool init )
	{
		EntityPtr newEnt = reinterpret_cast<EntityPtr>( ENTITY_MANAGER.New<Object>() );
		if ( !newEnt )
		{
			LOGERROR( "Cannot create object (" << name << ")." );
			return nullptr;
		}

		ObjectPtr newObj = static_cast<ObjectPtr>( newEnt );
		newObj->mName = ( name.Hash == 0 ) ? sDefaultObjectName : name;

		if ( !Add( newObj ) )
		{
			ENTITY_MANAGER.Delete( newEnt );
			return nullptr;
		}

		if ( init )
			InitObject( newObj );

		return newObj;
	}

	ObjectPtr ObjectManager::CreateFromArcheType( const Hash & archetype, const CName& name, bool init )
	{
		// ArcheType does not exist
		if ( mArcheTypeTable.find( archetype ) == mArcheTypeTable.end() )
			return nullptr;

		EntityPtr newEnt = reinterpret_cast<EntityPtr>( ENTITY_MANAGER.New<Object>() );
		if ( !newEnt )
		{
			LOGERROR( "Cannot create object (" << name << ")." );
			return nullptr;
		}

		ObjectPtr newObj = static_cast<ObjectPtr>( newEnt );
		newObj->mName = ( name.Hash == 0 ) ? sDefaultObjectName : name;
		newObj->mType = archetype;

		if ( !Add( newObj ) )
		{
			ENTITY_MANAGER.Delete( newEnt );
			return nullptr;
		}

		if ( init )
			InitObject( newObj );

		return newObj;
	}

	void ObjectManager::Destroy( ObjectID id )
	{
		if ( id >= mObjectList.size() || mObjectList[id] == nullptr )
		{
			LOGERROR( "Object ID not found: " << id );
			return;
		}

		mObjectsToDestroy.push_back( id );
	}

	void ObjectManager::DestroyAll()
	{
		mDestroyAll = true;
	}

	bool ObjectManager::Rename( const Hash & oldName, const Hash & newName )
	{
		if ( oldName == newName )
		{
			LOGERROR( "Object not renamed. Same name supplied." );
			return false;
		}

		ObjectNameTable::iterator findit = mObjectNameTable.find( CName( oldName ) );
		if ( findit == mObjectNameTable.end() )
		{
			LOGERROR( "Object (" << oldName << ") not found." );
			return false;
		}

		if ( mObjectNameTable.find( CName( newName ) ) != mObjectNameTable.end() )
		{
			LOGERROR( "Object (" << newName << ") name already exists." );
			return false;
		}

		// Trigger the object renaming events.
		ObjectPtr obj = mObjectList[findit->second];

		for ( auto & i : obj->mComponents )
			i->OnObjectRenamed( CName( oldName ), CName( newName ) );

		OnObjectRenamed( CName( oldName ), CName( newName ) );

		// Remove and re-add the object from the table.
		mObjectNameTable.erase( findit );

		// Reassign the object's name.
		obj->mName = newName;

		// Reinsert object into the table.
		mObjectNameTable.insert( std::make_pair( CName( obj->mName ), obj->GetID() ) );

		return true;
	}

	void ObjectManager::InitObject( ObjectPtr obj )
	{
		if ( !obj )
			return;

		if ( !obj->mInitialised )
		{
			obj->Initialise();
			obj->mInitialised = true;
		}
	}

	void ObjectManager::Purge()
	{
		if ( mDestroyAll )
		{
			ForceFullPurge();
		}
		else
		{
			for ( const auto & id : mObjectsToDestroy )
			{
				if ( id < mObjectList.size() && mObjectList[id] != nullptr )
				{
					ObjectPtr del = mObjectList[id];
					OnObjectDestroy( del );

					// Remove it from the object name table.
					mObjectNameTable.erase( CName( del->mName ) );

					// Delete the object.
					del->Shutdown();
					ENTITY_MANAGER.Delete( static_cast<EntityPtr>( del ) );

					// Put the ID in the unused list.
					mUnusedIDs.push_back( id );
					mObjectList[id] = nullptr;
				}
			}

			mObjectsToDestroy.clear();
		}
	}

	void ObjectManager::ForceFullPurge()
	{
		for ( auto & i : mObjectList )
		{
			if ( i != nullptr )
			{
				OnObjectDestroy( i );
				i->Shutdown();
				ENTITY_MANAGER.Delete( static_cast<EntityPtr>( i ) );
				i = nullptr;
			}
		}

		mObjectList.clear();
		mUnusedIDs.clear();
		mObjectsToDestroy.clear();
		mObjectNameTable.clear();
		mDestroyAll = false;
	}

	void ObjectManager::AssignAvailableObjectName( Hash & name ) const
	{
		ObjectNameTable::const_iterator findit = mObjectNameTable.find( CName( name ) );
		// If no available name found
		if ( findit == mObjectNameTable.end() && ( name.GetText().length() > 0 ) )
			return;

		String origName = ( name.GetText().length() > 0 ) ? name.GetText() : sDefaultObjectName;
		s32 postfix = 0;

		// If there are numbers at the back
		if ( origName.back() >= '0' && origName.back() <= '9' )
		{
			// Points at last character
			const Char * endPt = origName.c_str() + origName.length() - 1;

			// Get the point where the object's number starts
			while ( endPt >= origName.c_str() - 1 )	// While end pt does does point to one before the first
			{
				// If endpoint is numeric, move back
				if ( endPt >= origName.c_str() && *endPt >= '0' && *endPt <= '9' )
				{
					--endPt;
				}
				// Finally reach a non numeric character
				else
				{
					std::istringstream ss( endPt + 1 );
					ss >> postfix;
					++postfix;
					origName = String( origName.c_str(), endPt + 1 - origName.c_str() );
					break;
				}
			}
		}

		// Make sure new name does not have duplicates
		while ( findit != mObjectNameTable.end() )
		{
			std::stringstream newName;
			newName << origName << postfix++;
			name = newName.str().c_str();
			findit = mObjectNameTable.find( CName( name ) );
		}
	}

	bool ObjectManager::Add( ObjectPtr obj )
	{
		// Assign an unused ID if there is one.
		if ( mUnusedIDs.size() > 0 )
		{
			obj->mID = mUnusedIDs.back();
			mUnusedIDs.pop_back();
			mObjectList[obj->mID] = obj;
		}
		// Otherwise assign a new ID.
		else
		{
			mObjectList.push_back( obj );
			obj->mID = mObjectList.size() - 1;
		}

		// Check archetype
		if ( obj->mType != "" && mArcheTypeTable.find( obj->mType ) != mArcheTypeTable.end() )
		{
			// Check for missing component
			for ( const auto & i : mArcheTypeTable[obj->mType]->mComponents )
			{
				// If component not found
				if( !( obj->GetComponent( i->GetType().Name ) ) )
				{
					obj->AddComponent( i->GetType().Name );
				}
			}

			// Check for component that should not exist
			for ( const auto & i : obj->mComponents )
			{
				// If component not found
				if( !( mArcheTypeTable[obj->mType]->GetComponent( i->GetType().Name ) ) )
				{
					obj->RemoveComponent( i->GetType().Name );
				}
			}
		}

		AssignAvailableObjectName( obj->mName );

		obj->mObjectManager = this;
		mObjectNameTable.insert( std::make_pair( CName( obj->mName ), obj->GetID() ) );
		OnObjectCreate( obj );

		return true;
	}

	void ObjectManager::PreRename( ObjectPtr obj )
	{
		mObjectNameTable.erase( CName( obj->mName ) );
	}

	void ObjectManager::PostRename( ObjectPtr obj )
	{
		mObjectNameTable.insert( std::make_pair( CName( obj->mName ), obj->GetID() ) );
	}

	bool ObjectManager::AddArcheType( ObjectPtr obj )
	{
		// No ArcheType
		if ( obj->mType == "" )
			return false;

		// ArcheType already added
		if( mArcheTypeTable.find( obj->mType ) != mArcheTypeTable.end() )
			return false;

		mArcheTypeTable[obj->mType] = obj;
		obj->mName = obj->mType.GetText() + "ArcheType";
		obj->mObjectManager = this;
		return true;
	}

#if 0
	template<>
	ObjectPtr ObjectManager::LoadObjectFromFile<BinaryDeserialiser>(const PCL::Char* file, const PCL::Char* name, bool init)
	{
		std::ifstream fs;
		fs.open(file, std::ios_base::binary);

		if (!fs.is_open())
		{
			LOGERROR("Unable to open object file for reading: " << file);
			return NULL;
		}

		BinaryDeserialiser bd;
		bd.SetStream(fs);

		ObjectPtr newObj = nullptr;
		bool success = bd.DeserialisePtr(newObj);
		if (success)
		{
			if (name)
				newObj->mName = name;

			success = Add(newObj);

			if (!success)
			{
				ENTITY_MANAGER.Delete(newObj);
				newObj = nullptr;
			}
		}

		if (success)
		{
			LOGINFO("Object (" << newObj->GetName() << ") loaded from file: " << file);
		}
		else
		{
			LOGERROR("Unable to deserialise from binary file: " << file);
		}

		fs.close();

		if (init)
			InitObject(newObj);

		return newObj;
	}

	template<>
	void ObjectManager::SaveObjectToFile<BinarySerialiser>(const PCL::Char* file, ObjectPtr obj) const
	{
		if (!obj)
		{
			LOGERROR("No object to write to binary file: " << file);
			return;
		}

		std::ofstream fs;
		fs.open(file, std::ios_base::binary);

		if (!fs.is_open())
		{
			LOGERROR("Unable to open object file for writing: " << file);
			return;
		}

		BinarySerialiser bs;
		bs.SetStream(fs)
			.Serialise(*obj);

		LOGINFO("Object (" << obj->GetName() << ") saved to file: " << file);

		fs.close();
	}
#endif
}