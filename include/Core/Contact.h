#ifndef BH_MANIFOLD_H
#define BH_MANIFOLD_H

#include "Core/CoreEssential.h"
#include "Core/OBB.h"

#include "Platform/Vector3f.h"

namespace BH
{
	union FeaturePair
	{
		struct
		{
			u8 mInR;
			u8 mOutR;
			u8 mInI;
			u8 mOutI;
		};

		s32 mKey;
	};

	struct Contact
	{
		Vector3f mPosition;			// World coordinate of contact
		f32 mPenetration;			// Depth of penetration from collision
		//FeaturePair mFeaturePair;	// Features on A and B for this contact
	};

	// Store info for resolution
	struct Manifold
	{
		//OBB * mLhs;
		//OBB * mRhs;
		String mLHS;
		String mRHS;
		String mLHSTag;
		String mRHSTag;

		Vector3f mNormal;		//!< From lhs to rhs

		s32 mNumContact;		//!< Number of contact point
		Contact mContacts[8];	//!< Contact info

		Manifold()
		: mNormal( Vector3f::ZERO )
		, mNumContact( 0 ) {}
	};
}

#endif