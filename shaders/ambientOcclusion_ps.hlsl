////////////////////////////////////////////////////////////////////////////////
// Filename: ambientOcclusion.ps
////////////////////////////////////////////////////////////////////////////////
#include "lightingFunctions.hlsl"

Texture2D positionTexture : register(t0);
Texture2D normalTexture : register(t1);

SamplerState sampleType : register(s0);

/////////////
// GLOBALS //
/////////////
cbuffer AmbientBuffer
{
	float rangeOfInfluence;
	float scaleFactor;
	float contrast;
	float padding;
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
// Helper Functions
////////////////////////////////////////////////////////////////////////////////
/*
float HeavisideStep( float v )
{
	return ( v < 0.0f ) ? 0.0f : 1.0f;
}
*/

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 AOPixelShader( PixelInputType input ) : SV_TARGET
{
	// Get information from g-buffer
	if( positionTexture.Sample( sampleType, input.tex ).w == 0.0f )
		return float4( 1.0f, 1.0f, 1.0f, 1.0f );

	float3 P = positionTexture.Sample( sampleType, input.tex ).xyz;
	float3 N = normalTexture.Sample( sampleType, input.tex ).xyz;
	float d = normalTexture.Sample( sampleType, input.tex ).w;

	// Random select points for sampling
	int2 frag = int2( ( int )input.position.x, ( int )input.position.y );
	int phi = ( 30 * ( frag.x ^ frag.y ) ) + ( 10 * frag.x * frag.y );

	float c = 0.1f * rangeOfInfluence;
	float cSq = c * c;
	float delta = 0.001f;
	float n = ( float )AO_N;

	float S = 0.0f;

	for( int i = 0; i < AO_N; ++i )
	{
		float alpha = ( ( float )i + 0.5f ) / n;	// Range [0, 1]
		float h = alpha * rangeOfInfluence / d;
		float theta = 2.0f * PI * alpha * ( 7.0f * n / 9.0f ) + ( float )phi;

		float2 samplePoint = input.tex + h * float2( cos( theta ), sin( theta ) );

		// If no position, assume HeavisideStep = 0.0f
		if( positionTexture.Sample( sampleType, samplePoint ).w == 0.0f )
			continue;

		float3 Pi = positionTexture.Sample( sampleType, samplePoint ).xyz;
		float di = normalTexture.Sample( sampleType, samplePoint ).w;

		float3 omegai = Pi - P;
		S += ( max( dot( N, omegai ) - delta * di, 0.0f ) * HeavisideStep( rangeOfInfluence - length( omegai ) ) ) / ( max( cSq, dot( omegai, omegai ) ) );
	}

	S *= ( 2.0f * PI * c / n );
	float A = max( 1.0f - scaleFactor * S, 0.0f );
	A = pow( A, contrast );

	return float4( A, A, A, 1.0f );
}
