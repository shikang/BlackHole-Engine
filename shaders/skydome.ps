////////////////////////////////////////////////////////////////////////////////
// Filename: skydome.ps
////////////////////////////////////////////////////////////////////////////////

Texture2D skyTexture : register(t0);
Texture2D positionTexture : register(t1);
//Texture2D sceneTexture : register(t2);

SamplerState sampleType;

/////////////
// GLOBALS //
/////////////
cbuffer LightBuffer
{
	float2 screenResolution;
	float2 padding;
};

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 SkyPixelShader( PixelInputType input ) : SV_TARGET
{  
	float2 uv = float2( input.position.xy ) / float2( screenResolution.x, screenResolution.y );
	float written = positionTexture.Sample( sampleType, uv ).w;
	float4 color; 
	
	if( written == 0.0f )
	{
		// Tone mapping from HDR texture
		float3 c = skyTexture.Sample( sampleType, input.tex ).xyz;

		// It's better if it's tweakable
		float exposure = 0.18f;

		c = pow( ( exposure * c ) / ( exposure * c + float3( 1.0f, 1.0f,1.0f ) ), 1.0f / 2.2f );		
		color = float4( c, 1.0f );
	}
	else
	{
		color = float4( 0.0f, 0.0f, 0.0f, 0.0f );
	}
	
    return color;
}
