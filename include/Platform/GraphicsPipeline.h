#ifndef BH_GRAPHICS_PIPELINE_H
#define BH_GRAPHICS_PIPELINE_H

//#pragma comment(lib, "Effects11d.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxguid.lib")

// Direct X include
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>

// Platform include
#include "Platform/Typedefs.h"
#include "Platform/Matrix4.h"
#include "Platform/Vector4f.h"

namespace BH
{
	class GraphicsPipeline
	{
	public:
		// Destructor
		~GraphicsPipeline();

		// Initialise
		bool Initialise( u32 screen_width, 
						 u32 screen_height, 
						 HWND window,
						 f32 screen_near, 
						 f32 screen_far, 
						 bool fullscreen = false,
						 bool vsync = false );

		// Shutdown
		void Shutdown();

		// Begin scene (Before render)
		void BeginScene( const Vector4f & color );

		// End scene (After render)
		void EndScene();

		// Get world matrix
		Matrix4 GetWorld() const;

		// Get projection matrix
		Matrix4 GetProjection() const;

		// Get orthogonal matrix
		Matrix4 GetOrthogonal() const;

		// Set world matrix to be used in pipeline
		void SetWorld( const Matrix4 & world );

		// Set projection matrix to be used in pipeline
		void SetProjection( const Matrix4 & projection );

		// Set orthogonal matrix to be used in pipeline
		void SetOrthogonal( const Matrix4 & orthogonal );

		inline f32 GetFieldOfView() const { return mFOV; }
		inline f32 GetAspectRatio() const { return mAspectRatio; }

	public:
		// Getters (Normally you would not touch this)
		// Get device
		ID3D11Device * GetDevice() const;

		// Get context
		ID3D11DeviceContext * GetContext() const;

		// Get Depth Stencil View
		ID3D11DepthStencilView * GetDepthStencilView() const;

		// Set Back Buffer Render Target (Setting default back buffer as render target)
		void SetBackBufferRenderTarget();

		// Set alpha blend
		void SetAlphaBlend();

		// Set no blend
		void SetNoBlend();

		// Set additive blend
		void SetAdditiveBlend();

		// Turn back face cull on/off
		void SetBackFaceCull( bool on );

		// Turn front face cull on/off
		void SetFrontFaceCull( bool on );

		// Turn Depth test on/off
		void SetZBuffer( bool on );

		// Reset Viewport
		void ResetViewport();

	private:
		// Constructor (Only graphics create this)
		GraphicsPipeline();

	private:
		Matrix4 mWorldMatrix;							//!< Model to World
		Matrix4 mProjectionMatrix;
		Matrix4 mOrthoMatrix;

		f32 mFOV;
		f32 mAspectRatio;

		IDXGISwapChain * mSwapChain;
		ID3D11Device * mDevice;
		ID3D11DeviceContext * mDeviceContext;
		ID3D11RenderTargetView * mRenderTargetView;
		ID3D11Texture2D * mDepthStencilBuffer;
		ID3D11DepthStencilState * mDepthStencilState;
		ID3D11DepthStencilState * mDepthDisabledStencilState;
		ID3D11DepthStencilView * mDepthStencilView;
		ID3D11RasterizerState * mRasterState;
		ID3D11RasterizerState * mFrontCullRasterState;
		ID3D11RasterizerState * mNoCullRasterState;
		ID3D11BlendState * mAlphaEnableBlendingState;
		ID3D11BlendState * mAlphaDisableBlendingState;
		ID3D11BlendState * mAlphaAdditiveBlendState;
		D3D11_VIEWPORT mViewport;

		bool mVSync;									//!< Enable v-sync

		friend class Graphics;
		friend class Mesh;
		friend class Shader;
	};
}

#endif