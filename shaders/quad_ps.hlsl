////////////////////////////////////////////////////////////////////////////////
// Filename: quad.ps
////////////////////////////////////////////////////////////////////////////////

Texture2D shaderTexture;
SamplerState sampleType;

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
float4 QuadPixelShader( PixelInputType input ) : SV_TARGET
{
    return shaderTexture.Sample( sampleType, input.tex );
}
