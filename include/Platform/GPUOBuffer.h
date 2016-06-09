#ifndef BH_GPU_OBUFFER_H
#define BH_GPU_OBUFFER_H

#include "Platform/Vertex.h"
#include "Platform/Graphics.h"
#include "Platform/Vector4f.h"

#include <d3d11.h>

namespace BH
{
	class GPUOBuffer
	{
	public:
		// Constructor
		GPUOBuffer();

		// Destructor
		~GPUOBuffer();

		// Initialise
		void Initialise( u32 width, u32 height );

		// Shutdown
		void Shutdown();

		// Copy buffer to texture
		void CopyBufferToTexture();

		// Get Shader Resource View
		ID3D11ShaderResourceView * GetShaderResourceView();

	private:
		ID3D11ShaderResourceView * mShaderResourceView;
		ID3D11Buffer * mDataBuffer;
		ID3D11UnorderedAccessView * mDataBufferView;
		ID3D11Texture2D * mDataTexture;
		u32 mWidth;
		u32 mHeight;

		friend class RenderTarget;
		friend class Shader;
		friend class Graphics;
	};

}

#endif