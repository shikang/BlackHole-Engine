#ifndef TEST_THREAD_H
#define TEST_THREAD_H

#include "Platform/Mutex.h"
#include "Platform/Thread.h"
#include "Platform/ConsoleColor.h"

#include <iostream>
#include <cassert>

#define COUT_BOOL(exp)\
	{\
		bool condition = exp; \
		std::cout << ((condition) ? ConsoleGreen : ConsoleRed) << ((condition) ? "PASS" : "FAIL"); \
	}\
	std::cout << ConsoleWhite

int threadNumber = 10;

void ThreadFunction(void * args)
{
	int * num = reinterpret_cast< int * >(args);
	assert(*num == threadNumber);
	assert(num == &threadNumber);

	*num += 20;
}

void ThreadFunction2(void * args)
{
	int * num = reinterpret_cast< int * >(args);
	assert(*num == threadNumber);
	assert(num == &threadNumber);

	// 0.5 seconds should be enough of a wait.
	BH::Thread::ThreadSleep(0.5f);
	*num += 20;
}

void ThreadTest()
{
	BH::Thread newThread;

	newThread.Start(&ThreadFunction, &threadNumber);
	newThread.Join();
	COUT_BOOL(threadNumber ==  30) << " - Thread Test 1" << std::endl;

	newThread.Start(&ThreadFunction2, &threadNumber);
	COUT_BOOL(threadNumber == 30) << " - Thread Test 2" << std::endl;
	newThread.Join();
	COUT_BOOL(threadNumber == 50) << " - Thread Test 3" << std::endl;
}


void TestThread()
{
	std::cout << ConsoleYellow << "--------------------- Thread Test ------------------" << std::endl;

	ThreadTest();

	std::cout << std::endl << std::endl;
}

#undef COUT_BOOL

#endif