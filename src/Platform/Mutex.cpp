// Precompiled Headers
#include "Platform/StdAfx.h"

#include "Platform/Mutex.h"

// Dependencies
#include <chrono>
#include <ratio>
#include <iostream>

namespace BH
{
	Mutex::Mutex()
	{
		InitializeCriticalSectionAndSpinCount(&mCS, 4000);
	}

	Mutex::~Mutex()
	{
		DeleteCriticalSection(&mCS);
	}

	bool Mutex::TryLock(u32 milliseconds)
	{
		const DWORD interval = 5;
		s64 compare = s64(milliseconds);

		std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		std::chrono::milliseconds time_elaspe;
		do
		{
			if (TryEnterCriticalSection(&mCS) == TRUE)
			//if (mCS.try_lock())
				return true;

			::Sleep(interval);
			time_elaspe = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::steady_clock::now() - start);

			//std::cout << time_elaspe.count() << std::endl;
		} while (time_elaspe.count() < compare);

		if (milliseconds > 0)
			return (TryEnterCriticalSection(&mCS) == TRUE);
			//return (mCS.try_lock());

		return false;
	}
}