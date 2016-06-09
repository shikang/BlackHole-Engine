#ifndef BH_OBJECT_ALLOCATOR_H
#define BH_OBJECT_ALLOCATOR_H

#include "Core/CoreEssential.h"
#include "Core/FunctionsFramework.h"
#include "Core/Singleton.h"

#include <queue>
#include <algorithm>
#include <iterator>

namespace BH
{
	// Wrapper singleton class for allocator
	template< typename T >
	class ObjectAllocator
	{
	public:
		// Declare as singleton.
		DECLARE_SINGLETON( ObjectAllocator<T> );

	public:
		GETSET_AUTO( u32, MaxElement );		//!< Get Max element

	public:
		// Destructor
		~ObjectAllocator();

		// Allocate and construct 1 Object
		template< typename... Arguments >
		T * New( Arguments&&... args );

		// Destruct and deallocate 1 Object
		void Delete( void * obj );

	private:
		// Constructor
		ObjectAllocator();

		// Preallocate a block of memory
		void PrepareBlock();

	private:
		// Freelist container
		typedef std::priority_queue< u32, std::vector<u32>, std::greater<u32> > FreeList;

	private:
		std::allocator<T> mAllocator;		//!< Use std::allocator for now
		FreeList mFreeList;					//!< Which element is free (priority queue to ensure index around will always get chosen)
		T * mBlock;							//!< Block of memory allocated
		u32 mMaxElement;					//!< Max number of object that can be allocated
		u32 mAllocateElements;				//!< NUmber of elements allocated

	};
}

#define OBJECT_ALLOCATOR(type)\
	::BH::ObjectAllocator<type>::Instance()

#include "ObjectAllocator.inl"

#endif