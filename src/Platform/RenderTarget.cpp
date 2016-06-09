// Precompiled Header
#include "Platform/Stdafx.h"

#include "Platform/RenderTarget.h"
#include "Platform/Graphics.h"

namespace BH
{
	RenderTarget::RenderTarget()
	: mShaderResourceView( nullptr )
	, mRenderTargetView( nullptr )
	, mRenderTargetTexture( nullptr )
	, mWidth( 0 )
	, mHeight( 0 )
	{
	}

	RenderTarget::~RenderTarget()
	{
	}

	void RenderTarget::Initialise( u32 width, u32 height )
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
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		// Create the render target texture.
		ID3D11Device * device = GRAPHICS.GetPipeline()->GetDevice();
		HRESULT result = device->CreateTexture2D( &textureDesc, NULL, &mRenderTargetTexture );
		if ( FAILED( result ) )
		{
			return;
		}

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;

		// Setup the description of the render target view.
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		// Create the render target view.
		result = device->CreateRenderTargetView( mRenderTargetTexture, &renderTargetViewDesc, &mRenderTargetView );
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
		result = device->CreateShaderResourceView( mRenderTargetTexture, &shaderResourceViewDesc, &mShaderResourceView );

		mWidth = width;
		mHeight = height;
	}

	void RenderTarget::Shutdown()
	{
		if ( mShaderResourceView )
		{
			mShaderResourceView->Release();
			mShaderResourceView = nullptr;
		}

		if ( mRenderTargetView )
		{
			mRenderTargetView->Release();
			mRenderTargetView = nullptr;
		}

		if ( mRenderTargetTexture )
		{
			mRenderTargetTexture->Release();
			mRenderTargetTexture = nullptr;
		}
	}

	void RenderTarget::SetRenderTarget()
	{
		// Bind the render target view and depth stencil buffer to the output render pipeline.
		GRAPHICS.GetPipeline()->GetContext()->OMSetRenderTargets( 1, &mRenderTargetView, GRAPHICS.GetPipeline()->GetDepthStencilView() );
	}

	void RenderTarget::ClearRenderTarget( const Vector4f & color )
	{
		f32 targetColor[4] = { color.x, color.y, color.z, color.w };

		ID3D11DeviceContext* deviceContext = GRAPHICS.GetPipeline()->GetContext();

		// Clear the back buffer.
		deviceContext->ClearRenderTargetView( mRenderTargetView, targetColor );

		// Clear the depth buffer.
		deviceContext->ClearDepthStencilView( GRAPHICS.GetPipeline()->GetDepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0 );
	}

	ID3D11ShaderResourceView * RenderTarget::GetShaderResourceView()
	{
		return mShaderResourceView;
	}

	u32 RenderTarget::GetWidth() const
	{
		return mWidth;
	}

	u32 RenderTarget::GetHeight() const
	{
		return mHeight;
	}
}