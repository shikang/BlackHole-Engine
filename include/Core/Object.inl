namespace BH
{
	template<typename COMPONENT_TYPE>
	inline COMPONENT_TYPE * Object::AddComponent()
	{
		// Force a compile-time type test.
		static_cast<ObjectComponent *>( static_cast<COMPONENT_TYPE *>( nullptr ) );
		COMPONENT_TYPE * t = static_cast<COMPONENT_TYPE *>( AddComponent( TypeCName<COMPONENT_TYPE>() ) );

		if ( t )
		{
			t->mAddToSystem = &ObjectComponent::AddToAssignedSystem<COMPONENT_TYPE>;
			t->mRemoveFromSystem = &ObjectComponent::RemoveFromAssignedSystem<COMPONENT_TYPE>;
		}
		return t;
	}

	template<typename COMPONENT_TYPE>
	inline COMPONENT_TYPE * Object::GetComponent() const
	{
		// Force a compile-time type test.
		static_cast<ObjectComponent *>( static_cast<COMPONENT_TYPE *>( nullptr ) );
		return static_cast<COMPONENT_TYPE *>( GetComponent( TypeCName<COMPONENT_TYPE>() ) );
	}

	template<typename COMPONENT_TYPE>
	inline void Object::RemoveComponent()
	{
		RemoveComponent( TypeCName<COMPONENT_TYPE>() );
	}

	template<typename COMPONENT_TYPE>
	std::vector<COMPONENT_TYPE *> Object::GetAllComponent() const
	{
		// Force a compile-time type test.
		static_cast<ObjectComponent *>(static_cast<COMPONENT_TYPE *>(nullptr));

		std::vector<COMPONENT_TYPE *> components;

		for ( auto & i : mComponents )
		{
			if ( i->GetType().Name == TypeCName<COMPONENT_TYPE>() )
				components.push_back( static_cast<COMPONENT_TYPE *>( i ) );
		}

		return components;
	}
}