#ifndef BH_TYPING_H
#define BH_TYPING_H

#include "Platform/Typedefs.h"
#include "Core/CName.h"
#include "Core/Hash.h"
#include "Core/Uncopyable.h"

// External Libraries
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <deque>

// Reflection system. References Don Williamson's Reflectabit library.
// https://bitbucket.org/dwilliamson/reflectabit

namespace BH
{
	namespace detail
	{
		// Unique type name for basic types.
		// Use DECLARE_TYPE to declare type name.
		template< typename TYPE >
		inline const Char * TypeName( const TYPE * ) {}		//!< Will cause compile error if type not instantiate

		//! Template class to check if a type is STL Container
		template< typename TYPE >
		struct IsSTLContainer{ static const bool Value = false; };

		//! Template specialised class to check if a type is STL Container
		template< typename TYPE >
		struct IsSTLContainer< std::vector<TYPE> > { static const bool Value = true; };
	}

	template< typename TYPE >
	inline const Char * TypeName() 
	{
		return detail::TypeName( reinterpret_cast<const TYPE*>(0) );
	}

	// Unique type hash generated from each type.
	template< typename TYPE >
	inline const HashValue TypeHash() 
	{
		static const HashValue hash = Hash::Generate( TypeName< std::remove_pointer<TYPE>::type >() );
		return hash;
	}

	// Get type name and hash.
	template< typename TYPE >
	inline const CName & TypeCName()
	{
		static const CName name = CName( TypeName< std::remove_pointer<TYPE>::type >(), TypeHash< std::remove_pointer<TYPE>::type >() );
		return name;
	}

	// Get type hash.
	template< typename TYPE >
	inline const Hash & TypeHashName() 
	{
		static const Hash hash = Hash( TypeName< std::remove_pointer<TYPE>::type >() );
		return hash;
	}

	namespace detail
	{
		// Generated type name for arrays.
		// Basic type must be defined using DECLARE_TYPE.
		template< typename TYPE, int LENGTH >
		inline const Char* TypeName(const TYPE(*)[LENGTH]) 
		{
			static const String name = String( TypeName( reinterpret_cast<const std::remove_pointer<TYPE>::type*>(0) ) ) + "Array";
			return name.c_str();
		}

		// Generated type name for std::vector.
		// Basic type must be defined using DECLARE_TYPE.
		template< typename TYPE >
		inline const Char* TypeName(const std::vector<TYPE>*) 
		{
			static const String name = String( TypeName( reinterpret_cast<const std::remove_pointer<TYPE>::type*>(0) ) ) + "Vector";
			return name.c_str();
		}

		// Generated type name for std::set.
		// Basic type must be defined using DECLARE_TYPE.
		template< typename TYPE >
		inline const Char* TypeName(const std::set<TYPE>*) 
		{
			static const String name = String( TypeName( reinterpret_cast<const std::remove_pointer<TYPE>::type*>(0) ) ) + "Set";
			return name.c_str();
		}

		// Generated type name for std::unordered_set.
		// Basic type must be defined using DECLARE_TYPE.
		template< typename TYPE >
		inline const Char* TypeName(const std::unordered_set<TYPE>*) 
		{
			static const String name = String( TypeName( reinterpret_cast<const std::remove_pointer<TYPE>::type*>(0) ) ) + "USet";
			return name.c_str();
		}

		// Generated type name for std::map.
		// Basic type must be defined using DECLARE_TYPE.
		template< typename KEY, typename VALUE >
		inline const Char* TypeName(const std::map<KEY, VALUE>*) 
		{
			static const String name = String( TypeName( reinterpret_cast<const std::remove_pointer<KEY>::type *>(0) ) ) + 
									   String( TypeName( reinterpret_cast<const std::remove_pointer<VALUE>::type *>(0) ) ) + 
									   "Map";
			return name.c_str();
		}

		// Generated type name for std::unordered_map.
		// Basic type must be defined using DECLARE_TYPE.
		template< typename KEY, typename VALUE >
		inline const Char* TypeName(const std::unordered_map<KEY, VALUE>*) 
		{
			static const String name = String( TypeName( reinterpret_cast<const std::remove_pointer<KEY>::type*>(0) ) ) + 
									   String( TypeName( reinterpret_cast<const std::remove_pointer<VALUE>::type*>(0) ) ) + 
									   "UMap";
			return name.c_str();
		}

		// Generated type name for std::deque.
		// Basic type must be defined using DECLARE_TYPE.
		template< typename TYPE >
		inline const Char* TypeName(const std::deque<TYPE>*) 
		{
			static const String name = String( TypeName( reinterpret_cast<const std::remove_pointer<TYPE>::type*>(0) ) ) + "Deque";
			return name.c_str();
		}
	}
}

// Declare new type name for reflection.
#define DECLARE_TYPE( type, name )\
	namespace BH\
	{\
		namespace detail\
		{\
			template<>\
			inline const Char* TypeName<type>(const type*) { return #name; }\
		}\
	}

#define MAKE_TYPE_FRIEND\
	friend class BH::Type; \
	friend class BH::EntityManager

#define DECLARE_REFLECTABLE\
	friend class BH::EntityManager; \
	friend class BH::TypeDB; \
	friend class BH::Type;

// Built in type
DECLARE_TYPE(BH::s8, Int8);
DECLARE_TYPE(BH::s16, Int16);
DECLARE_TYPE(BH::s32, Int32);
DECLARE_TYPE(BH::s64, Int64);
DECLARE_TYPE(BH::u8, Uint8);
DECLARE_TYPE(BH::u16, Uint16);
DECLARE_TYPE(BH::u32, Uint32);
DECLARE_TYPE(BH::u64, Uint64);
DECLARE_TYPE(BH::f32, Float32);
DECLARE_TYPE(BH::f64, Float64);
DECLARE_TYPE(BH::Char, Char);
DECLARE_TYPE(BH::String, String);
DECLARE_TYPE(bool, Boolean);

#include "Field.h"
#include "Type.h"
#include "TypeDB.h"

#endif // BH_TYPE_TRAITS_H