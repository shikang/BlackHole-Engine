////////////////////////////////////////////////////////////////////////////////
// Filename: light.ps
////////////////////////////////////////////////////////////////////////////////

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
	float3 nearFarC;
	float lightRange;
};

cbuffer ShadowBuffer
{
	matrix shadowMatrix;
	int castShadow;
	int3 useless;
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
float3 BRDF( float3 l, float3 v, float3 h, float3 n, float3 diffuse, float3 specular, float roughness, float3 color )
{
	float distribution = ( ( roughness + 2.0f ) / ( 2.0f * PI ) ) * pow( max( dot( h, n ), 0.0f ), roughness );
	float3 fresnel = specular + ( float3( 1.0f, 1.0f, 1.0f ) - specular ) * pow( 1 - max( dot( l, h ), 0.0f ), 5.0f );

	//float geometry = 1.0f / pow( max( dot( l, h ), 0.01f ), 2.0f );
	float geometry = 1.0f;
	return ( diffuse / PI + ( ( distribution * fresnel * geometry ) * 0.25f ) ) * max( dot( n, l ), 0.0f ) * color;
}

float ExponentialShadow( float3 p )
{
	float4 shadowCoord = mul( float4( p.xyz, 1.0f ), shadowMatrix );
	float shadow = 1.0f;
	float2 shadowIndex = ( shadowCoord.w > 0.0f ) ? shadowCoord.xy / shadowCoord.w : float2( -1.0f, -1.0f );
	// Inside depth map
	if( shadowIndex.x >= 0.0f && shadowIndex.x <= 1.0f &&
		shadowIndex.y >= 0.0f && shadowIndex.y <= 1.0f )
	{
		//float4 depth = depthTexture.Sample( sampleType, shadowIndex );
		//if( shadowCoord.z > depth.z )
		//	shadow = 0.0f;

		float cDistance = clamp( ( shadowCoord.z - nearFarC.x ) / ( nearFarC.y - nearFarC.x ), 0.0f, 1.0f );
		float expNegCZ = exp( -nearFarC.z * cDistance );
		
		float expCDepth = depthTexture.Sample( sampleType, shadowIndex ).x;
		float expFactor  = expCDepth * expNegCZ;
		
		shadow = clamp( expFactor, 0.0f, 1.0f );
	}

	return shadow;
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
	float3 v = normalize( eye - p );
	float3 r = -reflect( l, n );
	float3 h = normalize( l + v );

	float4 diffuse = diffuseTexture.Sample( sampleType, uv );
	float4 specular = specularTexture.Sample( sampleType, uv );
	float roughness = specular.w;
	specular.w = 1.0f;

	float lightDistance = length( light - p );
	float shadow = ( castShadow == 0 || pTexel.w == 0.0f ) ? 1.0f : ExponentialShadow( p );
	shadow = ( lightRange < 0.0f || lightDistance <= lightRange ) ? shadow : 0.0f;

	// Attenuation
	float lightIntensity = ( lightRange <= 0.0f ) ? 1.0f : pow( 1.0f - saturate( lightDistance / lightRange ), 2.0f );
	
	//float4 lighting = max( dot( n, l ), 0.0f ) * diffuse * color + pow( max( dot( h, n ), 0.0f ), roughness ) * specular;
	float3 lighting = BRDF( l, v, h, n, diffuse.xyz, specular.xyz, roughness, color );

	return lightIntensity * shadow * float4( lighting.xyz, 1.0f );
}
