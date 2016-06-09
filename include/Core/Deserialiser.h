#ifndef BH_DESERIALISER_H
#define BH_DESERIALISER_H

#include "Core/CoreEssential.h"
#include "Core/Assert.h"
#include "Core/EntityManager.h"

namespace BH
{
	// Base deserialiser class.
	class Deserialiser
	{
	public:
		typedef void *		StreamPtr;	//!< Stream pointer type.

	public:
		// Constructor.
		Deserialiser();

		// Destructor.
		virtual ~Deserialiser();

		// Deserialisation method.
		template< typename OBJECT_TYPE >
		bool Deserialise(OBJECT_TYPE & obj);

		// Deserialisation method.
		template< typename OBJECT_TYPE >
		bool DeserialisePtr(OBJECT_TYPE * & obj);

		// Deserialisation method.
		bool Deserialise(const Type * type, void * & obj);

		// Check if the stream has ended.
		virtual bool IsStreamEnded() const = 0;

	protected:
		// Traverse the stream specified by the path.
		virtual StreamPtr TraverseStream(const Char * path) = 0;

	protected:
		// Recursive deserialise method.
		virtual StreamPtr DoDeserialise(StreamPtr s,					//!< Stream Pointer
										const Type* type,				//!< Type of object
										void *& obj,					//!< Object
										const FieldAttributes* attr,	//!< Attributes
										const Field* field,				//!< Field
										bool inferType					//!< If true, get type through deserialisation
										) = 0;

	protected:
		StreamPtr mStream;	//!< Stream pointer.

	};

	template< typename OBJECT_TYPE >
	inline bool Deserialiser::Deserialise(OBJECT_TYPE & obj)
	{
		BH_STATIC_ASSERT(std::is_pointer<OBJECT_TYPE>::value == false, "Deserialiser::Deserialise - OBJECT_TYPE is pointer type!");
		void * o = &obj;
		return Deserialise(ENTITY_MANAGER.Types.Get<OBJECT_TYPE>(), o);
	}

	template< typename OBJECT_TYPE >
	inline bool Deserialiser::DeserialisePtr(OBJECT_TYPE* & obj)
	{
		BH_STATIC_ASSERT(std::is_pointer<OBJECT_TYPE>::value == false, "Deserialiser::DeserialisePtr - OBJECT_TYPE is pointer type!");
		void * o = obj;
		if (Deserialise(ENTITY_MANAGER.Types.Get<OBJECT_TYPE>(), o))
		{
			obj = static_cast<OBJECT_TYPE*>(o);
			return true;
		}
		return false;
	}
}

#endif // BH_DESERIALISER_H