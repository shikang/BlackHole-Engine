#ifndef BH_IUPDATABLE_H
#define BH_IUPDATABLE_H

#include "Core/CoreEssential.h"
#include "Core/FunctionsFramework.h"

namespace BH
{
	// Updatable interface.
	class IUpdatable
	{
	public:
		GETTER_AUTO(u32, UpdateOrder);			//!< Get update order
		GETTER_AUTO(bool, Enabled);				//!< Get updatable is enabled for update
		GETSET_AUTO(bool, Flip);				//!< Get and set the flip status

	public:
		// Constructor.
		IUpdatable(u32 updateOrder = 0, bool enabled = true);

		// Empty virtual destructor.
		virtual ~IUpdatable() {}

		// Pure virtual update function.
		virtual void Update(f32 dt) = 0;

	private:
		u32 mUpdateOrder;					//!< Update order.
		bool mEnabled;						//!< Enabled.
		bool mFlip;							//!< Bit flipping status

		friend class System;
	};

	// Comparison operator.
	bool operator < (const IUpdatable & lhs, const IUpdatable & rhs);
}

#endif