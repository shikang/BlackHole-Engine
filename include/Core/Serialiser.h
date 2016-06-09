#ifndef BH_SERIALISER_H
#define BH_SERIALISER_H

#include "Core/CoreEssential.h"
#include "Core/Assert.h"
#include "Core/EntityManager.h"

namespace BH
{
	// Base serialiser class.
	class Serialiser
	{
	public:
		typedef void *		StreamPtr;			//!< Stream pointer type.

	public:
		// Constructor.
		Serialiser();

		// Destructor.
		virtual ~Serialiser();

		// Serialise object of specified type.
		template< typename OBJECT_TYPE >
		Serialiser & Serialise(const OBJECT_TYPE & obj);

		// Serialise object of specified type.
		Serialiser & Serialise(const Type * type, const void * obj);

	protected:
		// Traverse the stream specified by the path.
		virtual StreamPtr TraverseStream(const Char* path) = 0;

	protected:
		// Actual serialise method.
		virtual StreamPtr DoSerialise(StreamPtr s,					//!< Stream Pointer
									  const Type * type,			//!< Type of the object
									  const void * obj,				//!< Object
									  const FieldAttributes * attr,	//!< Attributes
									  const Field * field			//!< Field
									  ) = 0;

	protected:
		StreamPtr	mStream;	//!< Stream pointer.

	};

	template< typename OBJECT_TYPE >
	inline Serialiser & Serialiser::Serialise(const OBJECT_TYPE & obj)
	{
		BH_STATIC_ASSERT(std::is_pointer<OBJECT_TYPE>::value == false, "Serialiser::Serialise - OBJECT_TYPE is pointer type!");
		return Serialise(ENTITY_MANAGER.Types.Get<OBJECT_TYPE>(), &obj);
	}
}

#endif // BH_SERIALISER_H