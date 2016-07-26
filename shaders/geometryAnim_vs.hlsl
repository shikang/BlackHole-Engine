////////////////////////////////////////////////////////////////////////////////
// Filename: geometry.vs
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
    //int4 indices : BLENDINDICES;
    float4 indices : BLENDINDICES;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 world : TEXCOORD1;
	float depth : TEXCOORD2;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType GeometryVertexShader( VertexInputType input )
{
	PixelInputType output;

    float3 position = float3( 0.0f, 0.0f, 0.0f );
    float3 normal = float3( 0.0f, 0.0f, 0.0f );

    float weightsArray[4] = ( float[4] )input.weights;
    int indexArray[4] = ( int[4] )input.indices;

    // Apply weights
    for ( int i = 0; i < VERTEX_WEIGHTS_NUM; ++i )
    {
        position += mul( input.position, boneMatrixArray[indexArray[i]] ) * weightsArray[i];
        normal += mul( input.normal, boneMatrixArray[indexArray[i]] ) * weightsArray[i];
    }

    // Calculate the position
    //output.position = mul( float4( input.position.xyz, 1.0f ), worldMatrix );
    output.position = mul( float4( position.xyz, 1.0f ), worldMatrix );
    //output.position = float4( position.xyz, 1.0f );
    output.world = output.position.xyz;
    output.position = mul( output.position, viewMatrix );
    output.depth = output.position.z;
    output.position = mul( output.position, projectionMatrix );
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
    // Calculate the normal vector against the world matrix only.
    normal = normalize(normal);
    //output.normal = mul( input.normal, ( float3x3 )transpose( inverseWorldMatrix ) );
    output.normal = mul( normal, ( float3x3 )transpose( inverseWorldMatrix ) );
    
    return output;
}