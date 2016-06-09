// Precompiled Header
#include "Core/Stdafx.h"

#include "Platform/FileSystem.h"
#include "Platform/Texture.h"

#include "Core/HDRTextureManager.h"

#include <utility>

namespace BH
{
	String HDRTextureManager::LoadDirectory = "";

	HDRTextureManager::HDRTextureManager()
	{
	}

	HDRTextureManager::~HDRTextureManager()
	{
	}

	void HDRTextureManager::Initialise()
	{
		LOG("Initialising HDR Texture Manager...");
		LoadAllTextures();
		LOG("HDR Texture Manager Initialised");
	}

	void HDRTextureManager::Shutdown()
	{
		for( auto & i : mTextureList )
		{
			i.second.first->Shutdown();
			delete i.second.first;

			i.second.second->Shutdown();
			delete i.second.second;
		}

		mTextureList.clear();
	}

	HDRTextureManager::HDRPair HDRTextureManager::GetTexture( const String & name )
	{
		HDRTextureList::iterator it = mTextureList.find( name );
		if ( it == mTextureList.end() )
			return HDRPair( nullptr, nullptr );

		return it->second;
	}

	void HDRTextureManager::LoadAllTextures()
	{
		FileInfo::List files = FileSystem::GetFileList( LoadDirectory.c_str(), false );

		for ( const auto & i : files )
		{
			// Only load hdr for now
			if ( i.GetExtension() != "hdr" )
				continue;

			HDRTexture * texture = new HDRTexture();
			HDRTexture * irradiance = new HDRTexture();
			texture->Initialise( LoadDirectory + i.GetFullFile() );
			irradiance->Initialise( LoadDirectory + "Irradiance/" + i.GetFileNoExt() + "_Irradiance." + i.GetExtension() );
			mTextureList.insert( std::make_pair( i.GetFileNoExt(), std::make_pair( texture, irradiance ) ) );
		}
	}
}