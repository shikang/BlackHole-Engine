#ifndef BH_HANDLE_H
#define BH_HANDLE_H

#include "Core/CoreEssential.h"
#include "Core/HandleManager.h"

namespace BH
{

	template <typename T>
	class Handle
	{
	private:
		friend class HandleManager;

		typedef u16 IndexType;
		typedef u16 VersionType;

		typedef u32 HandleType;

	public:

		Handle( );
		Handle( u16 idx, u16 ver );
		
		inline bool IsNull( ) const;
		inline bool IsValid( ) const;

		inline operator bool( ) const;

		inline bool operator== ( const Handle & handle ) const;
		inline Handle& operator= ( const Handle & handle );

		inline T & operator* ( );
		inline const T & operator* ( ) const;

		inline T * operator-> ( );
		inline const T * operator-> ( ) const;

	private:

		union
		{
			struct
			{
				IndexType mIdx;
				VersionType mVer;
			};

			HandleType mHandle;
		};

	};


}

#include "Core/Handle.inl"

#endif