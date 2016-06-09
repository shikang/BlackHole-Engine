// Precompiled Header
#include "Platform/Stdafx.h"

#include "Platform/Platform.h"
#include "Platform/Thread.h"

#include <windows.h>

namespace BH
{
	Thread::Thread()
	: mThread()
	{

	}

	Thread::~Thread()
	{
		//if (mThread)
		//	CloseHandle(mThread);

		// Simulate CloseHandle
		// http://msdn.microsoft.com/en-us/library/windows/desktop/ms686724(v=vs.85).aspx
		if (mThread.joinable())
			mThread.detach();
	}

	bool Thread::TryJoin(u32 milliseconds)
	{
		// Because joinable does not tell me handle is null
		if (!mThread.native_handle()) 
			return true;

		// Because joinable does not tell me what error, we need platform specific code here
		switch (WaitForSingleObject(mThread.native_handle(), DWORD(milliseconds + 1)))
		{
		// Thread is still running
		case WAIT_TIMEOUT:
			return false;
		// Thread has completed
		case WAIT_OBJECT_0:
			return true;
		// Other errors
		default:
			return false;
		}
	}

	bool Thread::IsRunnning()
	{
		HANDLE thread_handle = mThread.native_handle();
		if (thread_handle != nullptr)
		{
			DWORD exitCode = 0;
			return GetExitCodeThread(thread_handle, &exitCode) && (exitCode == STILL_ACTIVE);
		}
		return false;
	}
}