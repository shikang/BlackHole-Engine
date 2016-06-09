#ifndef BH_COLLISION_MANAGER_H
#define BH_COLLISION_MANAGER_H

#include "Core/Event.h"
#include "Core/CoreEssential.h"
#include "Core/FunctionsFramework.h"
#include "Core/IUpdatable.h"
#include "Core/GameComponent.h"
#include "Core/SystemManager.h"
#include "Core/OBB.h"
#include "Core/Contact.h"

#include "Platform/Matrix4.h"
#include "Platform/Vector2f.h"
#include "Platform/Vector3f.h"

namespace BH
{
	class CollisionManager : public GameComponent, public IUpdatable
	{
	public:
		struct CollisionInfo
		{
			bool mPassable;
			String mTag;
			OBB mOBB;
		};

		typedef std::unordered_map< String, CollisionInfo > OBBList;
		typedef OBBList StaticList;
		typedef OBBList MovingList;
		typedef std::unordered_map< String, Vector3f > VelocityList;

		typedef std::vector< Manifold >	ManifoldList;
		typedef ManifoldList StaticManifoldList;
		typedef ManifoldList MovingManifoldList;

	public:
		GETTER_AUTO_CREF( ManifoldList, StaticManifoldList );
		GETTER_AUTO_CREF( ManifoldList, MovingManifoldList );

	public:
		// Constructor
		CollisionManager();

		// Destructor
		~CollisionManager();

		// Initialise
		void Initialise() override;

		// Shutdown
		void Shutdown() override;

		// Update
		void Update( f32 dt ) override;

	public:
		// Add to Static List
		void AddToStaticList( const String & name, OBB obb, bool passable, const Char * tag );

		// Update Static Object 
		void UpdateStaticObject( const String & name, OBB obb, bool passable, const Char * tag );

		// Get Static Object Box
		const OBB * GetStaticObject( const String & name ) const;

		// Remove from static list
		void RemoveFromStaticList( const String & name );

		// Add to Moving List
		void AddToMovingList( const String & name, OBB obb, bool passable, const Char * tag );

		// Update moving object
		void UpdateMovingObject( const String & name, OBB obb, bool passable, const Char * tag );

		// Get Moving Object Box
		const OBB * GetMovingObject( const String & name ) const;

		// Get Moving Object's velocity
		Vector3f GetMovingObjectVelocity( const String & name ) const;

		// Remove from Moving List
		void RemoveFromMovingList( const String & name );

		// Clear Static List
		void ClearStaticList();

		// CLear Moving List
		void ClearMovingList();

		// Clear all objects
		void ClearAllObjects();

	private:
		StaticList mStaticList;						//!< List of non-moving objects
		MovingList mMovingList;						//!< List of moving objects
		VelocityList mVelocities;					//!< List of moving object's velocities

		StaticManifoldList mStaticManifoldList;		//!< List of Static Manifold info
		MovingManifoldList mMovingManifoldList;		//!< List of Moving Manifold info

	};
}

#include "Core/Typing.h"
DECLARE_TYPE( ::BH::CollisionManager, CollisionManager );

#endif