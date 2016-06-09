#ifndef BH_ENUMCONST_H
#define BH_ENUMCONST_H

#include "Platform/Typedefs.h"

#include "Core/CoreEssential.h"
#include "Core/CName.h"
#include "Core/Uncopyable.h"

// Type Information
namespace BH
{
	// Enum constant for Type.
	class EnumConst
	{
	public:
		// Constructor.
		EnumConst(const CName & name, u32 value)
			: Name(name), Value(value) {}

	public:
		CName		Name;	//!< Enum name.
		u32			Value;	//!< Enum value.
	};
}

#endif // BH_ENUMCONST_H