#ifndef BH_GAME_H
#define BH_GAME_H

#include "Platform/Mutex.h"
#include "Platform/PlatformWindow.h"

#include "Core/Uncopyable.h"
#include "Core/Singleton.h"
#include "Core/Stopwatch.h"
#include "Core/IDrawable.h"
#include "Core/SystemManager.h"
#include "Core/ObjectManager.h"

namespace BH
{
	// Handles main game loop
	class Game : private Uncopyable
	{
	public:
		DECLARE_SINGLETON(Game);

	public:
		// Settings
		bool IsFixedTimeStep;				//!< Fixed Time Step

		// Managers
		SystemManager SystemManager;		//!< System Manager
		ObjectManager ObjectManager;		//!< Object Manager
		PlatformWindow Window;				//!< Game Window

	public:
		// Destructor
		~Game();

		// Intialise game
		void Initialise();

		// Run the game
		void Run();

		// Shutdown game
		void Shutdown();

		// Call this to end the game
		void EndGame();

	private:
		// Constructor
		Game();

		// Tick
		void Tick();

		// Update game
		void Update( Real elapsed );

		// Pre draw
		bool BeginDraw();

		// Draw all drawables ever exist
		void Draw( Real elapsed );

		// Post draw
		void EndDraw();

	private:
		Stopwatch mStopwatch;				//!< Stopwatch for dt
		Real mLeftoverTime;					//!< Time leftover due to fixed timestep

		bool mShutdown;						//!< Mark to terminate
	
	};
}

#define GAME\
	::BH::Game::Instance()

#endif