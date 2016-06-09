// Type Information
namespace BH
{
	template< typename ENTITY_TYPE, typename FIELD_TYPE >
	inline Type & Type::Field( const Char * name, FIELD_TYPE ENTITY_TYPE::*field, FieldAttributes attrs )
	{
		mFields.push_back( BH::Field( CName( name ), DB->Get< std::remove_pointer<FIELD_TYPE>::type >(), field, attrs ) );
		return *this;
	}

	inline Type & Type::Field( const Char * name, const Type * type, size_t offset, bool isPointer, std::shared_ptr<FieldContainer> container, FieldAttributes attrs )
	{
		mFields.push_back( BH::Field( CName( name ), type, offset, isPointer, attrs, container ) );
		return *this;
	}

	template< typename FIELD_TYPE >
	inline Type & Type::Field( const Char * name, const Type * type, size_t offset, bool isPointer, FieldAttributes attrs )
	{
		FIELD_TYPE f;
		mFields.push_back( BH::Field( CName( name ), type, offset, isPointer, f, attrs ) );
		return *this;
	}

	template< typename BASE_TYPE >
	inline Type & Type::Base()
	{
		BaseType = DB->Get< std::remove_pointer<BASE_TYPE>::type >();
		return *this;
	}

	inline bool Type::operator == ( const Type& rhs ) const
	{
		return Name == rhs.Name;
	}

	inline bool Type::operator < ( const Type& rhs ) const
	{
		return Name < rhs.Name;
	}

	template< typename PARENT >
	inline bool Type::IsType() const
	{
		return IsType( TypeCName<PARENT>() );
	}

	// Check if type is child of specified parent type.
	inline bool Type::IsType( const CName & parent ) const
	{
		if ( parent == Name ) 
			return true;

		const Type * p = BaseType;
		while ( p ) 
		{
			if ( p->Name == parent ) 
				return true;

			p = p->BaseType;
		}

		return false;
	}

	inline void * Type::New() const
	{
		BH_ASSERT( Size > 0, "Type not valid" );
		void * obj = Newer();
		return obj;
	}

	inline void Type::Delete( void* obj ) const
	{
		Deleter( obj );
	}
}