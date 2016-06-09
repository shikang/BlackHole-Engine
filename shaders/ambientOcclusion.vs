////////////////////////////////////////////////////////////////////////////////
// Filename: ambientOcclusion.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
	matrix inverseWorldMatrix;
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float2 position : POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType AOVertexShader( VertexInputType input )
{
	PixelInputType output;

	float4 pos = float4( input.position.x, input.position.y, 0.0f, 1.0f );

    // Calculate the position
    output.position = mul( pos, worldMatrix );
    output.position = mul( output.position, viewMatrix );
    output.position = mul( output.position, projectionMatrix );
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    return output;
}