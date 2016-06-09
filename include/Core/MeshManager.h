#ifndef BH_MESH_MANAGER_H
#define BH_MESH_MANAGER_H

#include "Core/CoreEssential.h"
#include "Core/FunctionsFramework.h"
#include "Core/GameComponent.h"
#include "Core/SystemManager.h"

#include "Platform/MeshLoader.h"

#include <unordered_map>

namespace BH
{
	class MeshManager : public GameComponent
	{
	public:
		typedef std::unordered_map< String, Mesh * > MeshList;
		typedef std::unordered_map< String, AABB > AABBList;

		GETTER_AUTO_CREF( MeshList, MeshList );

	public:
		// Load directories
		static String LoadDirectory;

	public:
		// Constructor
		MeshManager();

		// Destructor
		~MeshManager();

		// Initialise
		void Initialise() override;

		// Shutdown
		void Shutdown() override;

		// Get mesh
		Mesh * GetMesh( const String & name ) const;

		// Get AABB
		AABB GetAABB( const String & name ) const;

	private:
		// Load all meshes in the directory
		void LoadAllMeshes();

	private:
		MeshLoader mMeshLoader;
		MeshList mMeshList;
		AABBList mAABBList;

	};
}

#include "Core/Typing.h"
DECLARE_TYPE(::BH::MeshManager, MeshManager);

#endif