// Precompiled Header
#include "Core/Stdafx.h"

#include "Core/SystemManager.h"
#include "Core/System.h"

namespace BH
{
	SystemManager::SystemManager()
	: mUpdateFlag( false )
	, mMainSystem( new System( "Main System" ) )
	{
	}

	SystemManager::~SystemManager()
	{
		for ( auto & i : mSystems )
		{
			delete i.second.first;
			delete i.second.second;
		}

		mSystems.clear();

		delete mMainSystem;
	}

	void SystemManager::Initialise()
	{
		mMainSystem->Initialise();

		for ( auto & i : mSystems )
		{
			// Initialise system
			i.second.first->Initialise();
			
			// Start thread
			i.second.second->Start( &SystemManager::SystemLoop, i.second.first );
		}
	}

	void SystemManager::Shutdown()
	{
		// Stop all system
		for ( auto & i : mSystems )
		{
			i.second.first->StopSystem();
		}

		// System are independant by themseleves. no need to shutdown in reverse order
		for ( auto & i : mSystems )
		{
			// Wait for thread to finish
			i.second.second->Join();
			i.second.first->Shutdown();
		}

		// No thread for main system, no need to stop it.
		mMainSystem->Shutdown();
	}

	void SystemManager::SystemLoop( System * system )
	{
		// While system is still running
		while ( !system->mStop )
		{
			system->Work();
		}
	}

	void SystemManager::Update( f32 dt )
	{
		// Alternating flag
		mUpdateFlag = !mUpdateFlag;

		for ( auto & i : mSystems )
		{
			i.second.first->AddJob( 0,
									BH::Delegate< void( BH::f32, bool ) >::FromMethod<BH::System, &BH::System::UpdateUpdatables>( i.second.first ),
								    dt, mUpdateFlag );
		}

		// Main system update while the rest of the system is at work
		mMainSystem->UpdateUpdatables( dt, mUpdateFlag );
		mMainSystem->Work();
	}

	void SystemManager::Draw( f32 dt )
	{
		for ( auto & i : mSystems )
		{
			// Since drawing is still single threaded... I'm not adding to job.
			//i.second.first->AddJob( 0,
			//						BH::Delegate<void(BH::f32)>::FromMethod<BH::System, &BH::System::DrawDrawables>( i.second.first ),
			//					    dt );
			i.second.first->DrawDrawables( dt );
		}

		// Main system draw
		mMainSystem->DrawDrawables( dt );
	}

	bool SystemManager::IsAllIdling() const
	{
		for ( const auto & i : mSystems )
		{
			if ( !i.second.first->IsIdling() )
				return false;
		}

		return true;
	}

	void SystemManager::PauseObjectComponents()
	{
		for ( auto & i : mSystems )
		{
			i.second.first->AddJob( UINT_MAX,
									BH::Delegate< void() >::FromMethod<BH::System, &BH::System::PauseObjectComponents>( i.second.first ) );
		}

		mMainSystem->PauseObjectComponents();
	}

	void SystemManager::ResumeObjectComponents()
	{
		for ( auto & i : mSystems )
		{
			i.second.first->AddJob( UINT_MAX,
									BH::Delegate< void() >::FromMethod<BH::System, &BH::System::ResumeObjectComponents>( i.second.first ) );
		}

		mMainSystem->ResumeObjectComponents();
	}

	void SystemManager::HideObjectComponents()
	{
		for ( auto & i : mSystems )
		{
			i.second.first->AddJob( UINT_MAX,
									BH::Delegate< void() >::FromMethod<BH::System, &BH::System::HideObjectComponents>( i.second.first ) );
		}

		mMainSystem->HideObjectComponents();
	}

	void SystemManager::ShowObjectComponents()
	{
		for ( auto & i : mSystems )
		{
			i.second.first->AddJob( UINT_MAX,
									BH::Delegate< void() >::FromMethod<BH::System, &BH::System::ShowObjectComponents>( i.second.first ) );
		}

		mMainSystem->ShowObjectComponents();
	}
}