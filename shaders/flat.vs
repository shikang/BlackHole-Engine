////////////////////////////////////////////////////////////////////////////////
// Filename: flat.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;

};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType FlatVertexShader( VertexInputType input )
{
    PixelInputType output;

    // Calculate the position of the vertex in the world
    float4 viewPosition = mul( float4( input.position.xyz, 1.0f ), worldMatrix );
    viewPosition = mul( viewPosition, viewMatrix );
    
    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul( viewPosition, projectionMatrix );
    
    // Store the input color for the pixel shader to use.
    output.color = input.color;
    
    // Store the input tex for the pixel shader to use.
    output.tex = input.tex;

    return output;
}