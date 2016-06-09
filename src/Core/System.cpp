// Precompiled Header
#include "Core/Stdafx.h"

#include "Core/System.h"
#include "Core/IUpdatable.h"
#include "Core/IDrawable.h"
#include "Core/GameComponent.h"
#include "Core/ObjectComponent.h"
#include "Core/LogManager.h"

#include <algorithm>

namespace BH
{
	System::System( const BH::Char * name )
	: mLastFlag( false )
	, mStop( false )
	, mSystemName( name )
	{
	}

	System::~System()
	{
	}

	void System::StopSystem()
	{
		mStop = true;
	}

	void System::ProcessGameComponent()
	{
		// While there are game components not yet added
		while ( !mGameComponentProcessMap.empty() )
		{
			// Remove non-dependant game component
			std::vector<CName> removeList;
			for ( const auto & i : mGameComponentProcessMap )
			{
				if ( i.second == 0 )
				{
					removeList.push_back( i.first );

					// Add Game Component to the back of the list
					GameComponentMap::iterator it = mGameComponentMap.find( i.first );
					if ( it != mGameComponentMap.end() )
					{
						mGameComponentInitialiseList.push_back( it->second );
						LOGINFO( i.first.Text << " added to back of initialise list!" );
					}
				}
			}

			// Remove processed component
			for ( const auto & i : removeList )
			{
				mGameComponentProcessMap.erase(i);

				// Update dependency tree
				GameComponentDependentMap::iterator it = mGameComponentDependentMap.find( i );
				if ( it != mGameComponentDependentMap.end() )
				{
					for ( const auto & j : it->second )
					{
						// Minus 1 dependency count ( 0 = no longer dependant )
						GameComponentProcessMap::iterator it = mGameComponentProcessMap.find( j );
						if ( it != mGameComponentProcessMap.end() )
							--( it->second );
					}
				}
			}
		}

		// Prepare update run list
		for ( auto & i : mUpdatableList )
		{
			if( i->GetEnabled() )
				mUpdatableRunList.push_back( i );
		}

		// Prepare draw run list
		for ( auto & i : mDrawableList )
		{
			if( i->GetVisible() )
				mDrawableRunList.push_back( i );
		}
	}

	void System::Initialise()
	{
		ProcessGameComponent();

		for ( auto & i : mGameComponentInitialiseList )
			i->Initialise();
	}

	void System::Work()
	{
		mJobQueue.Work();
	}

	void System::Shutdown()
	{
		// Shutdown in reverse
		for ( GameComponentInitialiseList::reverse_iterator i = mGameComponentInitialiseList.rbegin(); 
			  i != mGameComponentInitialiseList.rend(); ++i )
			 ( *i )->Shutdown();
	}

	void System::Dependency( const Char * dependee, const Char * dependent )
	{
		CName cDependee( dependee );
		CName cDependent( dependent );

		GameComponentDependeeList & dependeeList = mGameComponentDependentMap[cDependent];
		GameComponentDependeeList::iterator it = std::find( dependeeList.begin(), dependeeList.end(), cDependee );

		// If dependee already added
		if ( it != dependeeList.end() )
		{
			LOGERROR("Dependee: " << dependee << " dependency with Dependent: " << dependent << " has been added!");
			return;
		}

		// Add dependency count
		++( mGameComponentProcessMap[cDependee] );
		dependeeList.push_back( cDependee );
	}

	void System::UpdateUpdatables( f32 dt, bool current_flag )
	{
		mLastFlag = current_flag;

		// Remove inactive
		RemoveInActive();

		// Insert active
		InsertActive( current_flag );

		// @cleanup
		//std::cout << mSystemName << ": " << mUpdatableRunList.size() << std::endl;

		for ( auto & i : mUpdatableRunList )
		{
			//i->Update( dt );
			//i->SetFlip( current_flag );
			AddJob( i->GetUpdateOrder(), 
					Delegate< void( IUpdatable *, f32, bool ) >::FromFunction< &System::UpdateDelegate >(), 
					i, dt, current_flag );
		}
	}

	void System::SetEnabled( IUpdatable * updatable, bool enable )
	{
		if ( updatable->GetEnabled() == enable )
			return;

		BH_ASSERT( std::find( mUpdatableList.begin(), mUpdatableList.end(), updatable ) != mUpdatableList.end(),
				   "Updatable does not belong to this system!" );

		if ( enable )
			mUpdatableInsertList.push_back( updatable );
		else
			mUpdatableRemoveList.push_back( updatable );

		updatable->mEnabled = enable;
	}

	void System::UpdateDelegate( IUpdatable * updatable, f32 dt, bool current_flag )
	{
		updatable->Update( dt );
		updatable->SetFlip( current_flag );
	}

	void System::RemoveInActive()
	{
		for ( auto & i : mUpdatableRemoveList )
		{
			UpdateList::iterator it = std::find( mUpdatableRunList.begin(), mUpdatableRunList.end(), i );
			if ( it != mUpdatableRunList.end() )
				mUpdatableRunList.erase( it );
		}

		mUpdatableRemoveList.clear();
	}

	void System::InsertActive( bool flip )
	{
		for (auto & i : mUpdatableInsertList)
		{
			// Find the current position where element is bigger
			UpdateList::iterator it = std::find_if( mUpdatableRunList.begin(), mUpdatableRunList.end(),
													[=]( const IUpdatable * rhs )
													{
														return *i < *rhs;
													} );
			mUpdatableRunList.insert( it, i );
			// Set invalid flip status
			i->SetFlip(!flip);
		}

		mUpdatableInsertList.clear();
	}

	void System::DrawDrawables(f32 dt)
	{
		// Remove invisible
		RemoveInVisible();

		// Insert visible
		InsertVisible();

		for ( auto & i : mDrawableRunList )
		{
			// Since drawing is still single thread... (I'm not adding job)
			i->Draw(dt);
		}
	}

	void System::SetVisible( IDrawable * drawable, bool visible )
	{
		if ( drawable->GetVisible() == visible )
			return;

		BH_ASSERT( std::find( mDrawableList.begin(), mDrawableList.end(), drawable ) != mDrawableList.end(),
				   "Drawable does not belong to this system!" );

		if ( visible )
			mDrawableInsertList.push_back( drawable );
		else
			mDrawableRemoveList.push_back( drawable );

		drawable->mVisible = visible;
	}

	void System::RemoveInVisible()
	{
		for ( auto & i : mDrawableRemoveList )
		{
			DrawList::iterator it = std::find( mDrawableRunList.begin(), mDrawableRunList.end(), i );
			if ( it != mDrawableRunList.end() )
				mDrawableRunList.erase( it );
		}

		mDrawableRemoveList.clear();
	}

	void System::InsertVisible()
	{
		for ( auto & i : mDrawableInsertList )
		{
			// Find the current position where element is bigger
			DrawList::iterator it = std::find_if( mDrawableRunList.begin(), mDrawableRunList.end(), 
													[=]( const IDrawable * rhs )
													{
														return *i < *rhs;
													} );
			mDrawableRunList.insert( it, i );
		}

		mDrawableInsertList.clear();
	}

	void System::PauseObjectComponents()
	{
		// If already paused
		if ( !mPausingList.empty() )
			return;

		for ( auto & updatable : mUpdatableRunList )
		{
			if ( dynamic_cast< ObjectComponent * >( updatable ) != nullptr )
			{
				mPausingList.push_back( updatable );
				SetEnabled( updatable, false );
			}
		}
	}

	void System::ResumeObjectComponents()
	{
		for ( auto & updatable : mPausingList )
		{
			SetEnabled( updatable, true );
		}

		mPausingList.clear();
	}

	void System::HideObjectComponents()
	{
		// If already hide
		if ( !mHidingList.empty() )
			return;

		for ( auto & drawable : mDrawableRunList )
		{
			if ( dynamic_cast< ObjectComponent * >( drawable ) != nullptr )
			{
				mHidingList.push_back( drawable );
				SetVisible( drawable, false );
			}
		}
	}

	void System::ShowObjectComponents()
	{
		for ( auto & drawable : mHidingList )
		{
			SetVisible( drawable, true );
		}

		mHidingList.clear();
	}
}