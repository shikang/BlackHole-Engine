// Precompiled Header
#include "Platform/Stdafx.h"

#include "Platform/HDRTexture.h"

#include "Dependencies/rgbe.h"

#include <string>
#include <codecvt>
#include <locale>
#include <cstdio>
#include <memory>
#include <cmath>

namespace BH
{
	HDRTexture::HDRTexture()
	: mTextureView( nullptr )
	, mTexture( nullptr )
	, mData()
	, mWidth( 0 )
	, mHeight( 0 )
	, mExposure( 1.0f )
	, mGamma( 1.0f )
	{
	}

	HDRTexture::~HDRTexture()
	{

	}

	void HDRTexture::Initialise( const String & filename )
	{
		std::wstring_convert< std::codecvt_utf8_utf16< wchar_t >> converter;
		std::wstring wFileName = converter.from_bytes( filename );
		LoadHDRTexture( wFileName.c_str() );
		CreateTextureFromData();
	}

	void HDRTexture::Shutdown()
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

		mData.clear();
	}

	void HDRTexture::CreateTextureFromData()
	{
		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = mData.data();
		initData.SysMemPitch = static_cast<UINT>( mWidth * 4 * sizeof( f32 ) );
		initData.SysMemSlicePitch = static_cast<UINT>( mHeight );

		D3D11_TEXTURE2D_DESC textureDesc;
		
		// Initialize the render target texture description.
		ZeroMemory( &textureDesc, sizeof( textureDesc ) );

		// Setup the render target texture description.
		textureDesc.Width = mWidth;
		textureDesc.Height = mHeight;
		textureDesc.MipLevels = static_cast< UINT >( log10( static_cast< f32 >( mWidth ) ) / log10( 2.0f ) );
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET  | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		ID3D11Device * device = GRAPHICS.GetPipeline()->GetDevice();
		//HRESULT result = device->CreateTexture2D( &textureDesc, &initData, &mTexture );
		HRESULT result = device->CreateTexture2D( &textureDesc, nullptr, &mTexture );
		if ( FAILED( result ) )
		{
			return;
		}

		D3D11_BOX destBox;
		destBox.left = 0;
		destBox.right = mWidth;
		destBox.top = 0;
		destBox.bottom = mHeight;
		destBox.front = 0;
		destBox.back = 1;

		ID3D11DeviceContext * context = GRAPHICS.GetPipeline()->GetContext();
		context->UpdateSubresource( mTexture,											// the texture to update
									0,													// first mip-level
									&destBox,											// position of the pixels to update in the texture
									mData.data(),										// image data
									static_cast<UINT>( mWidth * 4 * sizeof( f32 ) ),	// row pitch
									0													// not used for 2D textures
									);

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		// Setup the description of the shader resource view.
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = textureDesc.MipLevels;

		// Create the shader resource view.
		result = device->CreateShaderResourceView( mTexture, &shaderResourceViewDesc, &mTextureView );

		context->GenerateMips( mTextureView );
	}

	void HDRTexture::LoadHDRTexture( const WCHAR * filename )
	{
		// Load file
		std::unique_ptr< FILE, s32(*)( FILE * ) > f( nullptr, fclose );	// Auto call fclose when function exits
		{
			FILE * fp;

			_wfopen_s( &fp, filename, L"rb" );

			if ( !fp )
				return;

			std::unique_ptr< FILE, s32(*)( FILE * ) > current( fp, fclose );
			f = std::move( current );
		}
		rgbe_header_info header;

		Char errbuf[100] = { '\0' };

		if ( RGBE_ReadHeader( f.get(), &mWidth, &mHeight, &header, errbuf ) )
		{
			CreateMessageBox( errbuf );
			return;
		}

		mExposure = header.exposure;
		mGamma = header.gamma;

		mData.resize( mWidth * mHeight * 4 , 0.0f );
		
		if ( RGBE_ReadPixels_RLE( f.get(), mData.data(), mWidth, mHeight, errbuf ) )
		{
			CreateMessageBox( errbuf );
			return;
		}

	}

	f32 HDRTexture::GetTextureDimension() const
	{
		return static_cast< f32 >( mWidth ) * static_cast< f32 >( mHeight );
	}
}