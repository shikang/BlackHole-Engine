#ifndef BH_EVENT_QUEUE_H
#define BH_EVENT_QUEUE_H

#include "Core/CoreEssential.h"
#include "Core/Delegate.h"
#include "Core/Message.h"
#include "Core/Uncopyable.h"

#include <functional>
#include <queue>
#include <tuple>

namespace BH
{
	// Job System
	class JobQueue : private Uncopyable
	{
	public:
		typedef Delegate< void( const Message & ) > Job;	//!< Job type

	public:
		// Constructor
		JobQueue();

		// Destructor
		~JobQueue();

		// Add a job ( Message will be emptied )
		void AddJob( u32 priority, const Job & job, Message & msg );

		// Check if there is available job to work on
		bool JobAvailable() const;

		// Work on available jobs
		void Work();

	private:
		// Entry for queue to manage
		struct JobEntry
		{
		public:
			// Default Constructor
			JobEntry();

			// Constructor
			JobEntry( u32 priority, const Job & job, Message & msg );

			// Move Constructor ( For priority move to move object about )
			JobEntry( JobEntry && job ) BH_USE_NOEXCEPT;

			// Move Assignment
			JobEntry & operator = ( JobEntry && job ) BH_USE_NOEXCEPT;

			// Greater comparison operator
			bool operator > ( const JobEntry & rhs ) const;

			// Equality operator
			bool operator == ( const Job & job ) const;

		public:
			Message mMessage;
			Job mJob;
			u32 mPriority;

		};

		typedef std::priority_queue< JobEntry, std::vector<JobEntry>, std::greater<JobEntry> > JobQueueList;

	private:
		JobQueueList mJobs;			//!< Event delegates
		MUTEX_DECLARE(mJobMutex);	//!< Mutex for multi-threading
		bool mWorking;

	};
}

#include "Core/JobQueue.inl"

#endif