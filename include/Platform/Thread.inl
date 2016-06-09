#include "Thread.h"

namespace BH
{
	template <typename Fn, typename... Args>
	void Thread::Start(Fn && f, Args&&... args)
	{
		mThread = std::thread(f, std::forward<Args>(args)...);
	}

	inline Thread::ThreadID Thread::GetThreadID() const
	{
		return mThread.get_id();
	}

	inline void Thread::Join()
	{
		if (mThread.joinable())
			mThread.join();
		// May want to log something
	}

	inline void Thread::Detach()
	{
		mThread.detach();
	}

	inline void Thread::ThreadSleep(f32 seconds)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(u32(seconds * 1000.0f)));
	}

	inline void Thread::ThreadSleep(u32 milliseconds)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
	}

	inline void Thread::ThreadYield()
	{
		std::this_thread::yield();
	}

	inline Thread::ThreadID Thread::GetCurrentID()
	{
		return std::this_thread::get_id();
	}
}