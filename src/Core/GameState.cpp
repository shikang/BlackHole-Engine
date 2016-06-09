// Precompiled Header
#include "Core/Stdafx.h"
#include "Core/GameState.h"

namespace BH
{
	GameState::GameState()
	: mActive( false )
	, mPaused( true )
	{
	}

	GameState::~GameState()
	{
	}

	Entity::OPTIONS GameState::OnPreChanged()
	{
		return Entity::O_NORMAL;
	}

	void GameState::OnChanged()
	{
	}

	Entity::OPTIONS GameState::OnPreSaved() const
	{
		return Entity::O_NORMAL;
	}

	void GameState::OnSaved() const
	{
	}
}