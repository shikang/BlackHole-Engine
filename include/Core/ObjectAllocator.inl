namespace BH
{
	template< typename T >
	ObjectAllocator<T>::ObjectAllocator()
	: mBlock( nullptr )
	, mMaxElement( 1024 )
	, mAllocateElements( 0 )
	{
	}

	template< typename T >
	ObjectAllocator<T>::~ObjectAllocator()
	{
		if ( mBlock )
			mAllocator.deallocate( mBlock, mAllocateElements );
	}

	// Allocate and construct 1 Object
	template< typename T >
	template< typename... Arguments >
	T * ObjectAllocator<T>::New( Arguments&&... args )
	{
		// Have not allocate pre-made chunck
		if ( !mBlock )
			PrepareBlock();

		// Get pointer from freelist and construct it
		T * obj = mBlock + mFreeList.top();
		mFreeList.pop();
		mAllocator.construct( obj, std::forward< Arguments >( args )... );
		return obj;
	}

	// Destruct and deallocate 1 Object
	template< typename T >
	void ObjectAllocator<T>::Delete( void * obj )
	{
		// Destruct obj and return it to freelist
		mAllocator.destroy( reinterpret_cast<T *>( obj ) );
		mFreeList.push( reinterpret_cast<T *>( obj ) - mBlock );
	}

	// Preallocate a block of memory
	template< typename T >
	void ObjectAllocator<T>::PrepareBlock()
	{
		mAllocateElements = mMaxElement;
		mBlock = mAllocator.allocate( mAllocateElements );

		/*
		std::generate_n( std::back_inserter( mFreeList ), mAllocateElements,
						 []()->u32
						 {
							 static u32 num = 0;
							 return num++;
						 } );
		*/

		// Because priority queue and stack could not use inserter (push, not push_back)
		for ( u32 i = 0; i < mAllocateElements; ++i )
			mFreeList.push( i );
	}
}