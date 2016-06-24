////////////////////////////////////////////////////////////////////////////////
// Filename: light.vs
////////////////////////////////////////////////////////////////////////////////
#include "constants.hlsl"

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
    float3 position : POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType LightVertexShader( VertexInputType input )
{
	PixelInputType output;

    // Calculate the position
    output.position = mul( float4( input.position.xyz, 1.0f ), worldMatrix );
    output.position = mul( output.position, viewMatrix );
    output.position = mul( output.position, projectionMatrix );

    return output;
}