// Precompiled Header
#include "Core/Stdafx.h"

#include "Platform/FileSystem.h"
#include "Platform/Texture.h"

#include "Core/TextureManager.h"

#include <utility>

namespace BH
{
	String TextureManager::LoadDirectory = "";

	TextureManager::TextureManager()
	{
	}

	TextureManager::~TextureManager()
	{
	}

	void TextureManager::Initialise()
	{
		LOG("Initialising Texture Manager...");
		LoadAllTextures();
		LOG("Texture Manager Initialised");
	}

	void TextureManager::Shutdown()
	{
		for( auto & i : mTextureList )
		{
			i.second->Shutdown();
			delete i.second;
		}

		mTextureList.clear();
	}

	Texture * TextureManager::GetTexture( const String & name )
	{
		TextureList::iterator it = mTextureList.find( name );
		if ( it == mTextureList.end() )
			return nullptr;

		return it->second;
	}

	void TextureManager::LoadAllTextures()
	{
		FileInfo::List files = FileSystem::GetFileList( LoadDirectory.c_str(), true );

		for ( const auto & i : files )
		{
			// Only load png for now
			if ( i.GetExtension() != "png" )
				continue;

			Texture * texture = new Texture();
			texture->Initialise( LoadDirectory + i.GetFullFile() );
			mTextureList.insert( std::make_pair( i.GetFileNoExt(), texture ) );
		}
	}
}