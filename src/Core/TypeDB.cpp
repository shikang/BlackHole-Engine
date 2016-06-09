// Precompiled Headers
#include "Core/StdAfx.h"

#include "Platform/Typedefs.h"

#include "Core/TypeDB.h"

// Type information database.
// https://bitbucket.org/dwilliamson/reflectabit

namespace BH
{
	TypeDB::TypeDB()
	{
		//Create<s8>().DefaultSerialisers<s8>();
		//Create<s16>().DefaultSerialisers<s16>();
		//Create<s32>().DefaultSerialisers<s32>();
		//Create<s64>().DefaultSerialisers<s64>();
		//Create<u8>().DefaultSerialisers<u8>();
		//Create<u16>().DefaultSerialisers<u16>();
		//Create<u32>().DefaultSerialisers<u32>();
		//Create<u64>().DefaultSerialisers<u64>();
		//Create<f32>().DefaultSerialisers<f32>();
		//Create<f64>().DefaultSerialisers<f64>();
		//Create<Char>().DefaultSerialisers<Char>();
		//Create<Uchar>().DefaultSerialisers<Uchar>();
		//Create<String>().DefaultSerialisers<String>();
		//Create<bool>().DefaultSerialisers<bool>();

		Create<s8>();
		Create<s16>();
		Create<s32>();
		Create<s64>();
		Create<u8>();
		Create<u16>();
		Create<u32>();
		Create<u64>();
		Create<f32>();
		Create<f64>();
		Create<Char>();
		Create<Uchar>();
		Create<String>();
		Create<bool>();

		TYPE_STRINGIFIER.AddDefaultStringifier<s8>( *this );
		TYPE_STRINGIFIER.AddDefaultStringifier<s16>( *this );
		TYPE_STRINGIFIER.AddDefaultStringifier<s32>( *this );
		TYPE_STRINGIFIER.AddDefaultStringifier<s64>( *this );
		TYPE_STRINGIFIER.AddDefaultStringifier<u8>( *this );
		TYPE_STRINGIFIER.AddDefaultStringifier<u16>( *this );
		TYPE_STRINGIFIER.AddDefaultStringifier<u32>( *this );
		TYPE_STRINGIFIER.AddDefaultStringifier<u64>( *this );
		TYPE_STRINGIFIER.AddDefaultStringifier<f32>( *this );
		TYPE_STRINGIFIER.AddDefaultStringifier<f64>( *this );
		TYPE_STRINGIFIER.AddDefaultStringifier<Char>( *this );
		TYPE_STRINGIFIER.AddDefaultStringifier<Uchar>( *this );
		TYPE_STRINGIFIER.AddDefaultStringifier<String>( *this );
		TYPE_STRINGIFIER.AddDefaultStringifier<bool>( *this );
	}

	TypeDB::~TypeDB()
	{
		for ( auto & it : mTypes )
			delete( it.second );

		mTypes.clear();
	}

	void TypeDB::PrintAllTypes() const
	{
		for ( const auto & it : mTypes )
		{
			std::cout << "Type: " << it.second->Name << std::endl;
			std::stack<const Type::Fields *> fields = it.second->GetAllFields();

			while ( !fields.empty() )
			{
				const Type::Fields * f = fields.top();
				fields.pop();
				for ( const auto & field : *f )
				{
					std::cout << "\tField: " << field.Name << std::endl;
				}
			}
		}
	}
}