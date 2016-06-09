namespace BH
{

	template <typename T>
	Handle<T> HandleManager::Acquire( T * ptr )
	{
		BH_ASSERT( mFreeTable.size( ) != 0, "Cannot acquire, all handles in use." );

		Handle<u32>::IndexType idx = mFreeTable.front( );
		mFreeTable.pop( );

		mDataTable[ idx ] = ptr;

		++mNumHandles;

		return Handle<T>( idx, mVersionTable[ idx ] );
	}

	template <typename T>
	void HandleManager::Release( const Handle<T> & handle )
	{
		BH_ASSERT( CheckHandleValid( handle ) == true, "Cannot release, handle invalid." );

		mVersionTable[ handle.mIdx ] += 1;
		mFreeTable.push( handle.mIdx );

		--mNumHandles;
	}

	template <typename T>
	T * HandleManager::Get( const Handle<T> & handle )
	{
		BH_ASSERT( CheckHandleValid( handle ) == true, "Cannot get, handle invalid." );
		return reinterpret_cast< T* >( mDataTable[ handle.mIdx ] );
	}

	template <typename T>
	const T * HandleManager::Get( const Handle<T> & handle ) const
	{
		BH_ASSERT( CheckHandleValid( handle ) == true, "Cannot get, handle invalid." );
		return mDataTable[ handle.mIdx ];
	}

	template <typename T>
	bool HandleManager::CheckHandleValid( const Handle<T> & handle ) const
	{
		return handle.mVer == mVersionTable[ handle.mIdx ];
	}


}