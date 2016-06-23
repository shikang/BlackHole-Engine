////////////////////////////////////////////////////////////////////////////////
// Filename: edge.ps
////////////////////////////////////////////////////////////////////////////////

Texture2D shaderTexture;
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
	float isEdge = 0.0f;

	float offsetX = textureDimensionInv.x * 0.5f;
	float offsetY = textureDimensionInv.y * 0.5f;

	float c0 = shaderTexture.Sample( sampleType, input.tex ).x;

	float c1 = shaderTexture.Sample( sampleType, input.tex + float2( offsetX, 0) ).x;
	float c2 = shaderTexture.Sample( sampleType, input.tex + float2( 0,-offsetY) ).x;
	float c3 = shaderTexture.Sample( sampleType, input.tex + float2(-offsetX, 0) ).x;
	float c4 = shaderTexture.Sample( sampleType, input.tex + float2( 0, offsetY) ).x;

	float c5 = shaderTexture.Sample( sampleType, input.tex + float2( offsetX, offsetY) ).x;
	float c6 = shaderTexture.Sample( sampleType, input.tex + float2( offsetX,-offsetY) ).x;
	float c7 = shaderTexture.Sample( sampleType, input.tex + float2(-offsetX,-offsetY) ).x;
	float c8 = shaderTexture.Sample( sampleType, input.tex + float2(-offsetX, offsetY) ).x;

	float c9 =  shaderTexture.Sample( sampleType, input.tex + float2( -2.0 * offsetX, -2.0 * offsetY) ).x;
	float c10 = shaderTexture.Sample( sampleType, input.tex + float2( -1.0 * offsetX, -2.0 * offsetY) ).x;
	float c11 = shaderTexture.Sample( sampleType, input.tex + float2(  0.0 * offsetX, -2.0 * offsetY) ).x;
	float c12 = shaderTexture.Sample( sampleType, input.tex + float2(  1.0 * offsetX, -2.0 * offsetY) ).x;
	float c13 = shaderTexture.Sample( sampleType, input.tex + float2(  2.0 * offsetX, -2.0 * offsetY) ).x;

	float c14 = shaderTexture.Sample( sampleType, input.tex + float2( -2.0 * offsetX, 2.0 * offsetY) ).x;
	float c15 = shaderTexture.Sample( sampleType, input.tex + float2( -1.0 * offsetX, 2.0 * offsetY) ).x;
	float c16 = shaderTexture.Sample( sampleType, input.tex + float2(  0.0 * offsetX, 2.0 * offsetY) ).x;
	float c17 = shaderTexture.Sample( sampleType, input.tex + float2(  1.0 * offsetX, 2.0 * offsetY) ).x;
	float c18 = shaderTexture.Sample( sampleType, input.tex + float2(  2.0 * offsetX, 2.0 * offsetY) ).x;

	float c19 = shaderTexture.Sample( sampleType, input.tex + float2( -2.0 * offsetX, -1.0 * offsetY) ).x;
	float c20 = shaderTexture.Sample( sampleType, input.tex + float2(  2.0 * offsetX, -1.0 * offsetY) ).x;
	float c21 = shaderTexture.Sample( sampleType, input.tex + float2( -2.0 * offsetX,  0.0 * offsetY) ).x;
	float c22 = shaderTexture.Sample( sampleType, input.tex + float2(  2.0 * offsetX,  0.0 * offsetY) ).x;
	float c23 = shaderTexture.Sample( sampleType, input.tex + float2( -2.0 * offsetX,  1.0 * offsetY) ).x;
	float c24 = shaderTexture.Sample( sampleType, input.tex + float2(  2.0 * offsetX,  1.0 * offsetY) ).x;

	// Apply Sobel 5x5 edge detection filter
	float Gx = 1.0 * ( -c9 -c14 + c13 + c18 ) + 2.0 * ( -c19 -c23 - c10 - c15 + c12 + c17 + c20 + c24 ) + 3.0 * ( -c21 -c7 -c8 + c6 + c5 + c22 ) + 5.0 * ( -c3 + c1 );
	float Gy = 1.0 * ( -c14 -c18 + c9 + c13 ) + 2.0 * ( -c15 -c17 - c23 - c24 + c19 + c20 + c10 + c12 ) + 3.0 * ( -c16 -c8 -c5 + c6 + c7 + c11 ) + 5.0 * ( -c4 + c2 );
	float scale = 25.0; // Blur scale, can be depth dependent

    return float4( Gx * scale, Gy * scale, 0.0f, 1.0f );
	//return shaderTexture.Sample( sampleType, input.tex );
}
