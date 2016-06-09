namespace BH
{
	inline Message::Message() {}

	inline Message::Message( Message && msg ) BH_USE_NOEXCEPT
	: mMessages( std::move( msg.mMessages ) )
	{
	}

	inline Message::~Message()
	{
		for ( auto & i : mMessages )
			delete i;
	}

	inline Message & Message::operator = ( Message && msg ) BH_USE_NOEXCEPT
	{
		mMessages = std::move( msg.mMessages );
		return *this;
	}

	template < typename T, typename... Arguments >
	inline void Message::AddMsg( Arguments... args )
	{
		T * msg = new T( std::forward<Arguments>(args)... );
		mMessages.push_back( msg );
	}

	template < typename T >
	T * Message::GetMsg( u32 index ) const
	{
		BH_ASSERT( index < mMessages.size(), "Message::GetMessage - index out of bound!" );
		return reinterpret_cast<T *>( mMessages[index] );
	}
}