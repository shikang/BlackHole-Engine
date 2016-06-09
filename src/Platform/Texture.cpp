// Precompiled Header
#include "Platform/Stdafx.h"

#include "Platform/Texture.h"

#include "Platform/WICFunctions.h"

#include <string>
#include <codecvt>
#include <locale>

namespace BH
{
	Texture::Texture()
	: mTextureView( nullptr )
	, mTexture( nullptr )
	{
	}

	Texture::~Texture()
	{

	}

	void Texture::Initialise( const String & filename )
	{
		std::wstring_convert< std::codecvt_utf8_utf16< wchar_t >> converter;
		std::wstring wFileName = converter.from_bytes( filename );
		CreateTextureFromFile( wFileName.c_str() );
	}

	void Texture::Shutdown()
	{
		if ( mTexture )
		{
			mTexture->Release();
			mTexture = nullptr;
		}

		if ( mTextureView )
		{
			mTextureView->Release();
			mTextureView = nullptr;
		}
	}

	void Texture::CreateTextureFromFile( const WCHAR * filename )
	{
		CreateWICTextureFromFile( GRAPHICS.GetPipeline()->GetDevice(),
								  filename,
								  &mTexture,
								  &mTextureView,
								  0 );
	}
}