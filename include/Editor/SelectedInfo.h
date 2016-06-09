#ifndef BH_SELECTED_INFO_H
#define BH_SELECTED_INFO_H

#include "Game/GameConfig.h"

#if GAME_EDITOR_MODE

#include "Core/CoreEssential.h"

namespace BH
{
	class SelectedInfo
	{
	public:
		// Constructor
		SelectedInfo();

		// Invalid-ize object
		void Invalidize();

	public:
		f32 * mPositionX;
		f32 * mPositionY;
		f32 * mPositionZ;

		f32 * mScaleX;
		f32 * mScaleY;
		f32 * mScaleZ;

		f32 * mRotationX;
		f32 * mRotationY;
		f32 * mRotationZ;

		String * mMeshName;
	};
}

#endif

#endif