////////////////////////////////////////////////////////////////////////////////
// Filename: quad.vs
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
    float2 position : POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD0;
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
PixelInputType QuadVertexShader( VertexInputType input )
{
    PixelInputType output;
    
    float4 pos = float4( input.position.x, input.position.y, 0.0f, 1.0f );

    output.color = input.color;
    output.tex = input.tex;
    output.position = mul( pos, worldMatrix );
    output.position = mul( output.position, viewMatrix );
    output.position = mul( output.position, projectionMatrix );
    return output;
}