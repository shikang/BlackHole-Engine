#ifndef BH_THREAD_H
#define BH_THREAD_H

#include "Platform/Typedefs.h"
#include "Platform/Platform.h"

#include <thread>

namespace BH
{
	class Thread
	{
	public:
		// typedef DWORD	ThreadID;
		typedef std::thread::id ThreadID;

	public:
		// Get this thread ID
		ThreadID GetThreadID() const;

		// Is thread still running
		bool IsRunnning();

	public:
		// Constructor
		Thread();

		// Destructor
		~Thread();

		// Start thread with supplied function and args
		template <typename Fn, typename... Args>
		void Start(Fn && f, Args&&... args);

		// Wait for this thread to complete
		void Join();

		// Wait for this thread to complete given an amount of time
		bool TryJoin(u32 milliseconds);

		// Make thread run independently
		void Detach();

	public:
		// Static Functions

		// Make current active thread sleep
		static void ThreadSleep(f32 seconds);

		// Make current active thread sleep
		static void ThreadSleep(u32 milliseconds);

		// Hint to reschedule thread
		static void ThreadYield();

		// Get current thread ID
		static ThreadID GetCurrentID();

	private:
		// No copy constructor
		Thread(const Thread &);

		// No assignment operator
		const Thread & operator = (const Thread &);

	private:
		typedef std::thread	PlatformThread;
		PlatformThread	mThread;	//!< Platform specific thread handle

	};
}

#include "Platform/Thread.inl"

#endif // BH_THREAD_H