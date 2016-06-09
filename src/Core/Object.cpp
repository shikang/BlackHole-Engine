// Precompiled Header
#include "Core/Stdafx.h"

#include "Core/Object.h"
#include "Core/ObjectComponent.h"

#define ALLOW_UNIQUE 0

namespace BH
{
	Object * Object::ChangingObject = nullptr;

	Object::Object()
	: mID( UINT_MAX )
	, mObjectManager( nullptr )
	, mInitialised(false)
	, mType("")
	{

	}

	Object::~Object()
	{
		// Shutdown in reverse
		u32 size = mComponents.size();
		for (u32 i = size; i > 0; --i)
		{
			if ( mInitialised )
			{
				mComponents[i - 1]->mRemoveFromSystem( mComponents[i - 1] );
				mComponents[i - 1]->Shutdown();
			}

			ENTITY_MANAGER.Delete( mComponents[i - 1] );
		}
	}

	void Object::Initialise()
	{
		for ( auto & i : mComponents )
		{
			i->mObject = this;
			i->mObjectID = mID;
			i->Initialise();

			// Add component to system
			i->mAddToSystem( i );
		}

		mInitialised = true;
	}

	void Object::Shutdown()
	{
		// Shutdown in reverse
		u32 size = mComponents.size();
		for ( u32 i = size; i > 0; --i )
		{
			// Remove component from system
			mComponents[i - 1]->mRemoveFromSystem( mComponents[i - 1] );

			mComponents[i - 1]->Shutdown();
		}

		mInitialised = false;
	}

	Entity::OPTIONS Object::OnPreChanged()
	{
		if ( mObjectManager ) 
			mObjectManager->PreRename( this );

		ChangingObject = this;

		return Entity::O_NORMAL;
	}

	void Object::OnChanged()
	{
		for ( auto & i : mComponents )
		{
			// Means that component added through editor
			if ( i->mObjectID != mID )
			{
				i->mObject = this;
				i->mObjectID = mID;

				if ( mInitialised )
				{
					i->Initialise();
					i->mAddToSystem( i );
				}
			}
		}

		if ( mObjectManager ) 
			mObjectManager->PostRename( this );

		ChangingObject = nullptr;
	}

	Entity::OPTIONS Object::OnPreSaved() const
	{
		return Entity::O_NORMAL;
	}

	void Object::OnSaved() const
	{
	}

	ObjectComponent * Object::AddComponent( const CName & type )
	{
#if ALLOW_UNIQUE
		ObjectComponent * component = GetComponent( type );
		bool added = component != nullptr;

		if ( !component )
			component = static_cast<ObjectComponent *>( ENTITY_MANAGER.New( type ) );
#else
		ObjectComponent * component = static_cast<ObjectComponent *>( ENTITY_MANAGER.New( type ) );
#endif

		if ( !component )
		{
			LOGERROR( "Unable to create object component: " << type.Text << ". Type not registered." );
			return nullptr;
		}

		component->mObject = this;
		component->mObjectID = mID;

#if ALLOW_UNIQUE
		if ( !added )
#endif
			mComponents.push_back( component );

		if ( mInitialised && component )
		{
			component->Initialise();
			component->mAddToSystem( component );
		}

		return component;
	}

	ObjectComponent * Object::GetComponent( const CName & type ) const
	{
		for ( auto & i : mComponents )
		{
			if ( i->GetType().Name == type )
				return i;
		}

		return nullptr;
	}

	std::vector<ObjectComponent *> Object::GetAllComponent( const CName & type ) const
	{
		std::vector<ObjectComponent *> components;

		for ( auto & i : mComponents )
		{
			if ( i->GetType().Name == type )
				components.push_back( i );
		}

		return components;
	}

	void Object::RemoveComponent( const CName & type )
	{
		ComponentList::iterator end = mComponents.end();
		for ( ComponentList::iterator it = mComponents.begin(); it != end; ++it )
		{
			if ( ( *it )->GetType().Name == type )
			{
				( *it )->mRemoveFromSystem( *it );
				( *it )->Shutdown();
				ENTITY_MANAGER.Delete( *it );
				mComponents.erase( it );
				return;
			}
		}

		LOGERROR(type << " does not exist in Object ID:" << mID << ".");
	}

	void Object::RemoveComponent( ObjectComponent * component )
	{
		ComponentList::iterator it = std::find( mComponents.begin(), mComponents.end(), component );
		if ( it != mComponents.end() )
		{
			( *it )->mRemoveFromSystem( *it );
			( *it )->Shutdown();
			ENTITY_MANAGER.Delete( *it );
			mComponents.erase( it );
		}
	}

	void Object::ReassignComponent()
	{
		for ( auto & i : mComponents )
		{
			i->mObject = this;
		}
	}
}