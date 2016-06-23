Texture2D inTexture  : register(t0);

SamplerState ClampSampler
{
	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = clamp;
	AddressV = clamp;
};

//RWStructuredBuffer< float4 > outTexture : register( u0 );
RWTexture2D< float4 > outTexture;

//#define MAX_KERNEL_RADIUS 3
//#define BATCH_X_NUM 128

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

groupshared float4 cache[BATCH_X_NUM + 2 * MAX_KERNEL_RADIUS];	// Variable shared with other threads in 128x1 thread group

// Declare thread group size
[numthreads( BATCH_X_NUM, 1, 1 )]
void HorizontalBlurMain( int3 dispatchThreadID : SV_DispatchThreadID, int3 groupThreadID : SV_GroupThreadID )
{
	// Set left pixels
	if ( groupThreadID.x < resolution.z )
	{
		float2 samplePos = float2( ( dispatchThreadID.x - resolution.z ) / ( float )resolution.x, dispatchThreadID.y / ( float )resolution.y );
		cache[groupThreadID.x] = inTexture.SampleLevel( ClampSampler, samplePos, 0 );
	}

	//set right pixels
	if ( ( BATCH_X_NUM - groupThreadID.x ) <= resolution.z )
	{
		float2 samplePos = float2( ( dispatchThreadID.x + resolution.z ) / ( float )resolution.x, dispatchThreadID.y / ( float )resolution.y );
		cache[ groupThreadID.x + 2 * resolution.z ] = inTexture.SampleLevel( ClampSampler, samplePos, 0 );
	}
	
	float2 samplePos = float2( dispatchThreadID.x / ( float )resolution.x, dispatchThreadID.y / ( float )resolution.y );

	cache[resolution.z + groupThreadID.x] = inTexture.SampleLevel( ClampSampler, samplePos, 0 );
	
	// Wait for all threads to finish sampling
	GroupMemoryBarrierWithGroupSync();

	float4 blurColor = float4( 0.0f, 0.0f, 0.0f, 0.0f );

	//[unroll]	// Unroll the loop until it stops excuting
	for ( int i = -resolution.z; i <= resolution.z; ++i )
	{
		blurColor += weights[i + resolution.z].x * cache[groupThreadID.x + i + resolution.z];
		//blurColor += ( 1.0f / ( 2 * resolution.z + 1 ) ) * cache[groupThreadID.x + i + resolution.z];
		//blurColor += 0.24203622f * cache[groupThreadID.x + i + resolution.z];
		//blurColor += weights[1] * cache[groupThreadID.x + i + resolution.z];
	}

	outTexture[dispatchThreadID.xy] = float4( blurColor.xyz, 1.0f );
}
