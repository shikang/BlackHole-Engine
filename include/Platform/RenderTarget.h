#ifndef BH_RENDER_TARGET_H
#define BH_RENDER_TARGET_H

#include "Platform/Vertex.h"
#include "Platform/Graphics.h"
#include "Platform/Vector4f.h"

#include <d3d11.h>

namespace BH
{
	class RenderTarget
	{
	public:
		// Constructor
		RenderTarget();

		// Destructor
		~RenderTarget();

		// Initialise
		void Initialise( u32 width, u32 height );

		// Shutdown
		void Shutdown();

		// Set render target as current
		void SetRenderTarget();

		// Clear render target
		void ClearRenderTarget( const Vector4f & color );

		// Get Shader Resource View
		ID3D11ShaderResourceView * GetShaderResourceView();

		// Get Dimension
		u32 GetWidth() const;
		u32 GetHeight() const;

	private:
		ID3D11ShaderResourceView * mShaderResourceView;
		ID3D11RenderTargetView * mRenderTargetView;
		ID3D11Texture2D * mRenderTargetTexture;
		u32 mWidth;
		u32 mHeight;

		friend class GPUOBuffer;
		friend class Shader;
		friend class Graphics;
	};

}

#endif