// Precompiled Header
#include "Core/Stdafx.h"

#include "Core/IUpdatable.h"

namespace BH
{
	// Constructor.
	IUpdatable::IUpdatable(u32 updateOrder, bool enabled)
	: mUpdateOrder(updateOrder)
	, mEnabled(enabled)
	, mFlip(false)
	{
	}

	bool operator < (const IUpdatable & lhs, const IUpdatable & rhs)
	{
		return lhs.GetUpdateOrder() < rhs.GetUpdateOrder();
	}
}