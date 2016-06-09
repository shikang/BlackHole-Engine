// Precompiled Header
#include "Core/Stdafx.h"

#include "Core/ObjectComponent.h"

namespace BH
{
	ObjectComponent::ObjectComponent()
	: mObject(nullptr)
	, mObjectID( UINT_MAX )
	, mSystem( nullptr )
	, mAddToSystem( &ObjectComponent::AddToAssignedSystem<ObjectComponent> )
	, mRemoveFromSystem( &ObjectComponent::RemoveFromAssignedSystem<ObjectComponent> )
	{
	}

	ObjectComponent::~ObjectComponent()
	{
	}

	void ObjectComponent::Initialise()
	{
	}

	void ObjectComponent::Shutdown() 
	{
	}

	void ObjectComponent::OnObjectRenamed(const CName&, const CName&)
	{
	}

	Entity::OPTIONS ObjectComponent::OnPreChanged() 
	{
		return Entity::O_NORMAL;
	}

	void ObjectComponent::OnChanged() 
	{
	}

	Entity::OPTIONS ObjectComponent::OnPreSaved() const
	{
		return Entity::O_NORMAL;
	}

	void ObjectComponent::OnSaved() const
	{
	}
}