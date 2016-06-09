#ifndef BH_FIELDCONTAINER_H
#define BH_FIELDCONTAINER_H

#include "Platform/Typedefs.h"

#include "Core/CoreEssential.h"
#include "Core/Uncopyable.h"

// https://bitbucket.org/dwilliamson/reflectabit
// Field Container Information

namespace BH
{
	/*
		Field container read iterator.
		Abstract class used to read container values.
	*/
	struct FieldReadIterator : Uncopyable
	{
	public:
		virtual ~FieldReadIterator() {}
		virtual const void * GetKey() const = 0;
		virtual const void * GetValue() const = 0;
		virtual bool IncNext() const = 0;
		virtual bool IsValid() const = 0;
		virtual u32 Count() const = 0;
	};

	/*
		Field container write iterator.
		Abstract class used to write container values.
	*/
	struct FieldWriteIterator : Uncopyable
	{
	public:
		virtual ~FieldWriteIterator() {}
		virtual bool Add(void * obj) = 0;
		virtual bool Add(void * key, void * obj) = 0;
		virtual void * AddEmpty() = 0;
		virtual void * AddEmpty(void * key) = 0;
	};

	// Field container information.
	class FieldContainer
	{
	public:
		typedef void( * InsertFunction )( void * container, void * key, void * val );
		typedef void( * RemoveFunction )( void * container, void * key, void * val );

	public:
		const Type *	KeyType;		//!< Container key type.
		const Type *	ValueType;		//!< Container value type.
		bool			IsKeyPointer;	//!< Is key pointer?
		bool			IsValuePointer;	//!< Is value pointer?

		InsertFunction	Insert;			//!< Function pointer to add element
		RemoveFunction	Remove;			//!< Function pointer to delete element

	public:
		// Constructor.
		FieldContainer()
			: KeyType( nullptr ), ValueType( nullptr )
			, IsKeyPointer( false ), IsValuePointer( false )
			, Insert( nullptr ), Remove( nullptr ){}

		// Get read iterator allocation size.
		virtual u32 GetReadItAllocSize() const = 0;

		// Get write iterator allocation size.
		virtual u32 GetWriteItAllocSize() const = 0;

		// Allocate a new read iterator.
		virtual FieldReadIterator * CreateReadIt( void * dest, const void * container ) const = 0;

		// Allocate a new write iterator.
		virtual FieldWriteIterator * CreateWriteIt( void * dest, const void * container ) const = 0;

		// Deallocate an existing field read iterator.
		virtual void DestructIt( FieldReadIterator * it ) = 0;

		// Deallocate an existing field write iterator.
		virtual void DestructIt( FieldWriteIterator * it ) = 0;
	};

	namespace detail
	{
		// Wrapper class used to automatically determine container types.
		template< typename TYPE, typename READ_IT, typename WRITE_IT >
		class FieldContainer_Impl :
			public FieldContainer
		{
		public:
			u32 GetReadItAllocSize() const { return sizeof( READ_IT ); }
			u32 GetWriteItAllocSize() const { return sizeof( WRITE_IT ); }
			FieldReadIterator * CreateReadIt( void * dest, const void* container ) const { return new ( dest ) READ_IT( static_cast<const TYPE *>( container ) ); }		// Placement new
			FieldWriteIterator * CreateWriteIt( void * dest, const void* container ) const { return new ( dest )WRITE_IT( ( TYPE * )container ); }
			void DestructIt( FieldReadIterator * it ) { static_cast<READ_IT *>( it )->READ_IT::~READ_IT(); }															// Call destructor since this memory is return from placement new
			void DestructIt( FieldWriteIterator * it ) { static_cast<WRITE_IT *>( it )->WRITE_IT::~WRITE_IT(); }

		};

		// No container factory is created by default for all field types
		template< typename TYPE >
		inline FieldContainer * CreateContainer(TYPE &)
		{
			return nullptr;
		}
	}
}

/*
	_alloca create memory from stack, automatically free memory. 
	Risk of stack overflow, but it's doesn't matter as declaring 
	huge array size would cause the same problem.
*/

// Create a new read iterator on the stack.
// Use DEL_FIELD_IT to delete.
#define NEW_FIELD_READIT( field_cont, container )\
	field_cont->CreateReadIt(_alloca(field_cont->GetReadItAllocSize()), container)

// Create a new write iterator on the stack.
// Use DEL_FIELD_IT to delete.
#define NEW_FIELD_WRITEIT( field_cont, container )\
	field_cont->CreateWriteIt(_alloca(field_cont->GetWriteItAllocSize()), container)

// Delete an existing field iterator.
#define DEL_FIELD_IT( field_cont, it )\
	field_cont->DestructIt(it)

// Create a loop for reading field container values.
#define FOREACH_READ_FIELD_BEGIN( field_cont, it_name, container )\
	{\
		BH::FieldReadIterator* it_name = NEW_FIELD_READIT(field_cont, container); \
		for (; it_name->IsValid(); it_name->IncNext())

// End for loop.
#define FOREACH_FIELD_END( field_cont, it_name )\
		DEL_FIELD_IT(field_cont, it_name); \
	}

#endif // BH_FIELDCONTAINER_H
