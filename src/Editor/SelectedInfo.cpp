#include "Editor/SelectedInfo.h"

#if GAME_EDITOR_MODE

namespace BH
{
	SelectedInfo::SelectedInfo()
		: mPositionX(nullptr)
		, mPositionY(nullptr)
		, mPositionZ(nullptr)
		, mScaleX(nullptr)
		, mScaleY(nullptr)
		, mScaleZ(nullptr)
		, mRotationX(nullptr)
		, mRotationY(nullptr)
		, mRotationZ(nullptr)
		, mModelName(nullptr) {}

	void SelectedInfo::Invalidize()
	{
		mPositionX = nullptr;
		mPositionY = nullptr;
		mPositionZ = nullptr;
		mScaleX = nullptr;
		mScaleY = nullptr;
		mScaleZ = nullptr;
		mRotationX = nullptr;
		mRotationY = nullptr;
		mRotationZ = nullptr;
		mModelName = nullptr;
	}
}

#endif