#ifndef BH_CNAME_H
#define BH_CNAME_H

#include "Core/CoreEssential.h"
#include "Core/Hash.h"

// Constant hashed name. Intended for use with constant compile-time strings.
// To prevent additional hash calculation

namespace BH
{
	// Basic constant name hash.
	struct CName
	{
		const Char * Text;	//!< Name text.
		HashValue	 Hash;	//!< Name hash.

		// Default constructor.
		inline CName() : Text(nullptr), Hash(0) {}

		// Constructor. Create constant name from constant string.
		CName(const Char * txt);

		// Constructor. Create constant name from constant string.
		explicit CName(const BH::Hash & hash);

		// Constructor.
		CName(const Char * txt, HashValue hash);

		/*
			Hash value constructor.
			WARNING: This only sets the hash value and it's to be used for name comparisons only.
		*/
		explicit CName(HashValue hash);

		// Assignment operator.
		CName & operator = (const Char * txt);

		// Assignment operator.
		CName & operator = (const BH::Hash & hash);

		// Equality operator.
		bool operator == (const CName & rhs) const;

		// Equality operator.
		bool operator < (const CName & rhs) const;

		// Conversion operator.
		operator BH::Hash() const;
	};

	// Outputs hash value and name to stream operator.
	inline Ostream & operator << (Ostream& stream, const CName& name) 
	{
		return (stream << name.Text);
	}
}

template<>
struct std::hash<BH::CName>
{
	inline std::size_t operator() (const BH::CName& name) const { return name.Hash; }
};

#include "CName.inl"

#endif // BH_CNAME_H
