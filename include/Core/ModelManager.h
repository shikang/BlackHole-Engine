#ifndef BH_MODEL_MANAGER_H
#define BH_MODEL_MANAGER_H

#include "Core/CoreEssential.h"
#include "Core/FunctionsFramework.h"
#include "Core/GameComponent.h"
#include "Core/SystemManager.h"

#include "Platform/MeshLoader.h"

#include <unordered_map>

namespace BH
{
	class ModelManager : public GameComponent
	{
	public:
		typedef std::unordered_map< String, Model * > ModelList;
		typedef std::unordered_map< String, AABB > AABBList;

		GETTER_AUTO_CREF( ModelList, ModelList );

	public:
		// Load directories
		static String LoadDirectory;

	public:
		// Constructor
		ModelManager();

		// Destructor
		~ModelManager();

		// Initialise
		void Initialise() override;

		// Shutdown
		void Shutdown() override;

		// Get model
		Model * GetModel( const String & name ) const;

		// Get AABB
		AABB GetAABB( const String & name ) const;

	private:
		// Load all models in the directory
		void LoadAllModels();

	private:
		MeshLoader mMeshLoader;
		ModelList mModelList;
		AABBList mAABBList;

	};
}

#include "Core/Typing.h"
DECLARE_TYPE(::BH::ModelManager, ModelManager);

#endif