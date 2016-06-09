#ifndef BH_MESSAGE_H
#define BH_MESSAGE_H

#include "Core/CoreEssential.h"
#include "Core/Assert.h"

#include <vector>

namespace BH
{
	// Message that is to be sent to Job Queue (Something like std::tuple but not exactly)
	class Message
	{
	public:
		typedef std::vector<void *> Messages;		//!< Messages

	public:
		// Constructor
		Message();

		// Move Constructor
		Message( Message && msg ) BH_USE_NOEXCEPT;

		// Destructor
		~Message();

		// Move assignment
		Message & operator = ( Message && msg ) BH_USE_NOEXCEPT;

		// Add Message in sequence. (1st message is in index 0)
		template < typename T, typename... Arguments >
		void AddMsg( Arguments... args );

		// Get message with given index
		template < typename T >
		T * GetMsg( u32 index ) const;

	private:
		Messages mMessages;							//!< Stuff for sending across

	};
}

#include "Core/Message.inl"

#endif