#ifndef TEST_PROFILE_H
#define TEST_PROFILE_H

#include "Platform/Typedefs.h"
#include "Platform/ConsoleColor.h"
#include "Core/Profiling.h"

#include <iostream>

#define COUT_BOOL(exp)\
	{\
		bool condition = exp; \
		std::cout << ((condition) ? ConsoleGreen : ConsoleRed) << ((condition) ? "PASS" : "FAIL"); \
	}\
	std::cout << ConsoleWhite

void ProfileRecursion(const BH::Char * str)
{
	PROFILE_FUNCTION;

	if (*str)
		return;

	ProfileRecursion(str + 1);
}

void ProfileFunctionChild()
{
	PROFILE_FUNCTION;
	
	BH::s32 r = 0;
	for (BH::s32 i = 0; i < 1000; ++i)
	{
		r += i;
	}
}

void ProfileFunctionSibling()
{
	PROFILE_FUNCTION;

	BH::s32 r = 1;
	for (BH::s32 i = 0; i < 1000; ++i)
	{
		r *= 2;
	}
}

void ProfileFunctionMain()
{
	PROFILE_FUNCTION;

	ProfileFunctionChild();
	ProfileFunctionSibling();
}

void ProfileFunctionMainSibling()
{
	PROFILE_FUNCTION;

	ProfileFunctionSibling();
	ProfileFunctionChild();

	BH::s32 r = 0;
	for (BH::s32 i = 0; i < 1000; ++i)
	{
		r -= i;
	}
}

void TestProfile()
{
	ProfileFunctionMain();
	ProfileFunctionMainSibling();
}
#undef COUT_BOOL

#endif