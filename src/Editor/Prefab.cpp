#include "Platform/Platform.h"

#include "Core/CoreEssential.h"
#include "Core/Hash.h"
#include "Core/Game.h"

#include "Editor/Prefab.h"

namespace BH
{
	void Prefab::Initialise( const String & name, Object * object ) 
	{
		Name = name;
		mOriginalName = name;
		Original = object;
		OriginalComponents = static_cast<Object::ComponentList *>( ENTITY_MANAGER.Types.Get<Object>()->GetField( "Components" )->GetPtr( object ) );
		Components = *OriginalComponents;
	}

	Entity::OPTIONS Prefab::OnPreChanged()
	{
		if ( Original )
			Object::ChangingObject = Original;

		return Entity::O_NORMAL;
	}

	void Prefab::OnChanged()
	{
		// Rename archetype
		if ( Original && OriginalComponents )
		{
			*OriginalComponents = Components;

			Original->ReassignComponent();

			if ( mOriginalName != Name )
				GAME.ObjectManager.RenameArcheType( mOriginalName, Name );

			Object::ChangingObject = nullptr;
		}
	}

	Entity::OPTIONS Prefab::OnPreSaved() const
	{
		// Nothing
		return Entity::O_NORMAL;
	}

	void Prefab::OnSaved() const
	{

	}
}