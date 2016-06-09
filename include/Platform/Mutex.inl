#include <iostream>

namespace BH
{
	inline void Mutex::Lock()
	{
		EnterCriticalSection(&mCS);
	}

	inline void Mutex::Unlock()
	{
		LeaveCriticalSection(&mCS);
	}

	inline MutexScopedLock::MutexScopedLock(Mutex & mtx)
	: mMutex(mtx)
	{
		mtx.Lock();
	}

	inline MutexScopedLock::~MutexScopedLock()
	{
		mMutex.Unlock();
	}
}