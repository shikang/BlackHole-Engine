#ifndef BH_LEVEL_MANAGER_H
#define BH_LEVEL_MANAGER_H

#include "Core/CoreEssential.h"
#include "Core/GameComponent.h"
#include "Core/FunctionsFramework.h"

#include <unordered_set>

namespace BH
{
	class LevelManager : public GameComponent
	{
	public:
		typedef std::unordered_set<String> LeveList;

		GETTER_AUTO_CREF( String, CurrentLevel );
		GETTER_AUTO_CREF( LeveList, LevelList );

	public:
		// Constructor
		LevelManager();

		// Destructor
		~LevelManager();

		// Initialise
		void Initialise() override;

		// Shutdown
		void Shutdown() override;

		// Set level
		void SetLevel( const Char * level );

		// Remove level
		void RemoveLevel();

		// Is there an active level
		bool IsAnyLevelActive() const;

		// Create new level (for level editor)
		void CreateLevel( const String & newLevelName );

		// Delete level (for level editor)
		void DeleteLevel( const String & levelName );

	private:
		LeveList mLevelList;					//!< List of all levels
		bool mActiveLevel;						//!< Is there an active level
		String mCurrentLevel;					//!< Current level

	public:
		static String LoadDirectory;			//!< Directory for level

	};
}

#include "Core/Typing.h"
DECLARE_TYPE( ::BH::LevelManager, LevelManager );

#endif