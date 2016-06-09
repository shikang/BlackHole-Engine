namespace BH
{
	template< typename INTEGRAL_TYPE >
	inline void TypeStringifier::ToStringDef( String & str, const Type *, const void * obj, const FieldAttributes * attr )
	{
		std::ostringstream o;
		if ( attr && attr->Hex != 0 )
			o << std::hex << std::showbase;

		o << *reinterpret_cast<const INTEGRAL_TYPE *>( obj );
		str = o.str();
	}

	template< typename INTEGRAL_TYPE >
	inline void TypeStringifier::FromStringDef( const String & stream, const Type *, void * obj, const FieldAttributes * attr )
	{
		std::istringstream i( stream );
		if ( attr && attr->Hex != 0 )
			i >> std::hex;

		i >> *reinterpret_cast<INTEGRAL_TYPE *>( obj );
	}

	template<>
	inline void TypeStringifier::ToStringDef<s8>( String & str, const Type *, const void * obj, const FieldAttributes * attr )
	{
		std::ostringstream o;
		if ( attr && attr->Hex != 0 )
			o << std::hex << std::showbase;

		o << s32( *reinterpret_cast<const s8 *>( obj ) );
		str = o.str();
	}

	template<>
	inline void TypeStringifier::FromStringDef<s8>( const String & stream, const Type *, void * obj, const FieldAttributes * attr )
	{
		s32 t;
		std::istringstream i( stream );

		if ( attr && attr->Hex != 0 )
			i >> std::hex;

		i >> t;
		*reinterpret_cast<s8 *>( obj ) = s8( t );
	}

	template<>
	inline void TypeStringifier::ToStringDef<bool>( String & str, const Type *, const void * obj, const FieldAttributes * )
	{
		str = ( *reinterpret_cast<const bool *>( obj ) ) ? "true" : "false";
	}

	template<>
	inline void TypeStringifier::ToStringDef<String>( String & str, const Type *, const void * obj, const FieldAttributes * )
	{
		str = *static_cast<const String *>( obj );
	}

	template<>
	inline void TypeStringifier::FromStringDef<String>( const String & stream, const Type *, void * obj, const FieldAttributes * )
	{
		*static_cast<String *>( obj ) = stream;
	}

	template< typename TYPE >
	inline void TypeStringifier::AddDefaultStringifier()
	{
		//const Type * type = ENTITY_MANAGER.Types.Get<TYPE>();
		const Type * type = TypeDB::Instance().Get<TYPE>();

		if ( mToStringMap.find( type->Name ) == mToStringMap.end() )
			mToStringMap.insert( std::make_pair( type->Name, &TypeStringifier::ToStringDef< std::remove_pointer<TYPE>::type > ) );

		if ( mFmStringMap.find( type->Name ) == mFmStringMap.end() )
			mFmStringMap.insert( std::make_pair( type->Name, &TypeStringifier::FromStringDef< std::remove_pointer<TYPE>::type > ) );
	}

	template< typename TYPE >
	inline void TypeStringifier::AddDefaultStringifier( TypeDB & typedb )
	{
		//const Type * type = ENTITY_MANAGER.Types.Get<TYPE>();
		const Type * type = typedb.Get<TYPE>();

		if ( mToStringMap.find( type->Name ) == mToStringMap.end() )
			mToStringMap.insert( std::make_pair( type->Name, &TypeStringifier::ToStringDef< std::remove_pointer<TYPE>::type > ) );

		if ( mFmStringMap.find( type->Name ) == mFmStringMap.end() )
			mFmStringMap.insert( std::make_pair( type->Name, &TypeStringifier::FromStringDef< std::remove_pointer<TYPE>::type > ) );
	}

	inline void TypeStringifier::AddTypeStringifier( const Type * type, ToString tostr, FmString fmstr )
	{
		if ( !type )
			return;

		if ( mToStringMap.find( type->Name ) == mToStringMap.end() )
			mToStringMap.insert( std::make_pair( type->Name, tostr ) );

		if ( mFmStringMap.find( type->Name ) == mFmStringMap.end() )
			mFmStringMap.insert( std::make_pair( type->Name, fmstr ) );
	}
}