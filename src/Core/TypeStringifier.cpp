// Precompiled Headers
#include "Core/StdAfx.h"

#include "Core/TypeStringifier.h"

namespace BH
{
	TypeStringifier::TypeStringifier()
	{
	}

	TypeStringifier::~TypeStringifier()
	{
	}

	void TypeStringifier::ToStringEnum( String & str, const Type * type, const void * obj, const FieldAttributes * attr )
	{
		if ( attr && attr->Hex != 0 )
		{
			std::ostringstream o;
			o << std::hex << std::showbase << *reinterpret_cast<const u32 *>( obj );
			str = o.str();
			return;
		}

		if ( *reinterpret_cast<const u32 *>( obj ) == 0 )
		{
			str = "0";
			return;
		}

		if ( const EnumConst * e = type->GetEnum( *reinterpret_cast<const u32 *>( obj ) ) )
			str = e->Name.Text;
		else
			str = "";
	}

	void TypeStringifier::FromStringEnum( const String & str, const Type * type, void * obj, const FieldAttributes * attr )
	{
		if ( attr && attr->Hex != 0 )
		{
			std::istringstream i( str );
			i >> std::hex >> ( *reinterpret_cast<u32*>( obj ) );
			return;
		}

		if ( str == "0" )
		{
			*reinterpret_cast<u32*>( obj ) = 0;
			return;
		}

		if ( const EnumConst* e = type->GetEnum( CName( str.c_str() ) ) )
			*reinterpret_cast<u32*>( obj ) = e->Value;
	}

	template<>
	void TypeStringifier::FromStringDef<bool>( const String & str, const Type *, void * obj, const FieldAttributes * )
	{
		if ( str.length() > 0 )
		{
			if ( str.length() == 1 )
			{
				switch ( str[0] )
				{
				case '1':
					( *reinterpret_cast<bool*>( obj ) ) = true;
					break;
				case '0':
					( *reinterpret_cast<bool*>( obj ) ) = false;
					break;
				}
				return;
			}

			String tstr = str;
			std::transform( tstr.begin(), tstr.end(), tstr.begin(), tolower );	// In the case of using custom string

			if ( tstr == "true" )
				( *reinterpret_cast<bool*>( obj ) ) = true;

			else if ( tstr == "false" )
				( *reinterpret_cast<bool*>( obj ) ) = false;
		}
	}

	TypeStringifier::ToString TypeStringifier::GetToString( const Type * type )
	{
		ToStringMap::iterator it = mToStringMap.find( type->Name );
		if ( it != mToStringMap.end() )
			return it->second;

		return nullptr;
	}

	TypeStringifier::FmString TypeStringifier::GetFromString( const Type * type )
	{
		FmStringMap::iterator it = mFmStringMap.find( type->Name );
		if ( it != mFmStringMap.end() )
			return it->second;

		return nullptr;
	}
}