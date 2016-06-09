#ifndef BH_VECTORCONTAINER_IMPL_H
#define BH_VECTORCONTAINER_IMPL_H

#include "Platform/Typedefs.h"

#include "Core/CoreEssential.h"
#include "Core/EntityManager.h"
#include "Core/FieldContainer.h"
#include "Core/Assert.h"

#include <algorithm>
#include <type_traits>

// Vector field container reflection implementation.
// https://bitbucket.org/dwilliamson/reflectabit

namespace BH
{
	namespace detail
	{
		template< typename VECTOR_TYPE >
		class VectorReadIterator : public FieldReadIterator
		{
		public:
			VectorReadIterator( const VECTOR_TYPE * container )
				: mContainer( *container ), mPosition( 0 ) {}

			virtual const void * GetKey() const 
			{
				return nullptr;
			}

			virtual const void * GetValue() const 
			{
				BH_ASSERT_TRUE( mPosition < mContainer.size() );
				return mContainer.data() + mPosition;
				// because std::vector<bool> does not have data as member
				//return &(mContainer[mPosition]);
			}

			virtual bool IncNext() const 
			{
				if ( mPosition >= mContainer.size() ) 
					return false;

				++mPosition;
				return true;
			}

			virtual bool IsValid() const 
			{
				return mPosition < mContainer.size();
			}

			virtual u32 Count() const 
			{
				return mContainer.size();
			}

		private:
			const VECTOR_TYPE &	mContainer;
			mutable u32			mPosition;	//!< We need to modify this in const function

		};

		template< typename VECTOR_TYPE >
		class VectorWriteIterator : public FieldWriteIterator
		{
		public:
			inline VectorWriteIterator( VECTOR_TYPE * container )
				: mContainer( *container ) {}

			virtual bool Add( void * obj ) 
			{
				mContainer.push_back( *static_cast<VECTOR_TYPE::value_type *>( obj ) );
				return true;
			}

			virtual bool Add( void *, void * ) 
			{
				BH_FAIL( "Implementation not supported." );
				return false;
			}

			virtual void * AddEmpty() 
			{
				mContainer.push_back( VECTOR_TYPE::value_type() );
				return &mContainer.back();
			}

			virtual void * AddEmpty( void * ) 
			{
				BH_FAIL( "Implementation not supported." );
				return false;
			}

		private:
			VECTOR_TYPE &	mContainer;

		};

		template < typename VECTOR_TYPE >
		void VectorInsertFunction( void * container, void *, void * val )
		{
			if ( !val )
				reinterpret_cast< VECTOR_TYPE * >( container )->push_back( VECTOR_TYPE::value_type() );
			else
				reinterpret_cast< VECTOR_TYPE * >( container )->push_back( *static_cast< VECTOR_TYPE::value_type * >( val ) );
		}

		template < typename VECTOR_TYPE >
		void VectorRemoveFunction( void * container, void * , void * val )
		{
			if ( !val )
				return;

			VECTOR_TYPE & vec = *reinterpret_cast< VECTOR_TYPE * >( container );
			typename VECTOR_TYPE::iterator it = std::find( vec.begin(), vec.end(), *static_cast<VECTOR_TYPE::value_type *>( val ) );
			if ( it != vec.end() )
				vec.erase( it );
		}

		// std::vector container creation specialization.
		template <typename TYPE, typename ALLOCATOR>
		FieldContainer * CreateContainer( std::vector<TYPE, ALLOCATOR> & )
		{
			FieldContainer * cont = new FieldContainer_Impl< std::vector<TYPE, ALLOCATOR>,
															 VectorReadIterator< std::vector<TYPE, ALLOCATOR> >,
															 VectorWriteIterator< std::vector<TYPE, ALLOCATOR> > >();

			//cont->ValueType = &ENTITY_MANAGER.Types.Create<TYPE>();
			cont->ValueType = &TypeDB::Instance().Create<TYPE>();
			cont->IsValuePointer = std::is_pointer<TYPE>::value;
			cont->Insert = &VectorInsertFunction < std::vector< TYPE, ALLOCATOR > >;
			cont->Remove = &VectorRemoveFunction < std::vector< TYPE, ALLOCATOR > >;
			return cont;
		}
	}
}

#endif // BH_VECTORCONTAINER_IMPL_H
