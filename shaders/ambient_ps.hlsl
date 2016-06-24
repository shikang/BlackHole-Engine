////////////////////////////////////////////////////////////////////////////////
// Filename: ambient.ps
////////////////////////////////////////////////////////////////////////////////
#include "lightingFunctions.hlsl"

Texture2D positionTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D diffuseTexture : register(t2);
Texture2D specularTexture : register(t3);
Texture2D skyTexture : register(t4);
Texture2D irradianceTexture : register(t5);
Texture2D ambientTexture : register(t6);

SamplerState sampleType : register(s0);

/////////////
// GLOBALS //
/////////////
cbuffer AmbientBuffer
{
	float4 ambient;
	float3 eye;
	float skyDimension;
};

cbuffer RandomBuffer
{
	float4 hammersley[IBL_N];
};

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 AmbientPixelShader( PixelInputType input ) : SV_TARGET
{
	//return ambient * diffuseTexture.Sample( sampleType, input.tex );

	float twoPi = 2.0f * PI;

	// Getting color datas
	float3 color = pow( diffuseTexture.Sample( sampleType, input.tex ).xyz, 2.2f );
	float4 specAndRough = specularTexture.Sample( sampleType, input.tex );
	float3 s = pow( specAndRough.xyz, 2.2f );

	// Getting other datas
	float3 p = positionTexture.Sample( sampleType, input.tex ).xyz;
	float3 normal = normalTexture.Sample( sampleType, input.tex ).xyz;
	float3 v = normalize( eye - p );
	float roughness = specAndRough.w;
	float a = ambientTexture.Sample( sampleType, input.tex ).x;

	// Diffuse term
	float2 uv = float2( 0.5f - ( atan2( normal.z, normal.x ) / ( twoPi ) ), acos( normal.y ) / PI );
	//float2 uv = float2( 0.5f + ( atan2( normal.z, normal.x ) / ( twoPi ) ), 0.5f - ( asin( normal.y ) / PI ) );
	float3 diffuse = pow( ambient.xyz, 2.2f ) * color.xyz / PI * irradianceTexture.SampleLevel( sampleType, uv, 0 ).xyz;

	// Specular term
	float3 R = 2.0f * dot( normal, v ) * normal - v;
	float3 A = normalize( cross( float3( 0.0f, 0.0f, 1.0f ), R ) );
	float3 B = normalize( cross( R, A ) );

	float3 specular = float3( 0.0f, 0.0f, 0.0f );
	int N = 1;
	for( int i = 0; i < N; ++i )
	{
		float2 tex = float2( hammersley[i].x, 
							 acos( pow( hammersley[i].y, 1.0f / ( roughness + 1.0f ) ) ) / PI );

		float3 rl = float3( cos( twoPi * ( 0.5f - tex.x ) ) * sin( PI * tex.y ),
							cos( PI * tex.y ),
						    sin( twoPi * ( 0.5f - tex.x ) ) * sin( PI * tex.y )
						     );

		float3 l = normalize( rl.x * A + rl.z * B + rl.y * R );

		float3 h = HalfWayVector( l, v );
		
		// BRDF
		//float distribution = ( ( roughness + 2.0f ) / ( twoPi ) ) * pow( max( dot( h, normal ), 0.0f ), roughness );
        float distribution = BRDFDistribution( h, normal, roughness );
		//float3 fresnel = s + ( float3( 1.0f, 1.0f, 1.0f ) - s ) * pow( 1 - max( dot( l, h ), 0.0f ), 5.0f );
        float3 fresnel = BRDFFresnel( l, h, s );
		//float geometry = 1.0f / pow( max( dot( l, h ), 0.01f ), 2.0f );
		//float geometry = 1.0f;
        float geometry = BRDFGeometry( l, h );

		float level = 0.5f * log2( skyDimension / N ) - 0.5f * log2( distribution );

		float2 luv = float2( 0.5f - ( atan2( l.z, l.x ) / ( twoPi ) ), acos( l.y ) / PI );
		//float2 luv = float2( 0.5f + ( atan2( l.z, l.x ) / ( twoPi ) ), 0.5f - ( asin( l.y ) / PI ) );

        // Distribution term is gotten from sky texture
		specular += pow( ambient.xyz, 2.2f ) * ( geometry * fresnel * 0.25f ) * max( dot( normal, l ), 0.0f ) * skyTexture.SampleLevel( sampleType, luv, level ).xyz;
	}
	specular /= N;

	//return float4( pow( diffuse.xyz, 1.0f / 2.2f ), 1.0f );
	//return float4( pow( specular.xyz, 1.0f / 2.2f ), 1.0f );
	return float4( a * pow( diffuse.xyz + specular.xyz, 1.0f / 2.2f ), 1.0f );
	//return float4( ambient.xyz * color.xyz, 1.0f  );
}
