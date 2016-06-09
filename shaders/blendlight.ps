////////////////////////////////////////////////////////////////////////////////
// Filename: blendlight.ps
////////////////////////////////////////////////////////////////////////////////

Texture2D traceTexture : register(t0);

SamplerState sampleType : register(s0);

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
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 BlendLightPixelShader( PixelInputType input ) : SV_TARGET
{
	// Get texture coordinate position
	float2 uv = float2( input.position.xy ) / float2( screenResolution.x, screenResolution.y );

	float4 color;
	color.xyz = 2.0f * traceTexture.Sample( sampleType, uv ).x;
	color.w = traceTexture.Sample( sampleType, uv ).x;
	
	return color;
}
