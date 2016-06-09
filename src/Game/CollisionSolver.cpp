// Precompiled
#include "Core/Stdafx.h"

#include "Game/CollisionSolver.h"
#include "Game/ScriptLoader.h"

#include "Core/CollisionManager.h"

namespace BH
{
	CollisionSolver::CollisionSolver()
	: IUpdatable( 2 )
	{
	}

	CollisionSolver::~CollisionSolver()
	{
	}

	void CollisionSolver::Initialise()
	{
	}

	void CollisionSolver::Shutdown()
	{
	}

	void CollisionSolver::Update( f32 )
	{
		CollisionManager * collisionManager = SYSTEM_MANAGER.GetGameComponentFromSystem<CollisionManager>();
		ScriptLoader * scriptLoader = SYSTEM_MANAGER.GetGameComponentFromSystem<ScriptLoader>();

		const CollisionManager::ManifoldList & staticManifolds = collisionManager->GetStaticManifoldList();

		for ( const auto & manifold : staticManifolds )
		{
			if ( manifold.mNumContact != 0 )
			{
				// Reponse for snap back
				f32 penetration = FLT_MAX;
				for ( s32 i = 0; i < manifold.mNumContact; ++i )
				{
					penetration = std::min( penetration, manifold.mContacts[i].mPenetration );
					//std::cout << "Penetration " << i << ": " << manifold.mContacts[i].mPenetration << std::endl;
				}

				//std::cout << "Final Penetration: " << penetration << std::endl;

				// Without velocity
				Vector3f snapback = manifold.mNormal * penetration;

				//const OBB * obb = collisionManager->GetMovingObject( manifold.mLHS );
				//std::cout << "Before: " << obb->mPosition.x << ", " << obb->mPosition.y << ", " << obb->mPosition.z << ", " << std::endl;

				// With velocity
				Vector3f velocity = collisionManager->GetMovingObjectVelocity( manifold.mLHS );
				//std::cout << "Before velocity: " << velocity.x << ", " << velocity.y << ", " << velocity.z << ", " << std::endl;
				if ( velocity.LengthSq() > FLT_EPSILON )	// Non zero vector
				{
					velocity.Normalize();
					f32 cosA = manifold.mNormal.Dot( velocity );

					// If gradient is greater than 45 degree
					if ( cosA > 0.3f + FLT_EPSILON )
						snapback = ( penetration / cosA ) * velocity;
				}
				else
				{
					//std::cout << "------------------------------" << std::endl;
					continue;
				}

				ScriptComponent * scriptComponent = scriptLoader->GetScriptByObjectAndType( manifold.mLHS, "TransformComponent" );
				if ( scriptComponent && snapback.LengthSq() > FLT_EPSILON )
					scriptComponent->CallScriptFunction( "Snapback", snapback.x, snapback.y, snapback.z );

				// Update OBB after snap back
				scriptComponent = ( scriptComponent && snapback.LengthSq() > FLT_EPSILON ) ? scriptLoader->GetScriptByObjectAndType( manifold.mLHS, "BoxCollider" ) : nullptr;
				if ( scriptComponent )
					scriptComponent->CallScriptFunction( "Update", 0.0f );

				//obb = collisionManager->GetMovingObject( manifold.mLHS );
				//std::cout << "After: " << obb->mPosition.x << ", " << obb->mPosition.y << ", " << obb->mPosition.z << ", " << std::endl;
				//std::cout << "------------------------------" << std::endl;
				//Vector3f v = collisionManager->GetMovingObjectVelocity( manifold.mLHS );
				//std::cout << "After velocity: " << v.x << ", " << v.y << ", " << v.z << ", " << std::endl;
				//std::cout << "------------------------------" << std::endl;
			}

			ScriptComponent * scriptComponent = scriptLoader->GetScriptByObjectAndType( manifold.mLHS, "BoxCollider" );
			if ( scriptComponent )
				scriptComponent->CallScriptFunction( "CallOnCollision", manifold.mRHSTag.c_str() );

			// Update OBB after snap back
			scriptComponent = scriptLoader->GetScriptByObjectAndType( manifold.mRHS, "StationaryBoxCollider" );
			if ( scriptComponent )
				scriptComponent->CallScriptFunction( "CallOnCollision", manifold.mLHSTag.c_str() );
		}
	}
}