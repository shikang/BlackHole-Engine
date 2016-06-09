#ifndef BH_HDR_TEXTURE_H
#define BH_HDR_TEXTURE_H

#include "Platform/Vertex.h"
#include "Platform/Graphics.h"
#include "Platform/Vector2f.h"

#include <d3d11.h>
#include <vector>

namespace BH
{
	class HDRTexture
	{
	public:
		// Constructor
		HDRTexture();

		// Destructor
		~HDRTexture();

		// Initialise
		void Initialise( const String & filename );

		// Shutdown
		void Shutdown();

		// Get image dimension
		f32 GetTextureDimension() const;

	private:
		void CreateTextureFromData();

		void LoadHDRTexture( const WCHAR * filename );

	private:
		ID3D11ShaderResourceView * mTextureView;
		ID3D11Texture2D * mTexture;

		std::vector< f32 > mData;
		s32 mWidth;
		s32 mHeight;

		f32 mExposure;
		f32 mGamma;

		friend class Shader;
		friend class Graphics;
	};

}

#endif