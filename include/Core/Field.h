#ifndef BH_FIELD_H
#define BH_FIELD_H

#include "Core/CoreEssential.h"
#include "Core/CName.h"
#include "Core/FieldContainer.h"

#include <memory>

namespace BH
{
#pragma warning( push )
// Disable the unnamed struct warnings.
#pragma warning( disable : 4201 )

	// Field attributes.
	class FieldAttributes
	{
	public:
		// Attribute Flag
		enum 
		{
			F_READONLY		= 1 << 0,
			F_TRANSIENT		= 1 << 1,
			F_INLINE		= 1 << 2,
			F_HEX			= 1 << 3,
			F_CHANGEREFRESH = 1 << 4,
		};

	public:
		// Constructor.
		FieldAttributes() : Group( nullptr ), Value(0) {}
		// Constructor.

		FieldAttributes( u32 val ) : Group( nullptr ), Value( val ) {}

		// Constructor.
		FieldAttributes( const Char* group, u32 val ) : Group( group ), Value( val ) {}

	public:
		const Char * Group;

		union 
		{
			// Bit field
			struct 
			{
				u32 ReadOnly		: 1;		//!< Read-only field.
				u32 Transient		: 1;		//!< Transient field (not serialisable).
				u32 Inline			: 1;		//!< Inline output field.
				u32 Hex				: 1;		//!< Hex value field.
				u32 ChangeRefresh	: 1;		//!< Indicates if editor needs to be refreshed when this value is set.
				u32 Unused			: 27;		//!< Unused flags.
			};

			u32 Value;							//! Field attribute value.
		};
	};
#pragma warning( pop ) 

	// Forward Declarations
	class Type;

	// Type field.
	class Field
	{
	public:
		/*
			Constructor.

			Template Params:
				ENTITY_TYPE	Member class type.
				FIELD_TYPE	Field type.
		*/
		template< typename ENTITY_TYPE, typename FIELD_TYPE >
		Field( const CName & name,					//!< Field name.
			   const BH::Type * type,				//!< Field type.
			   FIELD_TYPE ENTITY_TYPE::*field,		//!< Pointer to member field. [FIELD_TYPE x = class_ptr->*field; (Getting the field)]
			   const FieldAttributes & attrs,		//!< Field attributes.
			   const Char * group = nullptr,		//!< Field grouping.
			   const Char * desc = nullptr );		//!< Field description.

		// Constructor to create field on runtime
		Field( const CName & name,					//!< Field name.
			   const BH::Type * type,				//!< Field type.
			   size_t offset,						//!< Offset from main pointer.
			   bool isPointer,						//!< Is pointer.
			   const FieldAttributes & attrs,		//!< Field attributes.
			   std::shared_ptr<FieldContainer> container = nullptr,
			   const Char * group = nullptr,		//!< Field grouping.
			   const Char * desc = nullptr );		//!< Field description.

		template< typename FIELD_TYPE >
		Field( const CName & name,					//!< Field name.
			   const BH::Type * type,				//!< Field type.
			   size_t offset,						//!< Offset from main pointer.
			   bool isPointer,						//!< Is pointer.
			   FIELD_TYPE & f,
			   const FieldAttributes & attrs,		//!< Field attributes.
			   const Char * group = nullptr,		//!< Field grouping.
			   const Char * desc = nullptr);		//!< Field description

		// Get pointer to field data.
		inline void * GetPtr( const void * entity ) const { return ( Char * )entity + Offset; }

	public:
		const BH::Type *				Type;		//!< Field type.
		bool							IsPointer;	//!< Is field pointer?
		size_t							Offset;		//!< Field memory offset.
		std::shared_ptr<FieldContainer>	Container;	//!< Field container. nullptr if this is not a container.
		CName							Name;		//!< Field name.
		Hash							Group;		//!< Field grouping.
		Hash							Desc;		//!< Field description.
		mutable FieldAttributes			Attributes;	//!< Field attributes.

	};
}

#include "Field.inl"

#endif // BH_FIELD_H_
