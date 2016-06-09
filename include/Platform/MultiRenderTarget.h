#ifndef BH_MULTI_RENDER_TARGET_H
#define BH_MULTI_RENDER_TARGET_H

#include "Platform/Vertex.h"
#include "Platform/Graphics.h"
#include "Platform/Vector4f.h"

#include <d3d11.h>
#include <vector>

namespace BH
{
	class MultiRenderTarget
	{
	public:
		// Constructor
		MultiRenderTarget();

		// Destructor
		~MultiRenderTarget();

		// Initialise
		void Initialise( u32 count, u32 width, u32 height );

		// Shutdown
		void Shutdown();

		// Set all render targets as current
		void SetRenderTargets();

		// Clear all render targets
		void ClearRenderTargets( const Vector4f & color );

		// Get Shader Resource View
		ID3D11ShaderResourceView * GetShaderResourceView( u32 index );

		// Get total render targets count
		u32 GetRenderTargetsCount() const;

		// Get Resolution
		Vector2f GetResolution() const;

	private:
		std::vector< ID3D11ShaderResourceView * > mShaderResourceViews;
		std::vector< ID3D11RenderTargetView * > mRenderTargetViews;
		std::vector< ID3D11Texture2D * > mRenderTargetTextures;
		ID3D11Texture2D * mDepthStencilBuffer;
		ID3D11DepthStencilView * mDepthStencilView;
		D3D11_VIEWPORT mViewport;
		u32 mWidth;
		u32 mHeight;

		friend class Shader;
		friend class Graphics;
	};
}

#endif