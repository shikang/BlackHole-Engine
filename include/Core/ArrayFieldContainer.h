#ifndef BH_ARRAYCONTAINER_IMPL_H
#define BH_ARRAYCONTAINER_IMPL_H

#include "Platform/Typedefs.h"

#include "Core/CoreEssential.h"
#include "Core/EntityManager.h"
#include "Core/FieldContainer.h"
#include "Core/Assert.h"

#include <type_traits>

// https://bitbucket.org/dwilliamson/reflectabit
// Array Field Container Implementation

namespace BH
{
	namespace detail
	{
		template< typename TYPE, u32 LENGTH >
		class ArrayReadIterator : public FieldReadIterator
		{
		public:
			inline ArrayReadIterator( const TYPE * container )
				: mContainer( container ), mPosition( 0 ) {}

			virtual const void * GetKey() const 
			{ 
				return nullptr; 
			}

			virtual const void * GetValue() const 
			{
				BH_ASSERT_TRUE( mPosition < LENGTH );
				return mContainer + mPosition;
			}

			virtual bool IncNext() const 
			{
				bool oversize = mPosition >= LENGTH;
				mPosition = oversize ? mPosition : mPosition + 1;
				return oversize;
			}

			virtual bool IsValid() const 
			{
				return mPosition < LENGTH;
			}

			virtual u32 Count() const 
			{
				return LENGTH;
			}

		private:
			const TYPE *	mContainer;
			mutable u32		mPosition;	//!< We need to modify this in const function

		};

		template< typename TYPE, u32 LENGTH >
		class ArrayWriteIterator : public FieldWriteIterator
		{
		public:
			inline ArrayWriteIterator( TYPE * container )
				: mContainer( container ), mPosition(0) {}

			virtual bool Add( void * obj ) 
			{
				if ( mPosition >= LENGTH ) 
					return false;

				mContainer[mPosition++] = *static_cast<TYPE*>( obj );
				return true;
			}

			virtual bool Add( void *, void * ) 
			{
				BH_FAIL("Implementation not supported.");
				return false;
			}

			virtual void * AddEmpty() 
			{
				if ( mPosition >= LENGTH ) 
					return nullptr;

				mContainer[mPosition] = TYPE();
				return &mContainer[mPosition++];
			}

			virtual void * AddEmpty( void * ) 
			{
				BH_FAIL("Implementation not supported.");
				return false;
			}

		private:
			TYPE *	mContainer;
			u32		mPosition;

		};

		template< typename TYPE, u32 LENGTH >
		void ArrayInsertFunction( void *, void *, void * )
		{
			// Logic not yet implemented
		}

		template< typename TYPE, u32 LENGTH >
		void ArrayRemoveFunction( void *, void *, void * )
		{
			// Logic not yet implemented
		}

		// Plain Old array container creation specialization.
		template < typename TYPE, u32 LENGTH >
		FieldContainer * CreateContainer( TYPE( & )[LENGTH] )
		{
			FieldContainer * cont = new FieldContainer_Impl< TYPE,
															 ArrayReadIterator<TYPE, LENGTH>,
															 ArrayWriteIterator<TYPE, LENGTH> >();

			//cont->ValueType = &ENTITY_MANAGER.Types.Create<TYPE>();
			cont->ValueType = &TypeDB::Instance().Create<TYPE>();
			cont->IsValuePointer = std::is_pointer<TYPE>::value;
			cont->Insert = &ArrayInsertFunction< TYPE, LENGTH >;
			cont->Remove = &ArrayRemoveFunction< TYPE, LENGTH >;

			return cont;
		}
	}
}

#endif // BH_ARRAYCONTAINER_IMPL_H
