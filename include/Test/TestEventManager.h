#ifndef TEST_EVENT_MANAGER_H
#define TEST_EVENT_MANAGER_H

#include "Platform/ConsoleColor.h"
#include "Platform/Thread.h"

#include "Core/Delegate.h"
#include "Core/EventManager.h"

#include <iostream>
#include <atomic>

#define COUT_BOOL(exp)\
	{\
		bool condition = exp; \
		std::cout << ((condition) ? ConsoleGreen : ConsoleRed) << ((condition) ? "PASS" : "FAIL"); \
	}\
	std::cout << ConsoleWhite

namespace TestEvent_Manager
{
	std::atomic<BH::u32> gCount = 0;

	void TestEvent1()
	{
		for (BH::s32 i = 0; i < 10000; ++i)
			gCount += 10;
	}

	void TestEvent2()
	{
		for (BH::s32 i = 0; i < 10000; ++i)
			gCount += 10;
	}

	void TestEvent3(int x, int y)
	{
		for (BH::s32 i = 0; i < 10000; ++i)
		{
			gCount += x;
			gCount += y;
		}
	}

	class TestEvent4
	{
	public:
		void Call(int x, int y)
		{
			for (BH::s32 i = 0; i < 10000; ++i)
			{
				gCount += x;
				gCount += y;
			}
		}
	};

	template< typename... Arguments >
	void TestThreadedEvent(const BH::String & eventName, Arguments... args)
	{
		EVENT_MANAGER.RaiseEvent(eventName, args...);
	}

	void TestThreadedRegisterEvent()
	{
		EVENT_MANAGER.RegisterEvent("NoParamsEvent", BH::Delegate<void()>::FromFunction<&TestEvent_Manager::TestEvent1>());
		BH::Thread::ThreadSleep(100u);
		EVENT_MANAGER.RegisterEvent("NoParamsEvent", BH::Delegate<void()>::FromFunction<&TestEvent_Manager::TestEvent2>());
		BH::Thread::ThreadSleep(100u);
		EVENT_MANAGER.RegisterEvent("GotParamsEvent", BH::Delegate<void(int, int)>::FromFunction<&TestEvent_Manager::TestEvent3>());
	}

	void TestThreadedUnregisterEvent()
	{
		EVENT_MANAGER.UnregisterEvent("NoParamsEvent", BH::Delegate<void()>::FromFunction<&TestEvent_Manager::TestEvent1>());
		BH::Thread::ThreadSleep(200u);
		EVENT_MANAGER.UnregisterEvent("NoParamsEvent", BH::Delegate<void()>::FromFunction<&TestEvent_Manager::TestEvent2>());
		BH::Thread::ThreadSleep(200u);
		EVENT_MANAGER.UnregisterEvent("GotParamsEvent", BH::Delegate<void(int, int)>::FromFunction<&TestEvent_Manager::TestEvent3>());
	}

	class TestObjectLevel_EventManager
	{
	public:
		TestObjectLevel_EventManager(const BH::String & name)
		: mTestName(name)
		, mCalled(false)
		{}

		void Call()
		{
			COUT_BOOL(!mCalled) << " - " << mTestName  << std::endl;
			mCalled = true;
		}

	private:
		BH::String mTestName;
		bool mCalled;

	};
}

void EventManagerTest()
{
	//EVENT_MANAGER.AddEvent("NoParamsEvent");
	//EVENT_MANAGER.AddEvent<int, int>("GotParamsEvent");

	EVENT_MANAGER.RegisterEvent("NoParamsEvent", BH::Delegate<void()>::FromFunction<&TestEvent_Manager::TestEvent1>());
	EVENT_MANAGER.RegisterEvent("NoParamsEvent", BH::Delegate<void()>::FromFunction<&TestEvent_Manager::TestEvent1>());
	EVENT_MANAGER.RegisterEvent("NoParamsEvent", BH::Delegate<void()>::FromFunction<&TestEvent_Manager::TestEvent2>());
	EVENT_MANAGER.UnregisterEvent("NoParamsEvent", BH::Delegate<void()>::FromFunction<&TestEvent_Manager::TestEvent1>());

	TestEvent_Manager::TestEvent4 o, o1;

	EVENT_MANAGER.RegisterEvent("GotParamsEvent", BH::Delegate<void(int, int)>::FromFunction<&TestEvent_Manager::TestEvent3>());
	EVENT_MANAGER.RegisterEvent("GotParamsEvent", BH::Delegate<void(int, int)>::FromMethod<E_METHOD(TestEvent_Manager::TestEvent4, Call)>(&o));
	EVENT_MANAGER.RegisterEvent("GotParamsEvent", BH::Delegate<void(int, int)>::FromMethod<E_METHOD(TestEvent_Manager::TestEvent4, Call)>(&o1));
	EVENT_MANAGER.UnregisterEvent("GotParamsEvent", BH::Delegate<void(int, int)>::FromMethod<E_METHOD(TestEvent_Manager::TestEvent4, Call)>(&o1));

	BH::Thread t1, t2;
	t1.Start(&TestEvent_Manager::TestThreadedEvent<>, "NoParamsEvent");
	t2.Start(&TestEvent_Manager::TestThreadedEvent<int, int>, "GotParamsEvent", 1, 9);

	t1.Join();
	t2.Join();

	COUT_BOOL(TestEvent_Manager::gCount == 400000) << " - Event Manager Multi-Threaded Raise Test 1" << std::endl;

	EVENT_MANAGER.DestroyEvent("NoParamsEvent");
	EVENT_MANAGER.RaiseEvent("NoParamsEvent");

	COUT_BOOL(TestEvent_Manager::gCount == 400000) << " - Event Manager Destory Event Test 1" << std::endl;

	EVENT_MANAGER.DestroyEvent("GotParamsEvent");
	EVENT_MANAGER.RaiseEvent("GotParamsEvent");

	COUT_BOOL(TestEvent_Manager::gCount == 400000) << " - Event Manager Destory Event Test 2" << std::endl;

	// Test thread safe
	t1.Start(&TestEvent_Manager::TestThreadedRegisterEvent);
	t2.Start(&TestEvent_Manager::TestThreadedUnregisterEvent);

	EVENT_MANAGER.RaiseEvent("NoParamsEvent");
	EVENT_MANAGER.RaiseEvent("GotParamsEvent");

	COUT_BOOL(TestEvent_Manager::gCount == 400000) << " - Event Manager Multi-Threaded Raise Test 2" << std::endl;

	// Object Level Test
	TestEvent_Manager::TestObjectLevel_EventManager to1("Event Manager Object Level Test 1");
	TestEvent_Manager::TestObjectLevel_EventManager to2("Event Manager Object Level Test 2");

	EVENT_MANAGER.RegisterObjectEvent("ObjectLevelEvent", BH::Delegate<void()>::FromMethod<E_METHOD(TestEvent_Manager::TestObjectLevel_EventManager, Call)>(nullptr));
	EVENT_MANAGER.RaiseObjectEvent("ObjectLevelEvent", &to1);
	EVENT_MANAGER.RaiseObjectEvent("ObjectLevelEvent", &to2);

	EVENT_MANAGER.DestroyObjectEvent("ObjectLevelEvent");
	EVENT_MANAGER.RaiseObjectEvent("ObjectLevelEvent", &to1);
}

void TestEventManager()
{
	std::cout << ConsoleYellow << "--------------------- Event Manager Test ------------------" << std::endl;

	EventManagerTest();

	std::cout << std::endl << std::endl;
}

#undef COUT_BOOL

#endif