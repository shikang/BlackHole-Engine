#ifndef BH_COLLISION_SOLVER_H
#define BH_COLLISION_SOLVER_H

#include "Platform/PlatformWindow.h"

#include "Core/CoreEssential.h"
#include "Core/FunctionsFramework.h"
#include "Core/IUpdatable.h"
#include "Core/GameComponent.h"

#include "Platform/Matrix4.h"
#include "Platform/Vector2f.h"
#include "Platform/Vector3f.h"

namespace BH
{
	class CollisionSolver : public GameComponent, public IUpdatable
	{
	public:
		// Constructor
		CollisionSolver();

		// Destructor
		~CollisionSolver();

		// Initialise
		void Initialise() override;

		// Shutdown
		void Shutdown() override;

		// Update
		void Update( f32 dt ) override;

	};
}

#include "Core/Typing.h"
DECLARE_TYPE( ::BH::CollisionSolver, CollisionSolver );

#endif