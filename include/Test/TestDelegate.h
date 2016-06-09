#ifndef TEST_DELEGATE_H
#define TEST_DELEGATE_H

#include "Platform/Typedefs.h"
#include "Platform/ConsoleColor.h"
#include "Core/Delegate.h"

#include <iostream>

#define COUT_BOOL(exp)\
	{\
		bool condition = exp; \
		std::cout << ((condition) ? ConsoleGreen : ConsoleRed) << ((condition) ? "PASS" : "FAIL"); \
	}\
	std::cout << ConsoleWhite

void TestVoidFunc() { std::cout << ConsoleBlue << "       Void Function" << ConsoleWhite << std::endl; }
std::string TestDelegateFunc1(int & t) { std::cout << ConsoleBlue << "       Test function 1 called " << t << ConsoleWhite << std::endl; return "Function1"; }
std::string TestDelegateFunc2(int t1, int t2) { std::cout << ConsoleBlue << "       Test function 2 called " << t1 << ", " << t2 << ConsoleWhite << std::endl; return "Function2"; }
std::string TestDelegateFunc3(int t1, int t2, int t3) { std::cout << ConsoleBlue << "       Test function 3 called " << t1 << ", " << t2 << ", " << t3 << ConsoleWhite << std::endl; return "Function3"; }

class TestObj
{
public:
	std::string operator()(const int & t)
	{
		std::cout << ConsoleBlue << "       TestObj " << t << ConsoleWhite << std::endl;
		return "TestObj1";
	}
};

class TestObj2 : public TestObj
{
public:
	TestObj2(const BH::Char * str) : String(str) {}

public:
	std::string OnEvent(int t1, int t2)
	{
		std::cout << ConsoleBlue << "       " << String << ", " << t1 << ", " << t2 << ConsoleWhite << std::endl;
		return String;
	}

private:
	std::string String;
};

typedef BH::Delegate< void() > Delegate0P;
typedef BH::Delegate< std::string(int &) > Delegate1P;
typedef BH::Delegate< std::string(int, int) > Delegate2P;
typedef BH::Delegate< std::string(int, int, int) > Delegate3P;

void DelegateTest()
{
	TestObj2 obj1("OBJECT1");
	TestObj2 obj2("OBJECT2");

#if 1
	Delegate0P dg0 = Delegate0P::FromFunction<&TestVoidFunc>();
	Delegate1P dg1 = Delegate1P::FromFunction<&TestDelegateFunc1>();
	Delegate2P dg2 = Delegate2P::FromFunction<&TestDelegateFunc2>();
	Delegate3P dg3 = Delegate3P::FromFunction<&TestDelegateFunc3>();
	Delegate2P dg4 = Delegate2P::FromMethod< TestObj2, &TestObj2::OnEvent >(&obj1);
	Delegate2P dg5 = Delegate2P::FromMethod< TestObj2, &TestObj2::OnEvent >(&obj2);
	Delegate2P dg6 = Delegate2P::FromMethod< TestObj2, &TestObj2::OnEvent >(&obj1);
	Delegate3P dg7 = Delegate3P::FromFunction<&TestDelegateFunc3>();
#else
	Delegate0P dg0 = Delegate0P::Bind(&TestVoidFunc);
	Delegate1P dg1 = Delegate1P::Bind(&TestDelegateFunc1);
	Delegate2P dg2 = Delegate2P::Bind(&TestDelegateFunc2);
	Delegate3P dg3 = Delegate3P::Bind(&TestDelegateFunc3);
	Delegate2P dg4 = Delegate2P::Bind(&obj1, &TestObj2::OnEvent);
	Delegate2P dg5 = Delegate2P::Bind(&obj2, &TestObj2::OnEvent);
	Delegate2P dg6 = Delegate2P::Bind(&obj1, &TestObj2::OnEvent);
	Delegate3P dg7 = Delegate3P::Bind(&TestDelegateFunc3);
#endif

	std::cout << ConsoleBlue << "       sizeof(Delegate) = " << sizeof(dg0) << ConsoleWhite << std::endl;
	std::cout << ConsoleBlue << "       sizeof(std::function) = " << sizeof(std::function<void()>) << ConsoleWhite << std::endl;

	int t1 = 1, t2 = 2, t3 = 3;
	COUT_BOOL(dg1(t1) == "Function1") << " - Delegate Test 1" << std::endl;
	COUT_BOOL(dg2(t1, t2) == "Function2") << " - Delegate Test 2" << std::endl;
	COUT_BOOL(dg3(t1, t2, t3) == "Function3") << " - Delegate Test 3" << std::endl;
	COUT_BOOL(dg4(t1, t2) == "OBJECT1") << " - Delegate Test 4" << std::endl;
	COUT_BOOL(dg5(t1, t2) == "OBJECT2") << " - Delegate Test 5" << std::endl;
	COUT_BOOL(dg6(t1, t2) == "OBJECT1") << " - Delegate Test 6" << std::endl;

	COUT_BOOL(dg4 == dg6) << " - Delegate Comparsion Test 1" << std::endl;
	COUT_BOOL(dg3 == dg7) << " - Delegate Comparison Test 2" << std::endl;
}

void TestDelegate()
{
	std::cout << ConsoleYellow << "--------------------- Delegate Test ------------------" << std::endl;

	DelegateTest();

	std::cout << std::endl << std::endl;
}

#undef COUT_BOOL

#endif