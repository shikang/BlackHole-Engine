// Precompiled Header
#include "Platform/Stdafx.h"

#include "Platform/MultiRenderTarget.h"
#include "Platform/Graphics.h"

namespace BH
{
	MultiRenderTarget::MultiRenderTarget()
	: mDepthStencilBuffer( nullptr )
	, mDepthStencilView( nullptr )
	, mWidth( 0 )
	, mHeight( 0 )
	{
	}

	MultiRenderTarget::~MultiRenderTarget()
	{
	}

	u32 MultiRenderTarget::GetRenderTargetsCount() const
	{
		return mRenderTargetTextures.size();
	}

	void MultiRenderTarget::Initialise( u32 count, u32 width, u32 height )
	{
		// Store the width and height of the render texture.
		mWidth = width;
		mHeight = height;

		// Initialize the render target texture description.
		D3D11_TEXTURE2D_DESC textureDesc;
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

		// Create the render target textures.
		ID3D11Device * device = GRAPHICS.GetPipeline()->GetDevice();
		mRenderTargetTextures.resize( count, nullptr );
		for( u32 i = 0; i < count; ++i )
		{
			HRESULT result = device->CreateTexture2D( &textureDesc, nullptr, mRenderTargetTextures.data() + i );
			if( FAILED( result ) )
			{
				return;
			}
		}

		// Setup the description of the render target view.
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		// Create the render target views.
		mRenderTargetViews.resize( count, nullptr );
		for( u32 i = 0; i < count; ++i )
		{
			HRESULT result = device->CreateRenderTargetView( mRenderTargetTextures[i], &renderTargetViewDesc, mRenderTargetViews.data() + i );
			if( FAILED( result ) )
			{
				return;
			}
		}

		// Setup the description of the shader resource view.
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;

		// Create the shader resource views.
		mShaderResourceViews.resize( count, nullptr );
		for( u32 i = 0; i < count; ++i )
		{
			HRESULT result = device->CreateShaderResourceView( mRenderTargetTextures[i], &shaderResourceViewDesc, mShaderResourceViews.data() + i );
			if( FAILED( result ) )
			{
				return;
			}
		}

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
		HRESULT result = device->CreateTexture2D( &depthBufferDesc, NULL, &mDepthStencilBuffer );
		if( FAILED( result ) )
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
		if( FAILED( result ) )
		{
			return;
		}

		// Setup the viewport for rendering.
		mViewport.Width = static_cast<float>( width );
		mViewport.Height = static_cast<float>( height );
		mViewport.MinDepth = 0.0f;
		mViewport.MaxDepth = 1.0f;
		mViewport.TopLeftX = 0.0f;
		mViewport.TopLeftY = 0.0f;
	}

	void MultiRenderTarget::Shutdown()
	{
		if ( mDepthStencilView )
		{
			mDepthStencilView->Release();
			mDepthStencilView = 0;
		}

		if ( mDepthStencilBuffer )
		{
			mDepthStencilBuffer->Release();
			mDepthStencilBuffer = 0;
		}

		u32 count = GetRenderTargetsCount();
		for ( u32 i = 0; i < count; ++i )
		{
			if ( mShaderResourceViews[i] )
			{
				mShaderResourceViews[i]->Release();
				mShaderResourceViews[i] = 0;
			}

			if ( mRenderTargetViews[i] )
			{
				mRenderTargetViews[i]->Release();
				mRenderTargetViews[i] = 0;
			}

			if ( mRenderTargetTextures[i] )
			{
				mRenderTargetTextures[i]->Release();
				mRenderTargetTextures[i] = 0;
			}
		}

		mShaderResourceViews.clear();
		mRenderTargetViews.clear();
		mRenderTargetTextures.clear();
	}

	void MultiRenderTarget::SetRenderTargets()
	{
		// Bind the render target view and depth stencil buffer to the output render pipeline.
		//GRAPHICS.GetPipeline()->GetContext()->OMSetRenderTargets( GetRenderTargetsCount(), mRenderTargetViews.data(), mDepthStencilView );
		GRAPHICS.GetPipeline()->GetContext()->OMSetRenderTargets( GetRenderTargetsCount(), mRenderTargetViews.data(), GRAPHICS.GetPipeline()->GetDepthStencilView() );

		// Set viewport
		//GRAPHICS.GetPipeline()->GetContext()->RSSetViewports( 1, &mViewport );
	}

	void MultiRenderTarget::ClearRenderTargets( const Vector4f & color )
	{
		f32 targetColor[4] = { color.x, color.y, color.z, color.w };

		ID3D11DeviceContext* deviceContext = GRAPHICS.GetPipeline()->GetContext();

		// Clear the render targets
		u32 count = GetRenderTargetsCount();
		for ( u32 i = 0; i < count; ++i )
		{
			deviceContext->ClearRenderTargetView( mRenderTargetViews[i], targetColor );
		}

		// Clear the depth buffer.
		deviceContext->ClearDepthStencilView( mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
	}

	ID3D11ShaderResourceView * MultiRenderTarget::GetShaderResourceView( u32 index )
	{
		if ( index > mShaderResourceViews.size() )
			return nullptr;

		return mShaderResourceViews[index];
	}

	Vector2f MultiRenderTarget::GetResolution() const
	{
		return Vector2f( static_cast< f32 >( mWidth ), static_cast< f32 >( mHeight ) );
	}
}