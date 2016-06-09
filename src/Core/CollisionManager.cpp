// Precompiled
#include "Core/Stdafx.h"

#include "Core/CollisionManager.h"

#include "Core/CollisionFunctions.h"

namespace BH
{
	CollisionManager::CollisionManager()
	: IUpdatable( 1 )
	{
	}

	CollisionManager::~CollisionManager()
	{
	}

	void CollisionManager::Initialise()
	{
	}

	void CollisionManager::Shutdown()
	{
		ClearAllObjects();
	}

	void CollisionManager::Update( f32 )
	{
		mStaticManifoldList.clear();
		mMovingManifoldList.clear();

		// Test for all static object
		for ( auto & i : mMovingList )
		{
			for ( auto & j : mStaticList )
			{
				Manifold m;
				if( OBBToOBB( m, i.second.mOBB, j.second.mOBB, i.second.mPassable || j.second.mPassable ) )
				{
					m.mLHS = i.first;
					m.mRHS = j.first;
					m.mLHSTag = i.second.mTag;
					m.mRHSTag = j.second.mTag;
					mStaticManifoldList.push_back( m );
				}
			}
		}

		// Test for all moving object
		MovingList::iterator next = mMovingList.begin();

		MovingList::iterator current;
		if ( next != mMovingList.end() )
			current = next++;
		else
			current = next;

		while ( next != mMovingList.end() )
		{
			Manifold m;
			if ( OBBToOBB( m, current->second.mOBB, next->second.mOBB, current->second.mPassable || next->second.mPassable ) )
			{
				m.mLHS = current->first;
				m.mRHS = next->first;
				m.mLHSTag = current->second.mTag;
				m.mRHSTag = next->second.mTag;
				mMovingManifoldList.push_back( m );
			}

			current = next++;
		}
	}

	void CollisionManager::AddToStaticList( const String & name, OBB obb, bool passable, const Char * tag )
	{
		if ( mStaticList.find( name ) == mStaticList.end() )
			mStaticList[name] = CollisionInfo{ passable, tag, obb };
	}

	void CollisionManager::UpdateStaticObject( const String & name, OBB obb, bool passable, const Char * tag )
	{
		if ( mStaticList.find( name ) != mStaticList.end() )
			mStaticList[name] = CollisionInfo{ passable, tag, obb };
	}

	const OBB * CollisionManager::GetStaticObject( const String & name ) const
	{
		StaticList::const_iterator it = mStaticList.find( name );
		if ( it != mStaticList.cend() )
			return &( it->second.mOBB );

		return nullptr;
	}

	void CollisionManager::RemoveFromStaticList( const String & name )
	{
		StaticList::iterator it = mStaticList.find( name );

		if ( it != mStaticList.end() )
			mStaticList.erase( it );
	}

	void CollisionManager::AddToMovingList( const String & name, OBB obb, bool passable, const Char * tag )
	{
		if ( mMovingList.find( name ) == mMovingList.end() )
		{
			mMovingList[name] = CollisionInfo{ passable, tag, obb };
			mVelocities[name] = Vector3f::ZERO;
		}
	}

	void CollisionManager::UpdateMovingObject( const String & name, OBB obb, bool passable, const Char * tag )
	{
		if ( mMovingList.find( name ) != mMovingList.end() )
		{
			mVelocities[name] = obb.mPosition - mMovingList[name].mOBB.mPosition;
			mMovingList[name] = CollisionInfo{ passable, tag, obb };
		}
	}

	const OBB * CollisionManager::GetMovingObject( const String & name ) const
	{
		MovingList::const_iterator it = mMovingList.find( name );
		if ( it != mMovingList.cend() )
			return &( it->second.mOBB );

		return nullptr;
	}

	Vector3f CollisionManager::GetMovingObjectVelocity( const String & name ) const
	{
		VelocityList::const_iterator it = mVelocities.find( name );
		if ( it != mVelocities.cend() )
			return it->second;

		return Vector3f::ZERO;
	}

	void CollisionManager::RemoveFromMovingList( const String & name )
	{
		MovingList::iterator it = mMovingList.find( name );

		if ( it != mMovingList.end() )
		{
			mMovingList.erase( it );
			mVelocities.erase( name );
		}
	}

	void CollisionManager::ClearStaticList()
	{
		mStaticList.clear();
	}
	
	void CollisionManager::ClearMovingList()
	{
		mMovingList.clear();
	}
	
	void CollisionManager::ClearAllObjects()
	{
		ClearStaticList();
		ClearMovingList();
	}
}