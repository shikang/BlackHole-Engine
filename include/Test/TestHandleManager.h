#ifndef TEST_HANDLE_MANAGER_H
#define TEST_HANDLE_MANAGER_H

#include "Core/Handle.h"
#include "Core/HandleManager.h"

#define TEST_ASSERT 0

namespace TestHandle_Manager
{
	void TestAndPrint( const String & desc, bool condition )
	{
		std::cout << ( ( condition ) ? ConsoleGreen : ConsoleRed ) << ( ( condition ) ? "PASS" : "FAIL" );
		std::cout << ConsoleWhite;
		std::cout << " - " << desc << std::endl;
	}

	// single u32 test
	void Test1( )
	{
		u32 check = 12345;
		u32 * pU32 = new u32;
		*pU32 = check;

		BH::Handle<u32> handle;

		TestAndPrint( "HandleManager Test 1 - NullHandle", handle.IsNull( ) == true );

		handle = HANDLE_MANAGER.Acquire( pU32 );

		TestAndPrint( "HandleManager Test 1 - IsValid", handle.IsValid( ) == true );
		TestAndPrint( "HandleManager Test 1 - CheckValue", *handle == check );

		BH::Handle<u32> copyHandle( handle );

		TestAndPrint( "HandleManager Test 1 - Copy Constructor & operator=", handle == copyHandle );

		HANDLE_MANAGER.Release( handle );
	}

	// multple u32 acquire
	void Test2( )
	{
		for ( u32 i = 0; i < 10; ++i )
		{
			u32 check = i;
			u32 * p = new u32;
			*p = check;

			BH::Handle<u32> handle;

			TestAndPrint( "HandleManager Test 2 - NullHandle", handle.IsNull( ) == true );

			handle = HANDLE_MANAGER.Acquire( p );

			TestAndPrint( "HandleManager Test 2 - IsValid", handle.IsValid( ) == true );
			TestAndPrint( "HandleManager Test 2 - CheckValue", *handle == check );

			BH::Handle<u32> copyHandle( handle );

			TestAndPrint( "HandleManager Test 2 - Copy Constructor & operator=", handle == copyHandle );

			HANDLE_MANAGER.Release( handle );
		}
	}

	// HandleManager Handle Count Test Acquire and Release
	void Test3( )
	{
		std::stack< Handle<u32> > handleStack;

		for ( u32 i = 0; i < ( 1 << 16 ); ++i )
		{
			u32 check = i;
			u32 * p = new u32;
			*p = check;

			handleStack.emplace( HANDLE_MANAGER.Acquire( p ) );
		}

		TestAndPrint( "HandleManager Test 3 - NumHandle", HANDLE_MANAGER.GetNumHandles( ) == ( ( 1 << 16 ) ) );

		for ( u32 i = 0; i < ( 1 << 16 ); ++i )
		{
			HANDLE_MANAGER.Release( handleStack.top( ) );

			if ( handleStack.empty( ) == false )
				handleStack.pop( );
		}

		TestAndPrint( "HandleManager Test 3 - NumHandle", HANDLE_MANAGER.GetNumHandles( ) == 0 );
	}

	// HandleManager Realeased Handle Test
	void Test4( )
	{
		u32 check = 12345;
		u32 * pU32 = new u32;
		*pU32 = check;

		BH::Handle<u32> handle;

		TestAndPrint( "HandleManager Test 4 - NullHandle", handle.IsNull( ) == true );

		handle = HANDLE_MANAGER.Acquire( pU32 );

		TestAndPrint( "HandleManager Test 4 - IsValid", handle.IsValid( ) == true );
		TestAndPrint( "HandleManager Test 4 - CheckValue", *handle == check );

		HANDLE_MANAGER.Release( handle );

		TestAndPrint( "HandleManager Test 4 - Released IsValid", handle.IsValid( ) == false );

#if TEST_ASSERT == 1
		// assert failure
		*handle;
#endif
	}

	// HandleManager Double Release
	void Test5( )
	{
#if TEST_ASSERT == 1
		BH::Handle<u32> handle;
		handle = HANDLE_MANAGER.Acquire( new u32 );
		HANDLE_MANAGER.Release( handle );
		HANDLE_MANAGER.Release( handle );
#endif
	}

	// Handle -> operator test
	struct HandleTestStruct
	{
		u32 x;
	};

	void Test6( )
	{
		u32 check = 12345;
		HandleTestStruct * p = new HandleTestStruct;
		p->x = check;

		BH::Handle<HandleTestStruct> handle;

		TestAndPrint( "HandleManager Test 6 - NullHandle", handle.IsNull( ) == true );

		handle = HANDLE_MANAGER.Acquire( p );

		TestAndPrint( "HandleManager Test 6 - IsValid", handle.IsValid( ) == true );
		TestAndPrint( "HandleManager Test 6 - CheckValue", handle->x ==  check );

		BH::Handle<HandleTestStruct> copyHandle( handle );

		TestAndPrint( "HandleManager Test 6 - Copy Constructor & operator=", handle == copyHandle );

		HANDLE_MANAGER.Release( handle );
	}

}

void HandleManagerTest( )
{
	TestHandle_Manager::Test1( );
	TestHandle_Manager::Test2( );
	TestHandle_Manager::Test3( );
	TestHandle_Manager::Test4( );
	TestHandle_Manager::Test5( );
	TestHandle_Manager::Test6( );
}

void TestHandleManager( )
{
	std::cout << ConsoleYellow << "--------------------- Handle Manager Test ------------------" << std::endl;

	HandleManagerTest( );

	std::cout << std::endl << std::endl;
}

#endif