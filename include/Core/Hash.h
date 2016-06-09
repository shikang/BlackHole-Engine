#ifndef BH_HASH_H
#define BH_HASH_H

#include "Platform/Platform.h"

#include "Core/CoreEssential.h"
#include "Core/FunctionsFramework.h"

namespace BH
{
	// Use std::hash for now
	typedef std::hash<String>				HashFunctor;
	typedef HashFunctor::result_type		HashValue;
	
	/*
		The object hashes a string into an integer value and stores both the string
		and the hash value. Hash objects are compared using their hash values for
		fast hash comparisons.
	*/
	class Hash
	{
	public:
		GETTER_AUTO_CREF(String, Text);		//!< Gets the hash text.
		GETTER_AUTO(HashValue, Hash);		//!< Gets the hash value.

	public:
		static HashValue Generate(const Char* string);
		static HashValue Generate(const String& string);

		// Used for serialisation.
		static void ToString(String & str, const Type *, const void * obj, const FieldAttributes * attr);
		// Used for deserialisation.
		static void FromString(const String & str, const Type *, void * obj, const FieldAttributes * attr);

	public:
		// Default constructor.
		Hash();

		// Destructor.
		~Hash();

		// Constructor.
		Hash(const Char * name);

		// Constructor.
		Hash(const String& name);

		// Assignment operator for strings.
		Hash & operator = (const Char* name);

		// Assignment operator for strings.
		Hash & operator = (const String& name);

		// Equality operator.
		bool operator == (const Hash & rhs) const;

		// Equality operator.
		bool operator == (const HashValue & rhs) const;

		// Inequality operator.
		bool operator != (const Hash & rhs) const;

		// Less than operator.
		bool operator < (const Hash & rhs) const;

		// More than operator.
		bool operator > (const Hash & rhs) const;

	private:
		String				mText;			//!< Hash name.
		HashValue			mHash;			//!< Hash value.
		friend class		Type;
		friend class		EntityManager;
	};

	//! Outputs hash value and name to stream operator.
	inline Ostream & operator << (Ostream & stream, const Hash & hash) 
	{
		return stream << hash.GetHash();
	}
}

#include "Core/Typing.h"
DECLARE_TYPE(::BH::Hash, Hash);

// For unordered_map comparison. (They hash every value and check for it)
template<>
struct std::hash<BH::Hash> 
{
	inline std::size_t operator() (const BH::Hash& hash) const { return hash.GetHash(); }
};

#include "Hash.inl"

#endif // BH_HASH_H
