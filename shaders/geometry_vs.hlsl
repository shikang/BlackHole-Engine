////////////////////////////////////////////////////////////////////////////////
// Filename: geometry.vs
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
    float3 position : POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;

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

    // Calculate the position
    output.position = mul( float4( input.position.xyz, 1.0f ), worldMatrix );
    output.world = output.position.xyz;
	output.position = mul( output.position, viewMatrix );
	output.depth = output.position.z;
    output.position = mul( output.position, projectionMatrix );
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
    // Calculate the normal vector against the world matrix only.
    output.normal = mul( input.normal, ( float3x3 )transpose( inverseWorldMatrix ) );

    return output;
}