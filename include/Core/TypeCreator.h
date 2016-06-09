#ifndef BH_TYPE_CREATOR_H
#define BH_TYPE_CREATOR_H

#include "Core/TypeRegistrar.h"
#include "Core/EntityManager.h"
#include "Core/Type.h"

namespace BH
{
	template< typename T >
	struct TypeCreator
	{
		inline static void * RegisterType();
	};
}

#define PASTE( _, __ )  _##__
#define GENERATE_LINE( _ ) PASTE( GENERATED_TOKEN_, _ )
#define GENERATE_FILE( _ ) PASTE( __FILE__, _ )
#define NAME_GENERATOR() GENERATE_FILE( __LINE__ )

#define MARK_DEFINE_TYPE( className )\
	friend struct TypeCreator<className>

// Had to friend Type Creator<className>
#define BEGIN_DEFINE_TYPE( className )\
	namespace BH\
	{\
		template<>\
		inline void * TypeCreator<className>::RegisterType()\
		{

#define END_DEFINE_TYPE( className )\
		return nullptr;\
		}\
	}\
	namespace\
	{\
		void * NAME_GENERATOR() = ::BH::TypeCreator<className>::RegisterType();\
	}
#endif