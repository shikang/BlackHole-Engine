// Precompiled Header
#include "Platform/Stdafx.h"

#include "Platform/UAVTexture.h"
#include "Platform/Graphics.h"

namespace BH
{
	UAVTexture::UAVTexture()
	: mUnorderedAccessView( nullptr )
	, mShaderResourceView( nullptr )
	, mTexture( nullptr )
	, mWidth( 0 )
	, mHeight( 0 )
	{
	}

	UAVTexture::~UAVTexture()
	{
	}

	void UAVTexture::Initialise( u32 width, u32 height )
	{
		D3D11_TEXTURE2D_DESC textureDesc;
		
		// Initialize the render target texture description.
		ZeroMemory( &textureDesc, sizeof( textureDesc ) );

		// Setup the render target texture description.
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		// Create the render target texture.
		ID3D11Device * device = GRAPHICS.GetPipeline()->GetDevice();
		HRESULT result = device->CreateTexture2D( &textureDesc, NULL, &mTexture );
		if ( FAILED( result ) )
		{
			return;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

		// Setup the description of the shader resource view.
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		// Create the shader resource view.
		result = device->CreateShaderResourceView( mTexture, &shaderResourceViewDesc, &mShaderResourceView );
		if ( FAILED( result ) )
		{
			return;
		}

		D3D11_UNORDERED_ACCESS_VIEW_DESC UAVdesc;
        ZeroMemory( &UAVdesc, sizeof( UAVdesc ) );
		UAVdesc.Format = textureDesc.Format;
        UAVdesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
        UAVdesc.Texture2D.MipSlice=0;
		result = device->CreateUnorderedAccessView( mTexture, &UAVdesc, &mUnorderedAccessView );

		mWidth = width;
		mHeight = height;
	}

	void UAVTexture::Shutdown()
	{
		if ( mUnorderedAccessView )
		{
			mUnorderedAccessView->Release();
			mUnorderedAccessView = nullptr;
		}

		if ( mShaderResourceView )
		{
			mShaderResourceView->Release();
			mShaderResourceView = nullptr;
		}

		if ( mTexture )
		{
			mTexture->Release();
			mTexture = nullptr;
		}
	}

	ID3D11UnorderedAccessView * UAVTexture::GetUnorderedAccessView()
	{
		return mUnorderedAccessView;
	}

	ID3D11ShaderResourceView * UAVTexture::GetShaderResourceView()
	{
		return mShaderResourceView;
	}

	u32 UAVTexture::GetWidth() const
	{
		return mWidth;
	}

	u32 UAVTexture::GetHeight() const
	{
		return mHeight;
	}
}