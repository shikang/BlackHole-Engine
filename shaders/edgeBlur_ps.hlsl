////////////////////////////////////////////////////////////////////////////////
// Filename: edgeBlur.ps
////////////////////////////////////////////////////////////////////////////////

Texture2D shaderTexture;
Texture2D gradientTexture;
SamplerState sampleType;

/////////////
// GLOBALS //
/////////////
cbuffer EdgeBuffer
{
	float2 textureDimensionInv;
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
float4 EdgePixelShader( PixelInputType input ) : SV_TARGET
{
	float2 gradient = gradientTexture.Sample( sampleType, input.tex ).xy;
	float2 offset;

	offset.x = textureDimensionInv.x * gradient.y; 
	offset.y = textureDimensionInv.x * gradient.x;
	
	float3 vColor = 0.0f;
    
    for( int iSample = -7; iSample < 8; iSample++ )
    {
		vColor += shaderTexture.Sample( sampleType, input.tex + offset * iSample ).xyz;
    }
    
    vColor *= ( 1.0 / 15.0 );
	return float4( vColor, 1.0f );
}
