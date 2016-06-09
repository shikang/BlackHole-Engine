#include "Test/TestMutex.h"
#include "Test/TestThread.h"
#include "Test/TestVector2f.h"
#include "Test/TestDelegate.h"
#include "Test/TestEvent.h"
#include "Test/TestEventManager.h"
#include "Test/TestHash.h"
#include "Test/TestReflection.h"
#include "Test/TestXMLSerialiser.h"
#include "Test/TestProfile.h"
#include "Test/TestHandleManager.h"
#include "Test/TestSystem.h"
#include "Test/TestScript.h"

#include <iostream>
#include <windows.h>

// There is a need to link al static library though core depend on platform
// http://www.cprogramdevelop.com/1026460/
int main ()
{
	// Test Platform
	TestMutex();
	TestThread();
	TestVector2f();
	
	// Test Core
	TestDelegate();
	TestEvent();
	TestEventManager();
	TestHash();
	TestReflection();
	TestXML();
	TestProfile();
	TestHandleManager( );
	TestSystem();

	// Test Script
	TestScript();

	return 0;
}

#if 0
BH::s32 Test(BH::f32 p0, BH::s64 p1, const std::pair<BH::f64, BH::String> & p2, const std::vector<BH::u32> & p3)
{
	std::cout << std::endl << "--------------------------------" << std::endl;
	std::cout << "p0 = " << p0 << std::endl;
	std::cout << "p1 = " << p1 << std::endl;
	std::cout << "p2.first = " << p2.first << std::endl;
	std::cout << "p2.second = " << p2.second << std::endl;

	BH::u32 size = p3.size();
	for (BH::u32 i = 0; i < size; ++i)
	{
		std::cout << "p3[" << i << "] = " << p3[i] << std::endl;
	}

	return 1;
}

int main()
{
	// Chrono test
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point end;
	std::chrono::milliseconds time_elaspe;
	do
	{
		Sleep(5);

		end = std::chrono::steady_clock::now();
		time_elaspe = std::chrono::duration_cast< std::chrono::milliseconds >(end - start);
		std::cout << time_elaspe.count() << std::endl;
	} while (time_elaspe.count() < 100);

	std::cout << std::chrono::duration_cast< std::chrono::nanoseconds >(end - start).count() << std::endl;
	std::cout << std::chrono::duration_cast< std::chrono::microseconds >(end - start).count() << std::endl;
	std::cout << std::chrono::duration_cast< std::chrono::seconds >(end - start).count() << std::endl;
	std::chrono::milliseconds ms(100);

	std::cout << ms.count() << std::endl;
	std::chrono::seconds sec(100);
	std::cout << sec.count() << std::endl;

	// std::bind test
	std::vector<BH::u32> v;
	for (BH::s32 i = 0; i < 6; ++i)
	{
		v.push_back(5 + i);
	}
	auto fn = std::bind(&Test, 1.f, 2, std::make_pair(3.0, "Four"), v);
	fn();

	auto fn2 = fn;
	auto fn3 = std::move(fn);

	fn2();
	fn3();
	fn();
	return 0;
}
#endif