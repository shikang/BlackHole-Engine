// Precompiled Header
#include "Platform/Stdafx.h"

#include "Platform/DepthRenderTarget.h"
#include "Platform/Graphics.h"

namespace BH
{
	DepthRenderTarget::DepthRenderTarget()
	: mShaderResourceView( nullptr )
	, mRenderTargetView( nullptr )
	, mRenderTargetTexture( nullptr )
	, mDepthStencilBuffer( nullptr )
	, mDepthStencilView( nullptr )
	, mWidth( 0 )
	, mHeight( 0 )
	{
	}

	DepthRenderTarget::~DepthRenderTarget()
	{
	}

	void DepthRenderTarget::Initialise( u32 width, u32 height )
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

		// Initialize the description of the depth buffer.
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory( &depthBufferDesc, sizeof( depthBufferDesc ) );

		// Set up the description of the depth buffer.
		depthBufferDesc.Width = width;
		depthBufferDesc.Height = height;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		// Create the texture for the depth buffer using the filled out description.
		result = device->CreateTexture2D( &depthBufferDesc, NULL, &mDepthStencilBuffer );
		if ( FAILED( result ) )
		{
			return;
		}

		// Initailze the depth stencil view description.
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory( &depthStencilViewDesc, sizeof( depthStencilViewDesc ) );

		// Set up the depth stencil view description.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// Create the depth stencil view.
		result = device->CreateDepthStencilView( mDepthStencilBuffer, &depthStencilViewDesc, &mDepthStencilView );
		if ( FAILED( result ) )
		{
			return;
		}

		// Setup the viewport for rendering.
		mViewport.Width = static_cast<f32>( width );
		mViewport.Height = static_cast<f32>( height );
		mViewport.MinDepth = 0.0f;
		mViewport.MaxDepth = 1.0f;
		mViewport.TopLeftX = 0.0f;
		mViewport.TopLeftY = 0.0f;

		// Setup the projection matrix.
		//D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, ((float)D3DX_PI / 4.0f), ((float)textureWidth / (float)textureHeight), screenNear, screenDepth);

		// Create an orthographic projection matrix for 2D rendering.
		//D3DXMatrixOrthoLH(&m_orthoMatrix, (float)textureWidth, (float)textureHeight, screenNear, screenDepth);

		mWidth = width;
		mHeight = height;
	}

	void DepthRenderTarget::Shutdown()
	{
		if ( mDepthStencilView )
		{
			mDepthStencilView->Release();
			mDepthStencilView = nullptr;
		}

		if ( mDepthStencilBuffer )
		{
			mDepthStencilBuffer->Release();
			mDepthStencilBuffer = nullptr;
		}

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

	void DepthRenderTarget::SetRenderTarget()
	{
		// Bind the render target view and depth stencil buffer to the output render pipeline.
		//GRAPHICS.GetPipeline()->GetContext()->OMSetRenderTargets( 1, &mRenderTargetView, GRAPHICS.GetPipeline()->GetDepthStencilView() );
		GRAPHICS.GetPipeline()->GetContext()->OMSetRenderTargets( 1, &mRenderTargetView, mDepthStencilView );

		// Set the viewport.
		GRAPHICS.GetPipeline()->GetContext()->RSSetViewports( 1, &mViewport );
	}

	void DepthRenderTarget::ClearRenderTarget( const Vector4f & color )
	{
		f32 targetColor[4] = { color.x, color.y, color.z, color.w };

		ID3D11DeviceContext* deviceContext = GRAPHICS.GetPipeline()->GetContext();

		// Clear the back buffer.
		deviceContext->ClearRenderTargetView( mRenderTargetView, targetColor );

		// Clear the depth buffer.
		deviceContext->ClearDepthStencilView( mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
	}

	ID3D11ShaderResourceView * DepthRenderTarget::GetShaderResourceView()
	{
		return mShaderResourceView;
	}

	u32 DepthRenderTarget::GetWidth() const
	{
		return mWidth;
	}

	u32 DepthRenderTarget::GetHeight() const
	{
		return mHeight;
	}
}