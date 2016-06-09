#ifndef TEST_EVENT_H
#define TEST_EVENT_H

#include "Core/Delegate.h"
#include "Core/Event.h"
#include "Platform/ConsoleColor.h"

#include <iostream>

#define COUT_BOOL(exp)\
	{\
		bool condition = exp; \
		std::cout << ((condition) ? ConsoleGreen : ConsoleRed) << ((condition) ? "PASS" : "FAIL"); \
	}\
	std::cout << ConsoleWhite

typedef BH::Event< void(int x, int y) > TestingEvent;

static const int TestX = 10;
static const int TestY = 5;

// Test listener class.
class TestListener
{
public:
	void OnEvent(int x, int y)
	{
		COUT_BOOL(x == TestX) << " - Event Object Test" << std::endl;
		COUT_BOOL(y == TestY) << " - Event Object Test" << std::endl;
	}
};

class TestFunctionClass
{
public:
	// static member function
	static void TestFunction(int x, int y)
	{
		COUT_BOOL(x == TestX) << " - Event Function Test" << std::endl;
		COUT_BOOL(y == TestY) << " - Event Function Test" << std::endl;
	}
};

void EventObjectTest()
{
	TestListener		listener;
	TestingEvent		evt;

	//printf("%d", sizeof(evt));
	std::cout << ConsoleBlue << "       sizeof(Event) = " << sizeof(evt) << ConsoleWhite << std::endl;
	evt.Register(TestingEvent::Method<E_METHOD(TestListener, OnEvent)>(&listener));
	evt.Raise(10, 5);
	evt.Unregister(TestingEvent::Method<E_METHOD(TestListener, OnEvent)>(&listener));
	evt.Raise(10, 5);

	evt += TestingEvent::Method<E_METHOD(TestListener, OnEvent)>(&listener);
	evt.Raise(10, 5);
	evt -= TestingEvent::Method<E_METHOD(TestListener, OnEvent)>(&listener);
	evt.Raise(10, 5);
}

void EventFunctionTest()
{
	//TestListener		listener;
	TestingEvent		evt;

	evt.Register(TestingEvent::Function< &TestFunctionClass::TestFunction >());
	evt.Raise(10, 5);
	evt.Unregister(TestingEvent::Function< &TestFunctionClass::TestFunction >());
	evt.Raise(10, 5);

	evt += TestingEvent::Function< &TestFunctionClass::TestFunction >();
	evt.Raise(10, 5);
	evt -= TestingEvent::Function< &TestFunctionClass::TestFunction >();
	evt.Raise(10, 5);
}

//! Test listener class.
class TestListener2
{
public:
	TestListener2()
		: testEvent(NULL), tX(0), tY(0)
	{
	}

	void OnEvent(int x, int y)
	{
		BH_ASSERT_TRUE(testEvent != NULL);
		tX += x;
		tY += y;
		*testEvent -= TestingEvent::Method<E_METHOD(TestListener2, OnEvent)>(this);
	}

	TestingEvent*	testEvent;
	int			tX;
	int			tY;
};

class TestListenerOther
{
public:
	TestListenerOther()
		: unreg(nullptr), testEvent(nullptr), tX(0), tY(0)
	{
	}

	void OnEvent(int x, int y)
	{
		BH_ASSERT_TRUE(testEvent != nullptr);
		tX += x;
		tY += y;
		*testEvent -= TestingEvent::Method<E_METHOD(TestListenerOther, OnEvent)>(this);
		if (unreg)
			*testEvent -= TestingEvent::Method<E_METHOD(TestListenerOther, OnEvent)>(unreg);
	}

	TestListenerOther* unreg;
	TestingEvent*	testEvent;
	int			tX;
	int			tY;
};

void EventCurrentUnregister()
{
	TestListener2		listener;
	TestListenerOther	ol1, ol2;
	TestingEvent			evt;

	listener.testEvent = &evt;

	ol1.unreg = &ol2;
	ol1.testEvent = &evt;
	ol2.testEvent = &evt;

	evt += TestingEvent::Method<E_METHOD(TestListener2, OnEvent)>(&listener);
	evt.Raise(10, 5);
	evt.Raise(10, 5);

	COUT_BOOL(listener.tX == 10) << " - Event Unregister Test" << std::endl;
	COUT_BOOL(listener.tY == 5) << " - Event Unregister Test" << std::endl;

	evt += TestingEvent::Method<E_METHOD(TestListenerOther, OnEvent)>(&ol1);
	evt += TestingEvent::Method<E_METHOD(TestListenerOther, OnEvent)>(&ol2);
	evt.Raise(10, 5);
	evt.Raise(10, 5);

	COUT_BOOL(ol1.tX == 10) << " - Event Unregister Test" << std::endl;
	COUT_BOOL(ol1.tY == 5) << " - Event Unregister Test" << std::endl;
	COUT_BOOL(ol2.tX == 0) << " - Event Unregister Test" << std::endl;
	COUT_BOOL(ol2.tY == 0) << " - Event Unregister Test" << std::endl;
}

void TestEvent()
{
	std::cout << ConsoleYellow << "--------------------- Event Test ------------------" << std::endl;

	EventObjectTest();
	EventFunctionTest();
	EventCurrentUnregister();

	std::cout << std::endl << std::endl;
}

#undef COUT_BOOL

#endif