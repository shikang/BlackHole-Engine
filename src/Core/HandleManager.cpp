// Precompiled Headers
#include "Core/StdAfx.h"
#include "Core/HandleManager.h"
#include "Core/Handle.h"

namespace BH
{

	HandleManager::HandleManager( ) :
		mVersionTable( TOTAL_HANDLE, 0 ),
		mDataTable( TOTAL_HANDLE, nullptr )
	{
		for ( u32 i = 0; i < TOTAL_HANDLE; ++i )
			mFreeTable.push( static_cast< Handle<u32>::IndexType >( i ) );
	}

	HandleManager::~HandleManager( )
	{

	}



}