/*
	Note: This is useful for event.

	Using delegate or std::function, using a function object with
	the class pointer as a private member, and calling the member
	function. However, this will lead to many function object with
	same structure, why not create a class that generalize it?

	We should actually use this (http://codereview.stackexchange.com/questions/14730/impossibly-fast-delegate-in-c11):
	std::function<float(int, int)> dg = std::bind(&TestClass::test2, obj, std::placeholders::_1, std::placeholders::_2);
	However, we need comparsion between delegate, therefore this is impossible. Best alternative is to create a wrapper function
*/

/*
	Usage:

	void Print() { std::cout << "void ()" << std::endl; }
	void Print1(int i) { std::cout << "void (int)" << std::endl; }
	void Print2(int i, int j) { std::cout << "void (int, int)" << std::endl; }

	void UseDelegate()
	{
		// Non member function
		Delegate<void ()> dg0 = Delegate<void ()>::FromFunction<&Print>();
		Delegate<void (int)> dg1 = Delegate<void (int)>::FromFunction<&Print1>();
		Delegate<void (int, int)> dg2 = Delegate<void (int, int)>::FromFunction<&Print2>();

		// Members functions
		std::string s = "Delegate";
		std::vector<int> v;
		Delegate<void ()> dgs = Delegate<void ()>::FromMethod<std::string, &std::string::clear>(&s);
		Delegate<void (int)> dgv = Delegate<void (int)>::FromMethod<std::vector<int>, &std::vector<int>::push_back>(&v);

		dg0();		// Output: "void ()"
		dg1(1);		// Output: "void (int)"
		dg2(1, 2);	// Output: "void (int, int)"

		dgs();		// s == ""
		dgv(1);		// v: 1
	}
*/

#ifndef BH_DELEGATE_H
#define BH_DELEGATE_H

#include "Platform/Platform.h"

#define BH_DELEGATE_CALLTYPE BH_FASTCALL

// Templated delegate object that can store a function pointer or a member function pointer.
// http://www.codeproject.com/KB/cpp/ImpossiblyFastCppDelegate.aspx.
namespace BH
{
	// Need this line for the code to work. 
	template < typename > class Delegate;

	template <typename RType, typename... Arguments>
	class Delegate< RType(Arguments...) >
	{
	public:
		typedef RType ReturnType;													//!< Delegate return type.
		typedef ReturnType(BH_DELEGATE_CALLTYPE * SignatureType)(Arguments...);	//!< Function pointer type

	public:
		// Get a function delegate.
		template< ReturnType(*TMethod)(Arguments...) >
		inline static Delegate FromFunction()
		{
			return FromStub(nullptr, &FunctionStub< TMethod >);
		}

		// Get a class method delegate.
		template< class T, ReturnType(T::*TMethod)(Arguments...) >
		inline static Delegate FromMethod(T * objectPtr)
		{
			return FromStub(objectPtr, &MethodStub< T, TMethod >);
		}

		// Get a class const method delegate.
		template< class T, ReturnType(T::*TMethod)(Arguments...) const >
		inline static Delegate FromConstMethod(T const * objectPtr)
		{
			return FromStub(const_cast< T * >(objectPtr), &ConstMethodStub< T, TMethod >);
		}

		// Trigger delegate.
		inline ReturnType Call(Arguments... args) const
		{
			return ObjectCall(mObjectPtr, args...);
		}

		// Bracket operator used to trigger delegate.
		inline ReturnType ObjectCall(void * obj, Arguments... args) const
		{
			return (*mStubPtr)(obj, args...);
		}

		// Bracket operator used to trigger delegate.
		inline ReturnType operator()(Arguments... args) const
		{
			return Call(args...);
		}

		// Check if delegate is empty.
		inline operator bool() const
		{
			return mStubPtr != nullptr;
		}

		// Logical negation operator.
		inline bool operator!() const
		{
			return !(operator bool());
		}

		// For STL container storage.
		inline bool operator < (const Delegate & rhs) const
		{
			return (mObjectPtr < rhs.mObjectPtr) && (mStubPtr < rhs.mStubPtr);
		}

		// Compares two delegates if they are the same.
		inline bool operator == (const Delegate & rhs) const
		{
			return (rhs.mObjectPtr == mObjectPtr) && (rhs.mStubPtr == mStubPtr);
		}

		// Resets the delegate to nullptr (for invalidation).
		inline void Reset()
		{
			mStubPtr = nullptr;
		}

	private:
		// Function callback type.
		typedef ReturnType(BH_DELEGATE_CALLTYPE * StubType)(void * objectPtr, Arguments...);

	private:
		// Constructor.
		inline Delegate() : mObjectPtr(nullptr), mStubPtr(nullptr) {}

		// Overloaded constructor.
		inline Delegate(void * objPtr, StubType stubPtr) : mObjectPtr(objPtr), mStubPtr(stubPtr) {}

		// Get the delegate 'stub'
		inline static Delegate FromStub(void * objectPtr, StubType stubType)
		{
			return Delegate(objectPtr, stubType);
		}

		// Call the delegate function 'stub'.
		template< ReturnType(*TMethod)(Arguments...) >
		inline static ReturnType BH_DELEGATE_CALLTYPE FunctionStub(void *, Arguments... args)
		{
			return (TMethod)(args...);
		}

		// Call the delegate method 'stub'.
		template< class T, ReturnType(T::*TMethod)(Arguments...) >
		inline static ReturnType BH_DELEGATE_CALLTYPE MethodStub(void * objectPtr, Arguments... args)
		{
			T * p = static_cast< T * >(objectPtr);
			return (p->*TMethod)(args...);
		}

		// Call the delegate const method 'stub'.
		template< class T, ReturnType(T::*TMethod)(Arguments...) const >
		inline static ReturnType BH_DELEGATE_CALLTYPE ConstMethodStub(void * objectPtr, Arguments... args)
		{
			T const * p = static_cast< T * >(objectPtr);
			return (p->*TMethod)(args...);
		}


	private:
		void *		mObjectPtr;		//!< Object pointer.
		StubType 	mStubPtr;		//!< Function mapping pointer.

	};
}

#endif