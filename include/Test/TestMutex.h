#ifndef TEST_MUTEX_H
#define TEST_MUTEX_H

#include "Platform/Mutex.h"
#include "Platform/Thread.h"
#include "Platform/ConsoleColor.h"

#include <iostream>

#define COUT_BOOL(exp)\
	{\
		bool condition = exp; \
		std::cout << ((condition) ? ConsoleGreen : ConsoleRed) << ((condition) ? "PASS" : "FAIL"); \
	}\
	std::cout << ConsoleWhite

BH::Mutex mtx;

void mutexThread(void * args)
{
	mtx.Lock();

	int & num = *(reinterpret_cast< int * >(args));
	int tmp = num + 1;
	BH::Thread::ThreadYield();
	num = tmp;

	mtx.Unlock();
}

void mutexScopedThread(void * args)
{
	BH::MutexScopedLock lock(mtx);

	int & num = *(reinterpret_cast< int * >(args));
	int tmp = num + 1;
	BH::Thread::ThreadYield();
	num = tmp;
}

int loopNum = 1000000;
void raceConditionThread(int * args)
{
	int & num = *args;

	for (int i = 0; i < loopNum; ++i)
	{
		BH::MutexScopedLock lock(mtx);
		num += 1;
	}
}

const BH::u32 totalThreads = 100;

void MutexLockTest()
{
	int mutexNumber = 0;
	BH::Thread newThread[totalThreads];

	for (int i = 0; i < totalThreads; ++i)
	{
		newThread[i].Start(mutexThread, &mutexNumber);
	}

	while (1)
	{
		bool finished = true;
		for (int i = 0; i < totalThreads; ++i)
		{
			if (!newThread[i].TryJoin(0))
			{
				finished = false;
				break;
			}
		}

		if (finished)
			break;
	}

	COUT_BOOL(mutexNumber == totalThreads) << " - Mutex Lock Test" << std::endl;

}

void MutexScopedLockTest()
{
	int mutexNumber = 0;
	BH::Thread newThread[totalThreads];

	for (int i = 0; i < totalThreads; ++i)
	{
		newThread[i].Start(mutexScopedThread, &mutexNumber);
	}

	while (1)
	{
		bool finished = true;
		for (int i = 0; i < totalThreads; ++i)
		{
			if (!newThread[i].TryJoin(0))
			{
				finished = false;
				break;
			}
		}

		if (finished)
			break;
	}

	COUT_BOOL(mutexNumber == totalThreads) << " - Mutex Scoped Lock Test" << std::endl;
}

void RaceConditionTest()
{
	BH::Thread t[2];

	int raceNumber = 0;

	t[0].Start(&raceConditionThread, &raceNumber);
	t[1].Start(&raceConditionThread, &raceNumber);

	while (1)
	{
		bool finished = true;
		for (int i = 0; i < 2; ++i)
		{
			if (!t[i].TryJoin(0))
			{
				finished = false;
				break;
			}
		}

		if (finished)
			break;
	}

	COUT_BOOL(raceNumber == (loopNum * 2)) << " - Race Condition Test" << std::endl;
}

void TestMutex()
{
	std::cout << ConsoleYellow << "--------------------- Mutex Test ------------------" << std::endl;

	MutexLockTest();
	MutexScopedLockTest();
	RaceConditionTest();

	std::cout << std::endl << std::endl;
}

#undef COUT_BOOL

#endif