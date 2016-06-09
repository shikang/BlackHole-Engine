// Precompiled Header
#include "Core/Stdafx.h"

#include "Platform/Graphics.h"

#include "Core/Game.h"
#include "Core/Time.h"
#include "Core/TimePoint.h"

namespace BH
{
	Game::Game()
	: IsFixedTimeStep( true )
	, mLeftoverTime( 0.0f )
	, mShutdown( false )
	{
	}

	Game::~Game()
	{
	}

	void Game::Initialise()
	{
		Time::mFixedDt = 1.0f / 60.0f;
		Time::mTimeScale = 1.0f;

		// Create Window
		Window.OpenWindow();
		Window.GetEventList().mOnWindowClosed = std::bind( &Game::EndGame, this );

		// Initialise all systems
		SystemManager.Initialise();
	}

	void Game::Shutdown()
	{
		// Shutdown in reverse order

		// Shutdown systems
		SystemManager.Shutdown();

		// Close Window
		Window.CloseWindow();

		ObjectManager.UnloadArcheTypes();
	}

	void Game::EndGame()
	{
		mShutdown = true;
	}

	void Game::Run()
	{
		Time::mGameStartTime = TimePoint::Now();
		Time::mTime = 0.0f;

		while (!mShutdown)
		{
			Tick();
		}
	}

	void Game::Tick()
	{
		Window.Update();
		Time::mUnscaledDt = mStopwatch.Stop();
		Time::mDt = Time::mTimeScale * Time::mUnscaledDt;

		// Continue timer
		mStopwatch.Reset();
		mStopwatch.Start();

		Real timeElapsed = Time::mDt + mLeftoverTime;

		// @cleanup
		//std::cout << "FPS: " << Time::mUnscaledDt << std::endl;
		
		// If not enough time has past, do not update
		if ( IsFixedTimeStep && timeElapsed < Time::mFixedDt )
		{
			mLeftoverTime += Time::mDt;
			return;
		}

		Update( timeElapsed );

		// Spin
		while ( !SystemManager.IsAllIdling() )
			Thread::ThreadYield();

		// @cleanup
		//std::cout << "---------------------------" << std::endl;

		if ( BeginDraw() )
			Draw( timeElapsed );

		EndDraw();

		//Clean up deleted object
		ObjectManager.Purge();
	}

	void Game::Update( Real elapsed )
	{
		if ( IsFixedTimeStep )
		{
			while ( elapsed > Time::mFixedDt )
			{
				// Spin
				while ( !SystemManager.IsAllIdling() ) 
					Thread::ThreadYield();

				// update
				SystemManager.Update( Time::mFixedDt );

				elapsed -= Time::mFixedDt;
				Time::mTime += Time::mFixedDt;
			}

			mLeftoverTime = elapsed;
		}
		else
		{
			SystemManager.Update( elapsed );
			Time::mTime += elapsed;
			mLeftoverTime = 0.0f;
		}
	}

	bool Game::BeginDraw()
	{
		// @todo Graphics stuff (E.g, lost device, swap buffer)
		return true;
	}

	void Game::Draw( Real elapsed )
	{
		// Spin
		while ( !SystemManager.IsAllIdling() ) 
			Thread::ThreadYield();

		// update
		SystemManager.Draw( elapsed );
	}

	void Game::EndDraw()
	{
		// @todo Graphics stuff
	}
}