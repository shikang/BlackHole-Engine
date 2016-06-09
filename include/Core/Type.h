#ifndef BH_TYPE_H
#define BH_TYPE_H

#include "Platform/Typedefs.h"

#include "Core/CoreEssential.h"
#include "Core/Field.h"
#include "Core/EnumConst.h"
#include "Core/CName.h"
#include "Core/Assert.h"

// External Libraries
#include <vector>
#include <stack>

// Type information.
// https://bitbucket.org/dwilliamson/reflectabit

namespace BH
{
	// Forward Declarations
	class TypeDB;

	// Basic type representation for any game entity.
	class Type
	{
	public:
		// Typedefs
		//typedef void * ( * NewFunc )();					//!< New function pointer type.
		//typedef void ( * DeleteFunc )( void * );		//!< Delete function pointer type.

		typedef std::function< void *() > NewFunc;				//!< New function type.
		typedef std::function< void ( void * ) > DeleteFunc;	//!< Delete function type.
		typedef std::vector< Field >		Fields;				//!< Field list type.
		typedef std::vector< EnumConst >	Enums;				//!< Enum list type.

	public:
		// Get type fields.
		inline const Fields & GetFields() const { return mFields; }

		// Get type enums.
		inline const Enums & GetEnums() const { return mEnums; }

		// Get enum by value if it exists.
		const EnumConst * GetEnum( u32 value ) const;

		// Get enum by name if it exists.
		const EnumConst * GetEnum( const CName & name ) const;

		// Get specific field.
		const Field * GetField( const CName & name ) const;

	public:
		/*
			Declare new field to for type.
			e.g. Field( "X", &Vector3f::X );
		*/
		template< typename ENTITY_TYPE, typename FIELD_TYPE >
		Type & Field( const Char * name, FIELD_TYPE ENTITY_TYPE::*field, FieldAttributes attrs = FieldAttributes() );

		// Declare field for type in run time
		Type & Field( const Char * name, const Type * type, size_t offset, bool isPointer, std::shared_ptr<FieldContainer> container = nullptr, FieldAttributes attrs = FieldAttributes() );

		// Declare field for type in run time
		template< typename FIELD_TYPE >
		Type & Field( const Char * name, const Type * type, size_t offset, bool isPointer, FieldAttributes attrs = FieldAttributes() );

		/*
			Declare a base type.
			e.g. Base<ParentType>();
		*/
		template< typename BASE_TYPE >
		Type & Base();

		// Declare an enum constant for this type.
		Type & Enum( const Char * name, u32 value );

		// Comparison operator.
		bool operator == ( const Type & rhs ) const;

		// Less-than operator.
		bool operator < ( const Type & rhs ) const;

		// Check if type is child of specified parent type.
		template< typename PARENT >
		bool IsType() const;

		// Check if type is child of specified parent type.
		bool IsType(  const CName & parent ) const;

		// Does this type have any fields? Includes base type fields.
		bool HasFields() const;

		// Newly allocated object of this type.
		void * New() const;

		// Delete existing allocated object of this type.
		void Delete( void * obj ) const;

		// Get all fields. (Including all base type field, most base type on top of the stack)
		std::stack<const Fields *> GetAllFields() const;

	private:
		// Private constructor. Only TypeDB should be creating this.
		inline Type( const CName & name, size_t size, NewFunc nfunc, DeleteFunc dfunc,
					 TypeDB * typeDB, bool entType )
					: DB( typeDB ), Newer( nfunc ), Deleter( dfunc )
					, Size( size ), BaseType( nullptr ), IsEntity( entType ), Name( name ) {}

	public:
		TypeDB *				DB;				//!< Parent type database
		NewFunc					Newer;			//!< Pointers to the new function
		DeleteFunc				Deleter;		//!< Pointers to the delete function
		size_t					Size;			//!< Result of sizeof(type) operation
		const Type *			BaseType;		//!< Base type.
		bool					IsEntity;		//!< Is this type derived from Entity.
		CName					Name;			//!< Type name.

	private:
		Fields					mFields;		//!< List of fields.
		Enums					mEnums;			//!< List of enum constants.
		friend class TypeDB;					//!< Befriend type DB.

	};
}

#include "Type.inl"

#define FIELD( fieldName, className )\
	Field(#fieldName, &className::fieldName)

#define FIELDM( fieldName, className )\
	Field(#fieldName, &className::m##fieldName)

#define FIELD_ATTR( fieldName, className, attr )\
	Field(#fieldName, &className::fieldName, attr)

#define FIELDM_ATTR( fieldName, className, attr )\
	Field(#fieldName, &className::m##fieldName, attr)

#define FIELD_ENUM( enumName, enumType )\
	Enum(#enumName, enumType::enumName)

#endif // BH_TYPE_H