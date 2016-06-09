// Precompiled
#include "Core/Stdafx.h"

#include "Core/Materials.h"
#include "Core/SystemManager.h"
#include "Core/TextureManager.h"

namespace BH
{
	void Phong::SendMaterial( Shader & shader, ShaderType type, u32 bufferNum ) const
	{
		shader.SetShaderParameters( type, bufferNum,
									mSpecular,
									mRoughness );

		Texture * texture = SYSTEM_MANAGER.GetGameComponentFromSystem<TextureManager>()->GetTexture( mTexture );
		texture = texture ? texture : SYSTEM_MANAGER.GetGameComponentFromSystem<TextureManager>()->GetTexture( "white" );
		shader.SetTexture( PixelShader, 0, texture );
	}
}