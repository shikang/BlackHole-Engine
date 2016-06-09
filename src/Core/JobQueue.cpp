// Precompiled Header
#include "Core/Stdafx.h"

#include "Core/JobQueue.h"

namespace BH
{
	JobQueue::JobEntry::JobEntry( u32 priority, const Job & job, Message & msg )
	: mPriority( priority )
	, mMessage( std::move( msg ) )
	, mJob( job )
	{
	}

	JobQueue::JobQueue()
	: mWorking( false )
	{
	}

	JobQueue::~JobQueue()
	{
	}

	void JobQueue::Work()
	{
		{
			MUTEX_SCOPEDLOCK(mJobMutex);
			mWorking = !mJobs.empty();
		}

		while ( mWorking )
		{
			JobEntry j;
			// Remove and process current
			{
				// top always return const references,therefore unable to cast it to r-value references
				MUTEX_SCOPEDLOCK(mJobMutex);
				j = std::move( const_cast<JobEntry &>( mJobs.top() ) );
				mJobs.pop();
			}

			j.mJob(j.mMessage);

			{
				MUTEX_SCOPEDLOCK(mJobMutex);
				mWorking = !mJobs.empty();
			}
		}
	}
}