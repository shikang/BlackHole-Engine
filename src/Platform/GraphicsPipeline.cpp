// Precompiled header
#include "Platform/Stdafx.h"

#include "Platform/GraphicsPipeline.h"

//#include <d3dx9.h>

namespace BH
{
	GraphicsPipeline::GraphicsPipeline()
	: mSwapChain( nullptr )
	, mDevice( nullptr )
	, mDeviceContext( nullptr )
	, mRenderTargetView( nullptr )
	, mDepthStencilBuffer( nullptr )
	, mDepthStencilState( nullptr )
	, mDepthDisabledStencilState( nullptr )
	, mDepthStencilView( nullptr )
	, mRasterState( nullptr )
	, mFrontCullRasterState( nullptr )
	, mNoCullRasterState( nullptr )
	, mAlphaEnableBlendingState( nullptr )
	, mAlphaDisableBlendingState( nullptr )
	, mAlphaAdditiveBlendState( nullptr )
	, mVSync( false )
	{
	}

	GraphicsPipeline::~GraphicsPipeline()
	{
	}

	bool GraphicsPipeline::Initialise( u32 screen_width, 
									   u32 screen_height, 
									   HWND window,
									   f32 screen_near, 
									   f32 screen_far, 
									   bool fullscreen,
									   bool vsync )
	{
		HRESULT result;
		IDXGIFactory* factory;
		
		// Store the vsync setting.
		mVSync = vsync;

		// Create a DirectX graphics interface factory.
		result = CreateDXGIFactory( __uuidof( IDXGIFactory ), 
									reinterpret_cast<void**>( &factory ) );
		if ( FAILED( result ) )
		{
			return false;
		}

		IDXGIAdapter* adapter;
		IDXGIOutput* adapterOutput;
		u32 numModes;

		// Use the factory to create an adapter for the primary graphics interface (video card).
		result = factory->EnumAdapters( 0, &adapter );
		if ( FAILED( result ) )
		{
			return false;
		}

		// Enumerate the primary adapter output (monitor).
		result = adapter->EnumOutputs( 0, &adapterOutput );
		if ( FAILED( result ) )
		{
			return false;
		}

		// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
		result = adapterOutput->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM, 
													DXGI_ENUM_MODES_INTERLACED, 
													&numModes, 
													NULL );
		if ( FAILED( result ) )
		{
			return false;
		}

		// Create a list to hold all the possible display modes for this monitor/video card combination.
		DXGI_MODE_DESC * displayModeList = new DXGI_MODE_DESC[numModes];
		if ( !displayModeList )
		{
			return false;
		}

		// Now fill the display mode list structures.
		result = adapterOutput->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM, 
													DXGI_ENUM_MODES_INTERLACED, 
													&numModes, 
													displayModeList );
		if ( FAILED( result ) )
		{
			return false;
		}

		// Now go through all the display modes and find the one that matches the screen width and height.
		// When a match is found store the numerator and denominator of the refresh rate for that monitor.
		u32 numerator = 0, denominator = 0;
		for ( u32 i = 0; i < numModes; ++i )
		{
			if ( displayModeList[i].Width == static_cast<u32>( screen_width ) )
			{
				if ( displayModeList[i].Height == static_cast<u32>( screen_height ) )
				{
					numerator = displayModeList[i].RefreshRate.Numerator;
					denominator = displayModeList[i].RefreshRate.Denominator;
				}
			}
		}

		DXGI_ADAPTER_DESC adapterDesc;

		// Get the adapter (video card) description.
		result = adapter->GetDesc( &adapterDesc );
		if ( FAILED( result ) )
		{
			return false;
		}

#if 0
		// Store the dedicated video card memory in megabytes.
		mVideoCardMemory = static_cast<int>( adapterDesc.DedicatedVideoMemory / 1024 / 1024 );

		// Convert the name of the video card to a character array and store it.
		error = wcstombs_s( &stringLength, mVideoCardDescription, 128, adapterDesc.Description, 128 );
		if ( error != 0 )
		{
			return false;
		}
#endif

		// Release the display mode list.
		delete[] displayModeList;
		displayModeList = 0;

		// Release the adapter output.
		adapterOutput->Release();
		adapterOutput = 0;

		// Release the adapter.
		adapter->Release();
		adapter = 0;

		// Release the factory.
		factory->Release();
		factory = 0;

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		// Initialize the swap chain description.
		ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );

		// Set to a single back buffer.
		swapChainDesc.BufferCount = 1;

		// Set the width and height of the back buffer.
		swapChainDesc.BufferDesc.Width = screen_width;
		swapChainDesc.BufferDesc.Height = screen_height;

		// Set regular 32-bit surface for the back buffer.
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		// Set the refresh rate of the back buffer.
		if ( mVSync )
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
		}
		else
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}

		// Set the usage of the back buffer.
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

		// Set the handle for the window to render to.
		swapChainDesc.OutputWindow = window;

		// Turn multisampling off.
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;

		// Set to full screen or windowed mode.
		swapChainDesc.Windowed = !fullscreen;

		// Set the scan line ordering and scaling to unspecified.
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// Discard the back buffer contents after presenting.
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		// Don't set the advanced flags.
		swapChainDesc.Flags = 0;

		// Set the feature level to DirectX 11.
		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

		// Create the swap chain, Direct3D device, and Direct3D device context.
		result = D3D11CreateDeviceAndSwapChain( NULL, 
												D3D_DRIVER_TYPE_HARDWARE, 
												NULL, 
												0, 
												&featureLevel, 
												1,
												D3D11_SDK_VERSION, 
												&swapChainDesc, 
												&mSwapChain, 
												&mDevice, 
												NULL, 
												&mDeviceContext );
		if ( FAILED( result ) )
		{
			return false;
		}

		// Get the pointer to the back buffer.
		ID3D11Texture2D* backBufferPtr;
		result = mSwapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), 
										reinterpret_cast<LPVOID*>( &backBufferPtr ) );
		if ( FAILED( result ) )
		{
			return false;
		}

		// Create the render target view with the back buffer pointer.
		result = mDevice->CreateRenderTargetView( backBufferPtr, 
												  NULL, 
												  &mRenderTargetView );
		if ( FAILED( result ) )
		{
			return false;
		}

		// Release pointer to the back buffer as we no longer need it.
		backBufferPtr->Release();
		backBufferPtr = 0;

		// Initialize the description of the depth buffer.
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		ZeroMemory( &depthBufferDesc, sizeof( depthBufferDesc ) );

		// Set up the description of the depth buffer.
		depthBufferDesc.Width = screen_width;
		depthBufferDesc.Height = screen_height;
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
		result = mDevice->CreateTexture2D( &depthBufferDesc, NULL, &mDepthStencilBuffer );
		if ( FAILED( result ) )
		{
			return false;
		}

		// Initialize the description of the stencil state.
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		ZeroMemory( &depthStencilDesc, sizeof( depthStencilDesc ) );

		// Set up the description of the stencil state.
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Stencil operations if pixel is front-facing.
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Stencil operations if pixel is back-facing.
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the depth stencil state.
		result = mDevice->CreateDepthStencilState( &depthStencilDesc, &mDepthStencilState );
		if ( FAILED( result ) )
		{
			return false;
		}

		// Set the depth stencil state.
		mDeviceContext->OMSetDepthStencilState( mDepthStencilState, 1 );

		// Initialize the depth stencil view.
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		ZeroMemory( &depthStencilViewDesc, sizeof( depthStencilViewDesc ) );

		// Set up the depth stencil view description.
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// Create the depth stencil view.
		result = mDevice->CreateDepthStencilView( mDepthStencilBuffer, &depthStencilViewDesc, &mDepthStencilView );
		if ( FAILED( result ) )
		{
			return false;
		}

		// Bind the render target view and depth stencil buffer to the output render pipeline.
		mDeviceContext->OMSetRenderTargets( 1, &mRenderTargetView, mDepthStencilView );

		// Setup the raster description which will determine how and what polygons will be drawn.
		D3D11_RASTERIZER_DESC rasterDesc;
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		// Create the rasterizer state from the description we just filled out.
		result = mDevice->CreateRasterizerState( &rasterDesc, &mRasterState );
		if ( FAILED( result ) )
		{
			return false;
		}

		// Now set the rasterizer state.
		mDeviceContext->RSSetState( mRasterState );

		// Setup a raster description which turns on front face culling.
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_FRONT;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		// Create the rasterizer state from the description we just filled out.
		result = mDevice->CreateRasterizerState( &rasterDesc, &mFrontCullRasterState );
		if ( FAILED( result ) )
		{
			return false;
		}

		// Setup a raster description which turns off back face culling.
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_NONE;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		// Create the no culling rasterizer state.
		result = mDevice->CreateRasterizerState( &rasterDesc, &mNoCullRasterState );
		if ( FAILED( result ) )
		{
			return false;
		}

		// Setup the viewport for rendering.
		mViewport.Width = static_cast<f32>( screen_width );
		mViewport.Height = static_cast<f32>( screen_height );
		mViewport.MinDepth = 0.0f;
		mViewport.MaxDepth = 1.0f;
		mViewport.TopLeftX = 0.0f;
		mViewport.TopLeftY = 0.0f;

		// Create the viewport.
		mDeviceContext->RSSetViewports( 1, &mViewport );

		// Setup the projection matrix.
		mFOV = static_cast<f32>( 3.14159265359f ) / 4.0f;
		mAspectRatio = static_cast<f32>( screen_width ) / static_cast<f32>( screen_height );

		// Create the projection matrix for 3D rendering.
		mProjectionMatrix = Matrix4::CreatePerspectiveFieldOfView( mFOV, mAspectRatio, screen_near, screen_far );

		// Create an orthographic projection matrix for 2D rendering.
		mOrthoMatrix = Matrix4::CreateOrthographic( static_cast<f32>( screen_width ), static_cast<f32>( screen_height ), screen_near, screen_far );

		// Clear the second depth stencil state before setting the parameters.
		D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
		ZeroMemory( &depthDisabledStencilDesc, sizeof( depthDisabledStencilDesc ) );

		// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
		// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
		depthDisabledStencilDesc.DepthEnable = false;
		depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthDisabledStencilDesc.StencilEnable = true;
		depthDisabledStencilDesc.StencilReadMask = 0xFF;
		depthDisabledStencilDesc.StencilWriteMask = 0xFF;
		depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Create the state using the device.
		result = mDevice->CreateDepthStencilState(&depthDisabledStencilDesc, &mDepthDisabledStencilState);
		if ( FAILED( result ) )
		{
			return false;
		}

		D3D11_BLEND_DESC blendStateDescription;
		// Clear the blend state description.
		ZeroMemory( &blendStateDescription, sizeof( D3D11_BLEND_DESC ) );

		// Create an alpha enabled blend state description.
		blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
		blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		// Create the blend state using the description.
		result = mDevice->CreateBlendState( &blendStateDescription, &mAlphaEnableBlendingState );
		if ( FAILED( result ) )
		{
			return false;
		}

		// Modify the description to create an alpha disabled blend state description.
		blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

		// Create the second blend state using the description.
		result = mDevice->CreateBlendState( &blendStateDescription, &mAlphaDisableBlendingState );
		if ( FAILED( result ) )
		{
			return false;
		}

		// Create a secondary alpha blend state description.
		blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
		blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

		// Create the blend state using the description.
		result = mDevice->CreateBlendState( &blendStateDescription, &mAlphaAdditiveBlendState );
		if ( FAILED( result ) )
		{
			return false;
		}

		return true;
	}

	void GraphicsPipeline::Shutdown()
	{
		// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
		if ( mSwapChain )
		{
			mSwapChain->SetFullscreenState( false, NULL );
		}

		if ( mAlphaAdditiveBlendState )
		{
			mAlphaAdditiveBlendState->Release();
			mAlphaAdditiveBlendState = 0;
		}

		if ( mAlphaEnableBlendingState )
		{
			mAlphaEnableBlendingState->Release();
			mAlphaEnableBlendingState = 0;
		}

		if ( mAlphaDisableBlendingState )
		{
			mAlphaDisableBlendingState->Release();
			mAlphaDisableBlendingState = 0;
		}

		if ( mRasterState )
		{
			mRasterState->Release();
			mRasterState = nullptr;
		}

		if ( mFrontCullRasterState )
		{
			mFrontCullRasterState->Release();
			mFrontCullRasterState = nullptr;
		}

		if ( mNoCullRasterState )
		{
			mNoCullRasterState->Release();
			mNoCullRasterState = nullptr;
		}

		if ( mDepthStencilView )
		{
			mDepthStencilView->Release();
			mDepthStencilView = nullptr;
		}

		if ( mDepthStencilState )
		{
			mDepthStencilState->Release();
			mDepthStencilState = nullptr;
		}

		if ( mDepthDisabledStencilState )
		{
			mDepthDisabledStencilState->Release();
			mDepthDisabledStencilState = nullptr;
		}

		if ( mDepthStencilBuffer )
		{
			mDepthStencilBuffer->Release();
			mDepthStencilBuffer = nullptr;
		}

		if ( mRenderTargetView )
		{
			mRenderTargetView->Release();
			mRenderTargetView = nullptr;
		}

		if ( mDeviceContext )
		{
			mDeviceContext->Release();
			mDeviceContext = nullptr;
		}

		if ( mDevice )
		{
			mDevice->Release();
			mDevice = nullptr;
		}

		if ( mSwapChain )
		{
			mSwapChain->Release();
			mSwapChain = nullptr;
		}
	}

	void GraphicsPipeline::BeginScene( const Vector4f & color )
	{
		f32 fcolors[4]{ color.x, color.y, color.z, color.w };

		// Clear the back buffer.
		mDeviceContext->ClearRenderTargetView( mRenderTargetView, fcolors );

		// Clear the depth buffer.
		mDeviceContext->ClearDepthStencilView( mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	}

	void GraphicsPipeline::EndScene()
	{
		// Present the back buffer to the screen since rendering is complete.
		// Lock to screen refresh rate if v-sync is on.
		// Present as fast as possible if v-sync is off.
		mSwapChain->Present( mVSync ? 1 : 0, 0 );
	}

	Matrix4 GraphicsPipeline::GetWorld() const
	{
		return mWorldMatrix;
	}

	Matrix4 GraphicsPipeline::GetProjection() const
	{
		return mProjectionMatrix;
	}

	Matrix4 GraphicsPipeline::GetOrthogonal() const
	{
		return mOrthoMatrix;
	}

	void GraphicsPipeline::SetWorld( const Matrix4 & world )
	{
		mWorldMatrix = world;
	}

	void GraphicsPipeline::SetProjection( const Matrix4 & projection )
	{
		mProjectionMatrix = projection;
	}

	void GraphicsPipeline::SetOrthogonal( const Matrix4 & orthogonal )
	{
		mOrthoMatrix = orthogonal;
	}

	ID3D11Device * GraphicsPipeline::GetDevice() const
	{
		return mDevice;
	}

	ID3D11DeviceContext * GraphicsPipeline::GetContext() const
	{
		return mDeviceContext;
	}

	ID3D11DepthStencilView * GraphicsPipeline::GetDepthStencilView() const
	{
		return mDepthStencilView;
	}

	void GraphicsPipeline::SetBackBufferRenderTarget()
	{
		// Bind the render target view and depth stencil buffer to the output render pipeline.
		mDeviceContext->OMSetRenderTargets( 1, &mRenderTargetView, mDepthStencilView );
	}

	void GraphicsPipeline::SetAlphaBlend()
	{
		// Set blend factor
		f32 blendFactor[4]{ 0.0f, 0.0f, 0.0f, 0.0f };

		// Turn on the alpha blending.
		mDeviceContext->OMSetBlendState( mAlphaEnableBlendingState, blendFactor, 0xffffffff );

		return;
	}


	void GraphicsPipeline::SetNoBlend()
	{
		f32 blendFactor[4]{ 0.0f, 0.0f, 0.0f, 0.0f };

		// Turn off the alpha blending.
		mDeviceContext->OMSetBlendState( mAlphaDisableBlendingState, blendFactor, 0xffffffff );

		return;
	}

	void GraphicsPipeline::SetAdditiveBlend()
	{
		// Set blend factor
		f32 blendFactor[4]{ 0.0f, 0.0f, 0.0f, 0.0f };

		// Turn on the alpha blending.
		mDeviceContext->OMSetBlendState( mAlphaAdditiveBlendState, blendFactor, 0xffffffff );

		return;
	}

	void GraphicsPipeline::SetBackFaceCull( bool on )
	{
		// Set the culling rasterizer state.
		if ( on )
			mDeviceContext->RSSetState( mRasterState );
		else
			mDeviceContext->RSSetState( mNoCullRasterState );
	}

	void GraphicsPipeline::SetFrontFaceCull( bool on )
	{
		// Set the culling rasterizer state.
		if ( on )
			mDeviceContext->RSSetState( mFrontCullRasterState );
		else
			mDeviceContext->RSSetState( mNoCullRasterState );
	}

	void GraphicsPipeline::SetZBuffer( bool on )
	{
		if ( on )
			mDeviceContext->OMSetDepthStencilState( mDepthStencilState, 1 );
		else
			mDeviceContext->OMSetDepthStencilState( mDepthDisabledStencilState, 1 );
	}

	void GraphicsPipeline::ResetViewport()
	{
		// Set viewport
		mDeviceContext->RSSetViewports( 1, &mViewport );
	}
}