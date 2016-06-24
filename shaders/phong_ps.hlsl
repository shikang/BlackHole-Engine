////////////////////////////////////////////////////////////////////////////////
// Filename: phong.ps
////////////////////////////////////////////////////////////////////////////////

#include "lightingFunctions.hlsl"

Texture2D shaderTexture;
SamplerState sampleType;

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
    float4 specularColor;
    float  specularPower;
    float3 padding;
};

//////////////
// TYPEDEFS //
//////////////
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
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 PhongPixelShader( PixelInputType input ) : SV_TARGET
{
    // Calculate half vector
    //float3 H = normalize( input.L + input.V );
    
    // Calculate diffuse term
    //float4 diffuse = max( dot( input.N, input.L ), 0.0f ) * diffuseColor;
    
    // Calculate specular term (Replace R.V with N.H)
    //float4 specular = pow( max( dot( input.N, H ), 0.0f ), specularPower ) * specularColor;
    
    //float4 color = input.color;
    float4 color = shaderTexture.Sample( sampleType, input.tex );

    //return ambientColor + diffuse * color + specular;
    return ambientColor + Phong( input.L, input.N, input.V, float4( 1.0f, 1.0f, 1.0f, 1.0f ), specularPower, specularColor, color );

}
