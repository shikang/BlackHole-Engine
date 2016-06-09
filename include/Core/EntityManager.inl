// Entity type manager. Handles type registrations, news and deletes.
// https://bitbucket.org/dwilliamson/reflectabit
namespace BH
{
	template< typename TYPE >
	inline TYPE* EntityManager::New() const
	{
		return static_cast<TYPE *>( New( Types.Get<TYPE>() ) );
	}

	inline void* EntityManager::New( const CName & type ) const
	{
		const Type * t = Types.Get( type );
		return t ? New( t ) : nullptr;
	}

	template< typename TYPE >
	inline void EntityManager::Delete( TYPE * obj ) const
	{
		const Type * type = Types.Get<TYPE>();
		type->Delete( obj );
	}
}
