#ifndef BH_WIC_H
#define BH_WIC_H

#include <wincodec.h>

namespace BH
{
	HRESULT CreateWICTextureFromFile( ID3D11Device * device,
									  const WCHAR * filename,
									  ID3D11Resource ** texture,
									  ID3D11ShaderResourceView ** textureView,
									  size_t maxsize );
}
#endif