// Precompiled Header
#include "Core/Stdafx.h"
#include "Core/LevelState.h"

namespace BH
{
	LevelState::LevelState()
	{
	}

	LevelState::~LevelState()
	{
	}

	Entity::OPTIONS LevelState::OnPreChanged()
	{
		return Entity::O_NORMAL;
	}

	void LevelState::OnChanged()
	{
	}

	Entity::OPTIONS LevelState::OnPreSaved() const
	{
		return Entity::O_NORMAL;
	}

	void LevelState::OnSaved() const
	{
	}

	void LevelState::Activate()
	{
		// Load level
		GAME.ObjectManager.LoadAllObjectFromFile<XMLDeserialiser>( ( LevelManager::LoadDirectory + mLevel + ".xml" ).c_str() );
	}

	void LevelState::Deactivate()
	{
		// Unload level
		GAME.ObjectManager.ForceFullPurge();
	}

	void LevelState::Resume()
	{
		// Resume updates
	}

	void LevelState::Pause()
	{
		// Pause all updates
	}
}