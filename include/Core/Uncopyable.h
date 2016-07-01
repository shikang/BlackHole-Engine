#ifndef BH_UNCOPYABLE_H
#define BH_UNCOPYABLE_H

/*
	Inherit from this class if the class cannot be copied.
	E.g. Manager class, mutex.
*/

// Disable annoying warning about non dll-interface class.
#pragma warning( disable:4275 )

namespace BH
{
	class Uncopyable
	{
#ifdef BH_CPLUSPLUS11
	public:
		Uncopyable( const Uncopyable & ) = delete;
		Uncopyable & operator=( const Uncopyable & ) = delete;
#else
	private:
		Uncopyable( const Uncopyable & );
		Uncopyable & operator=( const Uncopyable & );
#endif
    
	public:
		Uncopyable() {};
		virtual ~Uncopyable() {};
	};
}

#endif