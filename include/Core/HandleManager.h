#ifndef BH_HANDLEMANAGER_H
#define BH_HANDLEMANAGER_H

#include "Core/CoreEssential.h"
#include "Core/Singleton.h"

namespace BH
{
	template <typename T>
	class Handle;

	class HandleManager
	{
	private:

		enum CONSTANTS
		{
			TOTAL_HANDLE = 1 << 16
		};

	public:
		DECLARE_SINGLETON( HandleManager );
		~HandleManager( );

	private:
		// Constructor
		HandleManager( );

	private:
		typedef void * DataPtr;		

		typedef std::queue< u16 > FreeTable;
		typedef std::vector< u16 > VersionTable;
		typedef std::vector< DataPtr > DataTable;

	public:

		GETTER_AUTO( u32, NumHandles );

		// template <typename T, typename... Arguments>
		// Handle<T> Acquire( );

		template <typename T>
		Handle<T> Acquire( T * ptr );

		template <typename T>
		void Release( const Handle<T> & handle );

		template <typename T>
		T * Get( const Handle<T> & handle );

		template <typename T>
		const T * Get( const Handle<T> & handle ) const;

		template <typename T>
		bool CheckHandleValid( const Handle<T> & handle ) const;

	private:
		FreeTable mFreeTable;
		VersionTable mVersionTable;
		DataTable mDataTable;

		u32 mNumHandles = 0;
	};

}

#include "Core/HandleManager.inl"

#define HANDLE_MANAGER\
	::BH::HandleManager::Instance()


#endif