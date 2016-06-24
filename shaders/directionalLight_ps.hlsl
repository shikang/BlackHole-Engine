////////////////////////////////////////////////////////////////////////////////
// Filename: light.ps
////////////////////////////////////////////////////////////////////////////////
#include "lightingFunctions.hlsl"

Texture2D positionTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D diffuseTexture : register(t2);
Texture2D specularTexture : register(t3);
Texture2D depthTexture : register(t4);

SamplerState sampleType : register(s0);

/////////////
// GLOBALS //
/////////////
cbuffer LightBuffer
{
	float4 color;
	float3 eye;
	float screenWidth;
	float3 light;
	float screenHeight;
	float3 lightDirection;
	float lightHalfWidth;
	float3 lightSide;
	float lightHalfHeight;
	float3 lightUp;
	float lightRange;
};

cbuffer ShadowBuffer
{
	matrix shadowMatrix;
};

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 position : SV_POSITION;
};

////////////////////////////////////////////////////////////////////////////////
// Helper function
////////////////////////////////////////////////////////////////////////////////
/*
float3 Diffuse( float3 l, float3 n, float3 diffuse, float3 color )
{
	// @todo: Check if position is within light frustrum
	return ( diffuse / PI ) * max( dot( n, l ), 0.0f ) * color;
}
*/

float Shadow( float3 p, float3 n, float3 l )
{
	float4 shadowCoord = mul( float4( p.xyz, 1.0f ), shadowMatrix );
	shadowCoord.x = ( shadowCoord.x + 1.0f ) * 0.5f;
	shadowCoord.y = ( 1.0f - shadowCoord.y ) * 0.5f;
	float shadow = 1.0f;
	float2 shadowIndex = ( shadowCoord.w > 0.0f ) ? shadowCoord.xy / shadowCoord.w : float2( -1.0f, -1.0f );
	// Inside depth map
	if( shadowIndex.x >= 0.0f && shadowIndex.x <= 1.0f &&
		shadowIndex.y >= 0.0f && shadowIndex.y <= 1.0f )
	{
		float bias = 0.005 * tan( acos( dot( n, l ) ) );
		float4 depth = depthTexture.Sample( sampleType, shadowIndex );
		if( shadowCoord.z - bias > depth.z )
			shadow = 0.0f;
	}

	return shadow;
}

float WithinLight( float3 p )
{
	float3 v = p - light;
	float d = dot( v, lightDirection );

	if( abs( dot( v, lightSide ) ) <= lightHalfWidth &&
		abs( dot( v, lightUp ) ) <= lightHalfHeight &&
		d >= 0.0f && d <= lightRange )
		return 1.0f;

	return 0.0f;
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 LightPixelShader( PixelInputType input ) : SV_TARGET
{
	float2 uv = float2( input.position.xy ) / float2( screenWidth, screenHeight );
	float4 pTexel = positionTexture.Sample( sampleType, uv );
	float3 p = pTexel.xyz;
	float3 l = normalize( light - p );
	float3 n = normalTexture.Sample( sampleType, uv ).xyz;

	float4 diffuse = diffuseTexture.Sample( sampleType, uv );

	float lightDistance = length( light - p );
	float shadow = ( pTexel.w != 0.0f ) ? Shadow( p, n, l ) : 1.0f;

	float3 lighting = BRDFDiffuse( l, n, ( float3 )diffuse, ( float3 )color, 0.0f, float3( 0.0f, 0.0f, 0.0f ), 0.0f );

	float within = WithinLight( p );

	return within * shadow * float4( lighting.xyz, 1.0f );
}
