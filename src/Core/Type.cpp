// Precompiled Headers
#include "Core/StdAfx.h"

#include "Core/Type.h"
#include "Core/TypeStringifier.h"

namespace BH
{
	const EnumConst * Type::GetEnum( u32 value ) const
	{
		for ( size_t i = 0; i < mEnums.size(); ++i )
		{
			if ( mEnums[i].Value == value )
				return &mEnums[i];
		}

		return nullptr;
	}

	const EnumConst * Type::GetEnum( const CName & name ) const
	{
		for ( size_t i = 0; i < mEnums.size(); ++i )
		{
			if ( mEnums[i].Name == name )
				return &mEnums[i];
		}

		return nullptr;
	}

	const Field* Type::GetField( const CName & name ) const
	{
		for ( size_t i = 0; i < mFields.size(); ++i )
		{
			if ( mFields[i].Name == name )
				return &mFields[i];
		}

		return nullptr;
	}

	Type& Type::Enum( const Char * name, u32 value )
	{
		mEnums.push_back( EnumConst( name, value ) );

		TYPE_STRINGIFIER.AddTypeStringifier( this, &TypeStringifier::ToStringEnum, &TypeStringifier::FromStringEnum );
		return *this;
	}

	bool Type::HasFields() const
	{
		const Type* type = this;
		while ( type )
		{
			if ( type->mFields.size() > 0 )
				return true;

			type = type->BaseType;
		}

		return false;
	}

	std::stack<const Type::Fields *> Type::GetAllFields() const
	{
		std::stack<const Type::Fields *> fieldStack;
		const BH::Type* type = this;

		while ( type )
		{
			if ( type->GetFields().size() > 0 )
				fieldStack.push( &( type->GetFields() ) );

			type = type->BaseType;
		}

		return fieldStack;
	}
}