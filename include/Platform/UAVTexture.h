#ifndef BH_UAV_TEXTURE_H
#define BH_UAV_TEXTURE_H

#include "Platform/Vertex.h"
#include "Platform/Graphics.h"
#include "Platform/Vector4f.h"

#include <d3d11.h>

namespace BH
{
	class UAVTexture
	{
	public:
		// Constructor
		UAVTexture();

		// Destructor
		~UAVTexture();

		// Initialise
		void Initialise( u32 width, u32 height );

		// Shutdown
		void Shutdown();

		// Get Shader Resource View
		ID3D11UnorderedAccessView * GetUnorderedAccessView();

		// Get Shader Resource View
		ID3D11ShaderResourceView * GetShaderResourceView();

		// Get Dimension
		u32 GetWidth() const;
		u32 GetHeight() const;

	private:
		ID3D11UnorderedAccessView * mUnorderedAccessView;
		ID3D11ShaderResourceView * mShaderResourceView;
		ID3D11Texture2D * mTexture;
		u32 mWidth;
		u32 mHeight;

		friend class Shader;
		friend class Graphics;
	};

}

#endif