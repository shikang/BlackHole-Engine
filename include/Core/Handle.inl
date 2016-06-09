namespace BH
{
	template <typename T>
	Handle<T>::Handle( ) : mHandle( 0 )
	{

	}

	template <typename T>
	Handle<T>::Handle( u16 idx, u16 ver ) : mIdx( idx ), mVer( ver )
	{

	}

	template <typename T>
	inline bool Handle<T>::IsValid( ) const
	{
		return HANDLE_MANAGER.CheckHandleValid( *this );
	}

	template <typename T>
	inline T & Handle<T>::operator* ( )
	{
		return *HANDLE_MANAGER.Get( *this );
	}

	template <typename T>
	inline const T & Handle<T>::operator* ( ) const
	{
		return *HANDLE_MANAGER.Get( *this );
	}

	template <typename T>
	inline T * Handle<T>::operator-> ( )
	{
		return HANDLE_MANAGER.Get( *this );
	}

	template <typename T>
	inline const T * Handle<T>::operator-> ( ) const
	{
		return HANDLE_MANAGER.Get( *this );
	}

	template <typename T>
	inline bool Handle<T>::IsNull( ) const
	{
		return mHandle == 0;
	}

	template <typename T>
	inline Handle<T>::operator bool( ) const
	{
		return !IsNull( );
	}

	template <typename T>
	inline bool Handle<T>::operator== ( const Handle & handle ) const
	{
		return mHandle == handle.mHandle;
	}

	template <typename T>
	inline Handle<T>& Handle<T>::operator= ( const Handle & handle )
	{
		mHandle = handle.mHandle;
		return *this;
	}	

}