// Precompiled Header
#include "Core/Stdafx.h"

#include "Core/GameStateManager.h"
#include "Core/LevelState.h"
#include "Core/LevelManager.h"
#include "Core/XMLDeserialiser.h"

namespace BH
{
	String LevelManager::LoadDirectory = "";

	LevelManager::LevelManager()
	: mActiveLevel( false )
	{

	}

	LevelManager::~LevelManager()
	{

	}

	void LevelManager::Initialise()
	{
		LOG("Initialising Level Manager...");
		// Load all exisiting level
		FileInfo::List files = FileSystem::GetFileList( LoadDirectory.c_str(), true );

		for ( const auto & i : files )
		{
			if ( i.GetExtension() != "xml" )
				continue;

			mLevelList.insert( i.GetFileNoExt() );
		}
		LOG("Level Manager Initialised");
	}

	void LevelManager::CreateLevel( const String & newLevelName )
	{
		mLevelList.insert( newLevelName );
	}

	void LevelManager::DeleteLevel( const String & levelName )
	{
		if ( mActiveLevel && mCurrentLevel == levelName )
			RemoveLevel();

		mLevelList.erase( levelName );
	}

	void LevelManager::Shutdown()
	{
		RemoveLevel();
	}

	void LevelManager::SetLevel( const Char * level )
	{
		// No such level
		if ( mLevelList.find( String( level ) ) == mLevelList.end() )
			return;

		RemoveLevel();
		mCurrentLevel = level;

		//GAME.ObjectManager.LoadAllObjectFromFile<XMLDeserialiser>( ( LoadDirectory + level + ".xml" ).c_str() );
		LevelState * state = SYSTEM_MANAGER.GetGameComponentFromSystem<BH::GameStateManager>()->Push<LevelState>();
		state->SetLevel( level );

		mActiveLevel = true;
	}

	void LevelManager::RemoveLevel()
	{
		if ( !mActiveLevel )
			return;

		mCurrentLevel = "";

		//GAME.ObjectManager.ForceFullPurge();
		SYSTEM_MANAGER.GetGameComponentFromSystem<BH::GameStateManager>()->Pop();

		mActiveLevel = false;
	}

	bool LevelManager::IsAnyLevelActive() const
	{
		return mActiveLevel;
	}
}