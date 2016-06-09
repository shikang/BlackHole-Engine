#ifndef BH_SETCONTAINER_IMPL_H
#define BH_SETCONTAINER_IMPL_H

#include "Platform/Typedefs.h"

#include "Core/CoreEssential.h"
#include "Core/EntityManager.h"
#include "Core/FieldContainer.h"
#include "Core/Assert.h"

// External Libraries
#include <set>
#include <unordered_set>

// Set field container reflection implementation.
// https://bitbucket.org/dwilliamson/reflectabit

namespace BH
{
	namespace detail
	{
		// Normal/Hashed set iterator
		template< typename SET_TYPE >
		class SetReadIterator : public FieldReadIterator
		{
		public:
			typedef typename SET_TYPE::const_iterator				Iterator;

		public:
			inline SetReadIterator( const SET_TYPE * container )
				: mContainer( *container ), mIterator( container->begin() ) {}

			virtual const void * GetKey() const 
			{
				return nullptr;
			}

			virtual const void * GetValue() const 
			{
				BH_ASSERT_TRUE( mIterator != mContainer.end() );
				return &( *mIterator );
			}

			virtual bool IncNext() const 
			{
				if ( mIterator == mContainer.end() )
					return false;

				++mIterator;
				return true;
			}

			virtual bool IsValid() const 
			{
				return mIterator != mContainer.end();
			}

			virtual u32 Count() const 
			{
				return mContainer.size();
			}

		private:
			const SET_TYPE &	mContainer;
			mutable Iterator	mIterator;	//!< We need to modify this in const function

		};

		template< typename SET_TYPE >
		class SetWriteIterator : public FieldWriteIterator
		{
		public:
			typedef typename SET_TYPE::const_iterator			Iterator;
			typedef typename SET_TYPE::value_type				ValueType;

		public:
			inline SetWriteIterator( SET_TYPE * container )
				: mContainer( *container ) {}

			virtual bool Add( void * obj ) 
			{
				return mContainer.insert( *static_cast<ValueType *>( obj ) ).second;
			}

			virtual bool Add( void *, void * ) 
			{
				BH_FAIL( "Implementation not supported." );
				return false;
			}

			virtual void * AddEmpty() 
			{
				std::pair<Iterator, bool> r = mContainer.insert( ValueType() );
				return &( r.first );
			}

			virtual void * AddEmpty( void * ) 
			{
				BH_FAIL( "Feature not implemented." );
				return nullptr;
			}

		private:
			SET_TYPE &	mContainer;

		};

		template < typename SET_TYPE >
		void SetInsertFunction( void * container, void *, void * val )
		{
			if ( val )
				reinterpret_cast< SET_TYPE * >( container )->insert( *static_cast<SET_TYPE::value_type *>( val ) );
		}

		template < typename SET_TYPE >
		void SetRemoveFunction( void * container, void * , void * val )
		{
			if ( !val )
				return;

			SET_TYPE & set = *reinterpret_cast< SET_TYPE * >( container );
			set.erase( *static_cast<SET_TYPE::value_type *>( val ) );
		}

		// std::set container creation specialization.
		template< typename KEY, typename COMPARE, typename ALLOC >
		FieldContainer * CreateContainer( std::set<KEY, COMPARE, ALLOC> & )
		{
			// Can't deal with keys that are pointers
			BH_STATIC_ASSERT( std::is_pointer<KEY>::value == false, "SetFieldContainer: Key cannot be a pointer!" );

			typedef typename std::set<KEY, COMPARE, ALLOC> SetType;

			FieldContainer * cont = new FieldContainer_Impl< SetType,
															 SetReadIterator<SetType>,
															 SetWriteIterator<SetType> >();

			//cont->ValueType = &ENTITY_MANAGER.Types.Create<KEY>();
			cont->ValueType = &TypeDB::Instance().Create<KEY>();
			cont->IsValuePointer = std::is_pointer<KEY>::value;
			cont->Insert = &SetInsertFunction < SetType >;
			cont->Remove = &SetRemoveFunction < SetType >;

			return cont;
		}

		// std::unordered_set container creation specialization.
		template< typename KEY, typename COMPARE, typename ALLOC >
		FieldContainer * CreateContainer( std::unordered_set<KEY, COMPARE, ALLOC> & )
		{
			// Can't deal with keys that are pointers
			BH_STATIC_ASSERT( std::is_pointer<KEY>::value == false, "SetFieldContainer: Key is pointer!" );

			typedef typename std::unordered_set<KEY, COMPARE, ALLOC> SetType;

			FieldContainer * cont = new FieldContainer_Impl< SetType,
															 SetReadIterator<SetType>,
															 SetWriteIterator<SetType> >();

			//cont->ValueType = &ENTITY_MANAGER.Types.Create<KEY>();
			cont->ValueType = &TypeDB::Instance().Create<KEY>();
			cont->IsValuePointer = std::is_pointer<KEY>::value;
			cont->Insert = &SetInsertFunction < SetType >;
			cont->Remove = &SetRemoveFunction < SetType >;

			return cont;
		}
	}
}

#endif // BH_SETCONTAINER_IMPL_H