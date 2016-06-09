#ifndef BH_IDRAWABLE_H
#define BH_IDRAWABLE_H

#include "Core/CoreEssential.h"
#include "Core/FunctionsFramework.h"

namespace BH
{
	// Updatable interface.
	class IDrawable
	{
	public:
		GETTER_AUTO(u32, DrawOrder);			//!< Get Draw order
		GETTER_AUTO(bool, Visible);				//!< Get Visible

	public:
		// Constructor.
		IDrawable(u32 drawOrder = 0, bool visible = true);

		// Empty virtual destructor.
		virtual ~IDrawable() {}

		// Pure virtual draw function.
		virtual void Draw(f32 dt) = 0;

	private:
		u32 mDrawOrder;					//!< Draw order.
		bool mVisible;					//!< Visible.

		friend class System;
	};

	// Comparison operator.
	bool operator < (const IDrawable & lhs, const IDrawable & rhs);
}

#endif