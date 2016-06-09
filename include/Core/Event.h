/*
	Usage:

		namespace E
		{
			typedef BH::Event< void( int ) > KeyPress; //!< params: keycode.
		}

		class TestClass
		{
		public:
			void KeyPressTest( int keycode ) { printf( "pressed %d", keycode ); }
		};

		E::KeyPress OnKeyPress;
		TestClass testClass;
		OnKeyPress += E::KeyPress::Method<E_METHOD(TestClass,KeyPressTest)>(&testClass);
		OnKeyPress( 5 );

	// Output: pressed 5
*/

#ifndef BH_EVENT_H
#define BH_EVENT_H

// Convenience macro.
// Usage: Event< void() >::Method<E_METHOD(classType, classMethod)>(classInstance);
#define E_METHOD(classType, classMethod)\
	classType, &classType::classMethod

#include "Platform/Typedefs.h"
#include "Platform/Mutex.h"
#include "Core/Uncopyable.h"
#include "Core/Delegate.h"
#include "Core/Assert.h"

#include <vector>

namespace BH
{
	// Need this line for the code to work. 
	template < typename > class Event;

	template < typename RType, typename... Arguments >
	class Event< RType(Arguments...) > : private Uncopyable
	{
	public:
		typedef RType ReturnType;
		typedef Delegate< RType(Arguments...) > Delegate;	//!< Event delegate type.

	public:
		// Default constructor.
		inline Event() {}

		// Destructor.
		inline ~Event() {}

		// Raise an event with arguments.
		inline void Raise(Arguments... args)
		{
			RaiseImpl( std::bind( &Delegate::Call, std::placeholders::_1, args... ) );
		}

		// Raise an event using specified object with arguments.
		inline void ObjectRaise(void * obj, Arguments... args)
		{
			RaiseImpl( std::bind( &Delegate::ObjectCall, std::placeholders::_1, obj, args...) );
		}

		// Raise an event with arguments using the () operator.
		inline void operator () (Arguments... args)
		{
			Raise(args...);
		}

		// Register a listener function to the event.
		inline void Register(const typename Delegate & delegate) 
		{
			MUTEX_SCOPEDLOCK(mtxEvent);
			mDelegates.push_back(delegate);
		}

		// Increment operator used to register a listener function to the event.
		inline void operator += (const typename Delegate & delegate) 
		{
			Register(delegate);
		}

		// Unregister a listener function from the event.
		void Unregister(const typename Delegate & delegate) 
		{
			MUTEX_SCOPEDLOCK(mtxEvent);
			u32 size = mDelegates.size();
			for (u32 i = 0; i < size; ++i) 
			{
				if (mDelegates[i] == delegate) 
				{
					mDelegates[i].Reset();
					mSweep = true;
					break;
				}
			}
		}

		// Decrement operator used to unregister a listener function from the event.
		inline void operator -= (const typename Delegate & delegate) 
		{
			Unregister(delegate);
		}

		// Clears all registered event listeners.
		inline void Clear() 
		{
			MUTEX_SCOPEDLOCK(mtxEvent);
			mDelegates.clear();
		}

	public:
		// Wrapper function for getting a function delegate.
		template< ReturnType(*OMethod)(Arguments...) >
		inline static typename Delegate Function() 
		{
			return Delegate::FromFunction< OMethod >();
		}

		// Wrapper function for getting a class method delegate.
		template< class O, ReturnType(O::*OMethod)(Arguments...) >
		inline static typename Delegate Method(O * objectPtr) 
		{
			return Delegate::FromMethod< O, OMethod >(objectPtr);
		}

		// Wrapper function for getting a class const method delegate.
		template< class O, ReturnType(O::*OMethod)(Arguments...) const >
		inline static typename Delegate CMethod(const O * objectPtr) 
		{
			return Delegate::FromConstMethod< O, OMethod >(objectPtr);
		}

	private:
		// Sweep up invalidated events.
		void Sweep() 
		{
			DelegateList::iterator it = mDelegates.begin();
			while (it != mDelegates.end()) 
			{
				if (!*it)	
					it = mDelegates.erase(it);
				else		
					++it;
			}

			mSweep = false;
		}

		// Implementation of Raise
		template< typename Function >
		void RaiseImpl(Function f)
		{
			MUTEX_SCOPEDLOCK(mtxEvent);

			u32 size = mDelegates.size();
			if (size)
			{
				for (u32 i = 0; i < size; ++i)
				{
					Delegate run = mDelegates[i];
					if (run)
						f(run);
				}

				if (mSweep)
					Sweep();
			}
		}

	private:
		typedef std::vector< typename Delegate >	DelegateList;	//!< Event delegate list.

	private:
		typename DelegateList	mDelegates;		//!< Event delegates.
		bool					mSweep;			//!< Clean invalid delegates.
		MUTEX_DECLARE(mtxEvent);

	};
}

#endif	// BH_EVENT_H