////////////////////////////////////////////////////////////////////////////////
// Filename: environment.vs
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
	float3 viewPosition : TEXCOORD0;
	float3 viewNormal : NORMAL;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType EnvironmentVertexShader( VertexInputType input )
{
	PixelInputType output;

    // Calculate the position
    output.position = mul( float4( input.position.xyz, 1.0f ), worldMatrix );
    output.position = mul( output.position, viewMatrix );
	output.viewPosition = output.position.xyz;
    output.position = mul( output.position, projectionMatrix );

	output.viewNormal = mul( float4( input.normal.xyz, 0.0f ), inverseWorldMatrix );
	output.viewNormal = mul( output.viewNormal, viewMatrix ).xyz;

    return output;
}