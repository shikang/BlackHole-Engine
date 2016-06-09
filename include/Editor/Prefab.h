#ifndef BH_PREFAB_H
#define BH_PREFAB_H

#include "Core/CoreEssential.h"
#include "Core/TypeCreator.h"
#include "Core/ObjectComponent.h"
#include "Core/Object.h"
#include "Core/Entity.h"

namespace BH
{
	class Prefab : public Entity
	{
	public:
		// Destructor
		~Prefab() {}

		// Constructors
		Prefab() : Name(), OriginalComponents(nullptr), Original( nullptr ) {}

		// Intialise prefab
		void Initialise( const String & name, Object * object );

		// Called just before the internal representation of the entity changes. (e.g. Before deserialisation)
		OPTIONS OnPreChanged() override;

		// Called when the internal representation of the entity changes. (e.g. After deserialisation)
		void OnChanged() override;

		// Called when the entity is about to be saved. (e.g. Before serialisation)
		OPTIONS OnPreSaved() const override;

		// Called after the entity is saved. (e.g. After serialisation)
		void OnSaved() const override;

	public:
		String Name;									//!< Prefab name
		Object::ComponentList Components;				//!< Component list
		Object::ComponentList * OriginalComponents;		//!< Component list
		Object * Original;								//!< Original object

	private:
		String mOriginalName;							//!< Prefab original name

	};
}

DECLARE_TYPE( ::BH::Prefab, Prefab );

#endif