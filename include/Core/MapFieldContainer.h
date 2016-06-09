#ifndef BH_MAPCONTAINER_IMPL_H
#define BH_MAPCONTAINER_IMPL_H

#include "Platform/Typedefs.h"

#include "Core/CoreEssential.h"
#include "Core/EntityManager.h"
#include "Core/FieldContainer.h"
#include "Core/Assert.h"

// External Libraries
#include <map>
#include <unordered_map>

// Map field container reflection implementation.
// https://bitbucket.org/dwilliamson/reflectabit

namespace BH
{
	namespace detail
	{
		// Normal/Hashed map iterator
		template< typename MAP_TYPE >
		class MapReadIterator :	public FieldReadIterator
		{
		public:
			typedef typename MAP_TYPE::const_iterator				Iterator;

		public:
			inline MapReadIterator(const MAP_TYPE * container)
				: mContainer(*container), mIterator( container->begin() ) {}

			virtual const void * GetKey() const 
			{
				BH_ASSERT_TRUE( mIterator != mContainer.end() );
				return &(mIterator->first);
			}

			virtual const void * GetValue() const 
			{
				BH_ASSERT_TRUE( mIterator != mContainer.end() );
				return &(mIterator->second);
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
			const MAP_TYPE &	mContainer;
			mutable Iterator	mIterator;	//!< We need to modify this in const function

		};

		template< typename MAP_TYPE >
		class MapWriteIterator : public FieldWriteIterator
		{
		public:
			typedef typename MAP_TYPE::const_iterator			Iterator;
			typedef typename MAP_TYPE::value_type::first_type	KeyType;
			typedef typename MAP_TYPE::value_type::second_type	ValueType;

		public:
			inline MapWriteIterator(MAP_TYPE * container)
				: mContainer(*container) {}

			virtual bool Add(void *) 
			{
				BH_FAIL("Implementation not supported.");
				return false;
			}

			virtual bool Add(void * key, void * obj) 
			{
				return mContainer.insert( std::make_pair( *static_cast<KeyType *>(key), *static_cast<ValueType *>(obj) ) ).second;
			}

			virtual void * AddEmpty() 
			{
				BH_FAIL("Feature not implemented.");
				return nullptr;
			}

			virtual void * AddEmpty(void * key) 
			{
				return &( mContainer[ *static_cast<KeyType *>(key) ] );
			}

		private:
			MAP_TYPE &	mContainer;

		};

		template < typename MAP_TYPE >
		void MapInsertFunction( void * container, void * key, void * val )
		{
			if ( key )
				reinterpret_cast< MAP_TYPE * >( container )->insert( MAP_TYPE::value_type( *static_cast<MAP_TYPE::key_type *>( val ), 
																						   val ? *static_cast<MAP_TYPE::mapped_type *>( val ) : MAP_TYPE::mapped_type() ) );
		}

		template < typename MAP_TYPE >
		void MapRemoveFunction( void * container, void * key, void * )
		{
			if ( !key )
				return;

			MAP_TYPE & map = *reinterpret_cast< MAP_TYPE * >( container );
			map.erase( *static_cast<MAP_TYPE::key_type *>( key ) );
		}

		// std::map container creation specialization.
		template< typename KEY, typename DATA, typename COMPARE, typename ALLOC >
		FieldContainer * CreateContainer( std::map<KEY, DATA, COMPARE, ALLOC> & )
		{
			// Can't deal with keys that are pointers
			BH_STATIC_ASSERT(std::is_pointer<KEY>::value == false, "MapFieldContainer: Key is pointer!");

			typedef typename std::map<KEY, DATA, COMPARE, ALLOC> MapType;

			FieldContainer * cont = new FieldContainer_Impl< MapType,
															 MapReadIterator<MapType>,
															 MapWriteIterator<MapType> >();

			//cont->KeyType = &ENTITY_MANAGER.Types.Create<KEY>();
			//cont->ValueType = &ENTITY_MANAGER.Types.Create<DATA>();
			cont->KeyType = &TypeDB::Instance().Create<KEY>();
			cont->ValueType = &TypeDB::Instance().Create<DATA>();
			cont->IsKeyPointer = std::is_pointer<KEY>::value;
			cont->IsValuePointer = std::is_pointer<DATA>::value;

			return cont;
		}

		// std::unordered_map container creation specialization.
		template< typename KEY, typename DATA, typename COMPARE, typename ALLOC >
		FieldContainer * CreateContainer( std::unordered_map<KEY, DATA, COMPARE, ALLOC> & )
		{
			// Can't deal with keys that are pointers
			BH_STATIC_ASSERT( std::is_pointer<KEY>::value == false, "MapFieldContainer: Key cannot be a pointer!" );

			typedef typename std::unordered_map<KEY, DATA, COMPARE, ALLOC> MapType;

			FieldContainer* cont = new FieldContainer_Impl< MapType,
															MapReadIterator<MapType>,
															MapWriteIterator<MapType> >();

			//cont->KeyType = &ENTITY_MANAGER.Types.Create<KEY>();
			//cont->ValueType = &ENTITY_MANAGER.Types.Create<DATA>();
			cont->KeyType = &TypeDB::Instance().Create<KEY>();
			cont->ValueType = &TypeDB::Instance().Create<DATA>();
			cont->IsKeyPointer = std::is_pointer<KEY>::value;
			cont->IsValuePointer = std::is_pointer<DATA>::value;

			return cont;
		}
	}
}

#endif // BH_MAPCONTAINER_IMPL_H