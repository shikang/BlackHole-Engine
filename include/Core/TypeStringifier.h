#ifndef BH_TYPE_STRINGIFIER_H
#define BH_TYPE_STRINGIFIER_H

#include "Core/CoreEssential.h"
#include "Core/Hash.h"
#include "Core/CName.h"
#include "Core/Type.h"
#include "Core/Typing.h"
#include "Core/TypeDB.h"
#include "Core/EntityManager.h"
#include "Core/Singleton.h"
#include "Core/Uncopyable.h"

#include <unordered_map>
#include <sstream>
#include <type_traits>

namespace BH
{
	class TypeStringifier : private Uncopyable
	{
	public:
		typedef void( *ToString )( String &, const Type *, const void *, const FieldAttributes * );	//!< Type to string function pointer type.
		typedef void( *FmString )( const String &, const Type *, void *, const FieldAttributes * );	//!< String to type function pointer type.
		typedef std::unordered_map<CName, ToString> ToStringMap;
		typedef std::unordered_map<CName, FmString> FmStringMap;

		DECLARE_SINGLETON( TypeStringifier );

	public:
		// Destructor
		~TypeStringifier();

		// Add Type Stringifier
		void AddTypeStringifier( const Type * type, ToString tostr, FmString fmstr );

		// Add default Stringifier for type
		template < typename TYPE >
		void AddDefaultStringifier();

		template < typename TYPE >
		void AddDefaultStringifier( TypeDB & typedb );

		// Get to string function
		ToString GetToString( const Type * type );

		// Get from string function
		FmString GetFromString( const Type * type );

	private:
		// Constructor
		TypeStringifier();

	public:
		// To String, From String implementation
		template< typename INTEGRAL_TYPE >
		static void ToStringDef( String & str, const Type * type, const void * obj, const FieldAttributes * attr );

		template< typename INTEGRAL_TYPE >
		static void FromStringDef( const String & str, const Type * type, void * obj, const FieldAttributes * attr );

		static void ToStringEnum( String & str, const Type * type, const void * obj, const FieldAttributes * attr );

		static void FromStringEnum( const String & str, const Type * type, void * obj, const FieldAttributes * attr );

	private:
		ToStringMap mToStringMap;
		FmStringMap mFmStringMap;

	};

	template<>
	void TypeStringifier::FromStringDef<bool>( const String & stream, const Type *, void * obj, const FieldAttributes * );
}

#include "Core/TypeStringifier.inl"

#define TYPE_STRINGIFIER\
	::BH::TypeStringifier::Instance()

#endif