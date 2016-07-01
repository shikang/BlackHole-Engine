// Precompiled Header
#include "Core/Stdafx.h"

#include "Platform/FileSystem.h"

#include "Core/ModelManager.h"
#include "Core/Model.h"

#include <utility>

namespace BH
{
	String ModelManager::LoadDirectory = "";

	ModelManager::ModelManager()
	{
	}

	ModelManager::~ModelManager()
	{
	}

	void ModelManager::Initialise()
	{
		LOG( "Initialising Model Manager..." );
		LoadAllModels();
		LOG( "Model Manager Initialised" );
	}

	void ModelManager::Shutdown()
	{
		for ( auto & i : mModelList )
		{
			i.second->Shutdown();
			delete i.second;
		}

		mModelList.clear();
	}

	Model * ModelManager::GetModel( const String & name ) const
	{
		ModelList::const_iterator it = mModelList.find( name );
		if ( it == mModelList.end() )
			return nullptr;

		return it->second;
	}

	AABB ModelManager::GetAABB( const String & name ) const
	{
		AABBList::const_iterator it = mAABBList.find( name );
		if ( it == mAABBList.end() )
			return AABB();

		return it->second;
	}

	void ModelManager::LoadAllModels()
	{
		FileInfo::List files = FileSystem::GetFileList( LoadDirectory.c_str(), true );

		for ( const auto & i : files )
		{
			// Only load fbx for now
			if ( i.GetExtension() != "fbx" )
				continue;

			Model * model = new Model();
			AABB aabb;
			std::vector<Bone> bones;
			mMeshLoader.LoadMeshFromFile( LoadDirectory + i.GetFullFile(), model->mMesh, aabb, bones, model->mAnimations );
			model->mSkeleton.Initialise( std::move( bones ) );
			mModelList.insert( std::make_pair( i.GetFileNoExt(), model ) );
			mAABBList.insert( std::make_pair( i.GetFileNoExt(), aabb ) );
		}
	}
}