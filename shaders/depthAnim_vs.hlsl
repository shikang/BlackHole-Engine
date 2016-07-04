////////////////////////////////////////////////////////////////////////////////
// Filename: depth.vs
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
};

cbuffer AnimationBuffer
{
    matrix boneMatrixArray[MAX_BONE_MATRICES] : WORLDMATRIXARRAY;
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
    float4 weights : BLENDWEIGHT;
    float4 indices : BLENDINDICES;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float4 shadowPosition : TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType DepthVertexShader( VertexInputType input )
{
	PixelInputType output;

    float3 position = float3( 0.0f, 0.0f, 0.0f );

    float weightsArray[4] = ( float[4] )input.weights;
    int indexArray[4] = ( int[4] )input.indices;

    // Apply weights
    for ( int i = 0; i < VERTEX_WEIGHTS_NUM; ++i )
    {
        position += mul( input.position, boneMatrixArray[indexArray[i]] ) * weightsArray[i];
    }

    // Calculate the position
    //output.position = mul( float4( input.position.xyz, 1.0f ), worldMatrix );
    output.position = mul( float4( position.xyz, 1.0f ), worldMatrix );
    output.position = mul( output.position, viewMatrix );
    output.position = mul( output.position, projectionMatrix );

	output.shadowPosition = output.position;

    return output;
}