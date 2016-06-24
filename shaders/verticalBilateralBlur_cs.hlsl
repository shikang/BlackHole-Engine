Texture2D inTexture : register(t0);
Texture2D ndTexture : register(t1);

#include "constants.hlsl"

SamplerState ClampSampler
{
	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = clamp;
	AddressV = clamp;
};

//RWStructuredBuffer< float4 > outTexture : register( u0 );
RWTexture2D< float4 > outTexture;

//#define MAX_KERNEL_RADIUS 3
//#define BATCH_Y_NUM 128

cbuffer ResolutionBuffer
{
	int3 resolution;
	int padding;
};

cbuffer WeightBuffer
{
	//float weights[2 * MAX_KERNEL_RADIUS + 1];		//!< Padding screw me up http://stackoverflow.com/questions/14711763/hlsl-float-array-packing-in-constant-buffer
	float4 weights[2 * MAX_KERNEL_RADIUS + 1];
};

groupshared float4 cache[BATCH_Y_NUM + 2 * MAX_KERNEL_RADIUS];		// Variable shared with other threads in 128x1 thread group
groupshared float4 ndCache[BATCH_X_NUM + 2 * MAX_KERNEL_RADIUS];	// Variable shared with other threads in 128x1 thread group

// Declare thread group size
[numthreads( 1, BATCH_Y_NUM, 1 )]
void VerticalBlurMain( int3 dispatchThreadID : SV_DispatchThreadID, int3 groupThreadID : SV_GroupThreadID )
{
	// Set left pixels
	if ( groupThreadID.y < resolution.z )
	{
		float2 samplePos = float2( ( float )dispatchThreadID.x / ( float )resolution.x, ( float )( dispatchThreadID.y - resolution.z ) / ( float )resolution.y );
		cache[groupThreadID.y] = inTexture.SampleLevel( ClampSampler, samplePos, 0 );
		ndCache[groupThreadID.y] = ndTexture.SampleLevel( ClampSampler, samplePos, 0 );
	}

	//set right pixels
	if ( ( BATCH_Y_NUM - groupThreadID.y ) <= resolution.z )
	{
		float2 samplePos = float2( ( float )dispatchThreadID.x / ( float )resolution.x, ( float )( dispatchThreadID.y + resolution.z ) / ( float )resolution.y );
		cache[ groupThreadID.y + 2 * resolution.z ] = inTexture.SampleLevel( ClampSampler, samplePos, 0 );
		ndCache[ groupThreadID.y + 2 * resolution.z ] = ndTexture.SampleLevel( ClampSampler, samplePos, 0 );
	}
	
	float2 samplePos = float2( ( float )dispatchThreadID.x / ( float )resolution.x, ( float )dispatchThreadID.y / ( float )resolution.y );

	cache[resolution.z + groupThreadID.y] = inTexture.SampleLevel( ClampSampler, samplePos, 0 );
	ndCache[resolution.z + groupThreadID.y] = ndTexture.SampleLevel( ClampSampler, samplePos, 0 );
	
	// Wait for all threads to finish sampling
	GroupMemoryBarrierWithGroupSync();

	float3 N = ndCache[resolution.z + groupThreadID.y].xyz;
	float d = ndCache[resolution.z + groupThreadID.y].w;
	// If normal = zero vector. (Empty space)
	if ( dot( N, N ) == 0.0f )
	{
		outTexture[dispatchThreadID.xy] = cache[resolution.z + groupThreadID.y];
		return;
	}

	float4 blurColor = float4( 0.0f, 0.0f, 0.0f, 0.0f );
	float s = 0.01f;
	float f = 1.0f / sqrt( 2.0f * PI * s );
	float sumWeight = 0.0f;

	//[unroll]	// Unroll the loop until it stops excuting
	for ( int i = -resolution.z; i <= resolution.z; ++i )
	{
		float exponent = -pow( ndCache[groupThreadID.y + i + resolution.z].w - ndCache[resolution.z + groupThreadID.y].w, 2.0f ) / ( 2.0f * s );
		float r = dot( ndCache[groupThreadID.y + i + resolution.z].xyz, ndCache[resolution.z + groupThreadID.y].xyz ) + f * exp( exponent );
		float w = r * weights[i + resolution.z].x;
		blurColor += w * cache[groupThreadID.y + i + resolution.z];
		sumWeight += w;
	}

	outTexture[dispatchThreadID.xy] = float4( blurColor.xyz / sumWeight, 1.0f );
}
