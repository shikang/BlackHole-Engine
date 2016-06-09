#ifndef BH_TEXTURE_H
#define BH_TEXTURE_H

#include "Platform/Vertex.h"
#include "Platform/Graphics.h"

#include <d3d11.h>

namespace BH
{
	class Texture
	{
	public:
		// Constructor
		Texture();

		// Destructor
		~Texture();

		// Initialise
		void Initialise( const String & filename );

		// Shutdown
		void Shutdown();

	private:
		void CreateTextureFromFile( const WCHAR * filename );

	private:
		ID3D11ShaderResourceView * mTextureView;
		ID3D11Resource * mTexture;

		friend class Shader;
		friend class Graphics;
	};

}

#endif