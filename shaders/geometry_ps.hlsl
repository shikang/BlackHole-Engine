////////////////////////////////////////////////////////////////////////////////
// Filename: geometry.ps
////////////////////////////////////////////////////////////////////////////////

Texture2D diffuse : register(t0);
SamplerState sampleType : register(s0);

/////////////
// GLOBALS //
/////////////
cbuffer MaterialBuffer
{
    float3 specular;
	float roughness;
};

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 world : TEXCOORD1;
	float depth : TEXCOORD2;
};

struct PixelOutputType
{
    float4 pos : SV_Target0;
    float4 normal : SV_Target1;
	float4 diffuse : SV_Target2;
	float4 specular : SV_Target3;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
PixelOutputType GeometryPixelShader( PixelInputType input ) : SV_TARGET
{
    PixelOutputType output;

	output.pos = float4( input.world, 1.0f );
	output.normal = float4( normalize( input.normal ), input.depth );
	output.diffuse = diffuse.Sample( sampleType, input.tex );
	output.specular = float4( specular, roughness );

	return output;
}
