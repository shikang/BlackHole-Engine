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
	private:
		Uncopyable(const Uncopyable&);
		Uncopyable & operator=(const Uncopyable &);
    
	public:
		Uncopyable() {};
		virtual ~Uncopyable() {};
	};
}

#endif