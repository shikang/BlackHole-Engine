// Type information database.
// https://bitbucket.org/dwilliamson/reflectabit

namespace BH
{
#if 0
	template < typename TYPE, bool OBJECT_COMPONENT >
	struct NewEntityImpl;

	template < typename TYPE >
	struct NewEntityImpl< TYPE, true >
	{
		static void * New()
		{
			TYPE * t = OBJECT_ALLOCATOR( TYPE ).New();
			t->mAddToSystem = &ObjectComponent::AddToAssignedSystem<TYPE>;
			t->mRemoveFromSystem = &ObjectComponent::RemoveFromAssignedSystem<TYPE>;
			return t;
		}
	};

	template < typename TYPE >
	struct NewEntityImpl< TYPE, false >
	{
		static void * New()
		{
			return OBJECT_ALLOCATOR( TYPE ).New();
		}
	};
#endif

	template< typename TYPE >
	void * TypeDB::NewEntity()
	{
		return OBJECT_ALLOCATOR( TYPE ).New();
	}

	template< typename TYPE >
	void TypeDB::DeleteEntity( void * entity )
	{
		OBJECT_ALLOCATOR(TYPE).Delete( entity );
	}

	template< typename TYPE >
	inline Type & TypeDB::Create()
	{
		const CName & name = TypeCName<TYPE>();
		TypeMap::iterator findit = mTypes.find( name );
		return ( findit == mTypes.end() ) ? *( CreateImpl<TYPE>() ) : *( findit->second );
	}

	template< typename TYPE >
	inline const Type * TypeDB::Get() const
	{
		const Type * type = Get( TypeCName<TYPE>() );
		return type ? type : const_cast<TypeDB *>( this )->CreateImpl<TYPE>();
	}

	inline const Type * TypeDB::Get( const CName & name ) const
	{
		TypeMap::const_iterator findit = mTypes.find( name );
		return ( findit != mTypes.end() ) ? findit->second : nullptr;
	}

	template< typename TYPE >
	inline Type * TypeDB::CreateImpl()
	{
		Type * type = new Type( TypeCName<TYPE>(), sizeof( TYPE ), NewEntity<TYPE>, DeleteEntity<TYPE>,
							  this, std::is_base_of<Entity, TYPE>::value );
		mTypes.insert( std::make_pair( TypeCName<TYPE>(), type ) );
		return type;
	}

	inline Type & TypeDB::Create( const CName & name )
	{
		TypeMap::const_iterator findit = mTypes.find( name );
		
		// Add if not available
		if ( findit == mTypes.end() )
		{
			Type * type = new Type( name, 0, Type::NewFunc(), Type::DeleteFunc(),
									this, false );
			mTypes.insert( std::make_pair( name, type ) );
		}

		return *mTypes[name];
	}

	inline void TypeDB::Remove( const CName & name )
	{
		TypeMap::const_iterator findit = mTypes.find( name );
		
		// Add if available
		if ( findit != mTypes.end() )
		{
			delete findit->second;
			mTypes.erase( findit );
		}
	}
}
