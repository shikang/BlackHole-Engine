// Precompiled Header
#include "Core/Stdafx.h"

#include "Platform/FileSystem.h"

#include "Core/MeshManager.h"

#include <utility>

namespace BH
{
	String MeshManager::LoadDirectory = "";

	MeshManager::MeshManager()
	{
	}

	MeshManager::~MeshManager()
	{
	}

	void MeshManager::Initialise()
	{
		LOG("Initialising Mesh Manager...");
		LoadAllMeshes();
		LOG("Mesh Manager Initialised");
	}

	void MeshManager::Shutdown()
	{
		for( auto & i : mMeshList )
		{
			i.second->Shutdown();
			delete i.second;
		}

		mMeshList.clear();
	}

	Mesh * MeshManager::GetMesh( const String & name ) const
	{
		MeshList::const_iterator it = mMeshList.find( name );
		if ( it == mMeshList.end() )
			return nullptr;

		return it->second;
	}

	AABB MeshManager::GetAABB( const String & name ) const
	{
		AABBList::const_iterator it = mAABBList.find( name );
		if ( it == mAABBList.end() )
			return AABB();

		return it->second;
	}

	void MeshManager::LoadAllMeshes()
	{
		FileInfo::List files = FileSystem::GetFileList( LoadDirectory.c_str(), true );

		for ( const auto & i : files )
		{
			// Only load fbx for now
			if ( i.GetExtension() != "fbx" )
				continue;

			Mesh * mesh = new Mesh();
			AABB aabb;
			mMeshLoader.LoadMeshFromFile( LoadDirectory + i.GetFullFile(), *mesh, aabb );
			mMeshList.insert( std::make_pair( i.GetFileNoExt(), mesh ) );
			mAABBList.insert( std::make_pair( i.GetFileNoExt(), aabb ) );
		}
	}
}