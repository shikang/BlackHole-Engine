////////////////////////////////////////////////////////////////////////////////
// Filename: trace.ps
////////////////////////////////////////////////////////////////////////////////

Texture2D positionTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D diffuseTexture : register(t2);
Texture2D specularTexture : register(t3);
Texture2D depthTexture : register(t4);
Texture2D noiseTexture : register(t5);

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
	float sampleRate;
	float2 nearFar;
	float2 padding;
	float3 lightDimension;
	float useless;
};

cbuffer ShadowBuffer
{
	matrix shadowMatrix;
	matrix invPV;
};

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 endPosition : TEXCOORD0;
	float3 normal : NORMAL;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 TracePixelShader( PixelInputType input ) : SV_TARGET
{
	// Get world position
	float2 uv = float2( input.position.xy ) / float2( screenWidth, screenHeight );
	float4 posTexel = positionTexture.Sample( sampleType, uv );
	float3 pos = posTexel.xyz;

	if( posTexel.w == 0.0f )
	{
		// Set position to exit point of the light beam
		//pos = exitTraceTexture.Sample( sampleType, uv ).xyz;
		pos = input.endPosition;
	}

	// Calculate tracing distance
	float3 camForward = normalize( pos - eye );
	float3 nearPos = eye + camForward * nearFar.x;
	
	// Camera may be inside the beam
	float traceDistance = clamp( dot( pos - nearPos, camForward ), 0.0f, 2500.0f );	// Hardcode

	// Calculate Max shortest sample
	float sampleDistance = dot( abs( input.normal ), lightDimension );
	float maxSampleCount = sampleDistance * sampleRate;

	// Number of samples
	float3 sampleForward = camForward * 1.0f / sampleRate;
	float sampleLength = length( sampleForward );
	uint stepNum = min( traceDistance / sampleLength, 2000 );				// Hardcode

	// Brightness base factor
	float scaleStep = 0.01f;
	float scale = sampleLength * scaleStep;									// Hardcode
	//float scale = sampleLength * 0.0005f;

	// Jittering
	float jitter = noiseTexture.Sample( sampleType, uv ).x;

	// Step
	float3 positionStart = nearPos + jitter * sampleForward;
	float4 shadowCoord = mul( float4( positionStart.xyz, 1.0f ), shadowMatrix );
	float3 coordinates = shadowCoord.xyz / shadowCoord.w;
	coordinates.x = ( coordinates.x + 1.0f ) * 0.5f;
	coordinates.y = ( 1.0f - coordinates.y ) * 0.5f;
	//coordinates.z = dot( positionStart - light, lightDirection );

	float3 positionEnd = nearPos + ( 1.0f + jitter ) * sampleForward;
	shadowCoord = mul( float4( positionEnd.xyz, 1.0f ), shadowMatrix );
	float3 coordinateEnd = shadowCoord.xyz / shadowCoord.w;
	coordinateEnd.x = ( coordinateEnd.x + 1.0f ) * 0.5f;
	coordinateEnd.y = ( 1.0f - coordinateEnd.y ) * 0.5f;
	//coordinateEnd.z = dot( positionEnd - light, lightDirection );

	float3 coordinateDelta = coordinateEnd - coordinates;

	float intensity = 0.0f;

	[loop]
	for( uint i = 0; i < stepNum; ++i )
	{
		//float sampleFine = depthTexture.SampleCmpLevelZero( samplerPointLess, coordinates.xy, coordinates.z );
		
		float sampleFine = 0.0f;
		if( coordinates.x >= 0.0f && coordinates.x <= 1.0f &&
			coordinates.y >= 0.0f && coordinates.y <= 1.0f &&
			coordinates.z >= 0.0f && coordinates.z <= 1.0f )
		{
			float4 depth = depthTexture.SampleLevel( sampleType, coordinates.xy, 0 );
			if( coordinates.z <= depth.z )
			{
				sampleFine = 1.0f;

				// Attenuation
				//float lightIntensity = pow( 1.0f - saturate( coordinates.z / 1.0f ), 2.0f );
				//sampleFine *= lightIntensity;
			}
		}

		intensity += scale * sampleFine;
		coordinates += coordinateDelta;
	}

	
	// @todo : clamp to max value of distance from enter to exit
	intensity = clamp( intensity, 0.0f, ( float )( ( int )( maxSampleCount - 0.5f ) ) * scale );

	return float4( intensity, intensity, intensity, 1.0f );
	//return float4( jitter, jitter, jitter, 1.0f );
}
