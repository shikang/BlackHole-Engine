// Precompiled Header
#include "Core/Stdafx.h"

#include "Core/IDrawable.h"

namespace BH
{
	// Constructor.
	IDrawable::IDrawable(u32 drawOrder, bool visible)
	: mDrawOrder(drawOrder)
	, mVisible(visible)
	{
	}

	bool operator < (const IDrawable & lhs, const IDrawable & rhs)
	{
		return lhs.GetDrawOrder() < rhs.GetDrawOrder();
	}
}