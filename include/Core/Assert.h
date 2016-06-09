#ifndef BH_ASSERT_H
#define BH_ASSERT_H

#include "Platform/Typedefs.h"

#include "Core/CoreConfig.h"

namespace BH
{
	/*
		Assertion break.
		Usage:

			Char * tmp = nullptr;
			BH_ASSERT_TRUE( tmp );
			BH_ASSERT( tmp != nullptr, "tmp is null!" );
	*/
	void Assert(Char * const expression,
				const Char * const message,
				const Char * const filename,
				const size_t line);

	#define BH_FORCE_ASSERT( message )\
		{ ::BH::Assert("Force assert.", message, __FILE__, __LINE__); }

	#if BH_ASSERT_MODE == BH_ENABLED

		// Assert expression is not null.
		#define BH_ASSERT_TRUE( exp )\
			{ if (!(exp)) ::BH::Assert(#exp, "Expression is false. Expected true.", __FILE__, __LINE__); }
		// Assert expression is null.
		#define BH_ASSERT_FALSE( exp )\
			{ if ((exp)) ::BH::Assert(#exp, "Expression is true. Expected false.", __FILE__, __LINE__); }
		// Assert expression.
		#define BH_ASSERT( exp, message )\
			{ if (!(exp)) { std::ostringstream m; m << message; ::BH::Assert(#exp, m.str().c_str(), __FILE__, __LINE__); } }
		#define BH_FAIL( message )\
			{ std::ostringstream m; m << message; ::BH::Assert("Assertion failed.", m.str().c_str(), __FILE__, __LINE__); }

	#else
		#define BH_ASSERT_TRUE( exp )			
		#define BH_ASSERT_FALSE( exp )			
		#define BH_ASSERT( exp, message )		
		#define BH_FAIL( message )				
	#endif

	#define BH_STATIC_ASSERT( condition, message )	static_assert(condition, message)
}

#endif // BH_ASSERT_H
