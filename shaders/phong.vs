////////////////////////////////////////////////////////////////////////////////
// Filename: phong.vs
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

cbuffer LightPositionBuffer
{
    float3 lightPosition;
    float padding;
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
    float3 N : NORMAL;
    float3 L : TEXCOORD1;
    float3 V : TEXCOORD2;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType PhongVertexShader( VertexInputType input )
{
    PixelInputType output;

    // Calculate the position of the vertex in the world
    float4 viewPosition = mul( float4( input.position.xyz, 1.0f ), worldMatrix );
    viewPosition = mul( viewPosition, viewMatrix );
    
    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = viewPosition;
    output.position = mul( output.position, projectionMatrix );
    
    // Store the input color for the pixel shader to use.
    output.color = input.color;
    
    // Store the input tex for the pixel shader to use.
    output.tex = input.tex;
    
    // Normalize normal vector (in view space)
    output.N = mul( input.normal, ( float3x3 )inverseWorldMatrix );
    output.N = mul( output.N, ( float3x3 )viewMatrix );
    //output.N = normalize( output.N );
    
    // Light in view direction
    output.L = normalize( mul( lightPosition, ( float3x3 )viewMatrix ) - viewPosition.xyz );

    output.V = normalize( -viewPosition.xyz );

    return output;
}