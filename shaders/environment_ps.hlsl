////////////////////////////////////////////////////////////////////////////////
// Filename: environment.ps
////////////////////////////////////////////////////////////////////////////////

Texture2D environmentMap : register(t0);
SamplerState sampleType : register(s0);

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
	float3 viewPosition : TEXCOORD0;
	float3 viewNormal : NORMAL;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 DepthPixelShader( PixelInputType input ) : SV_TARGET
{
	// Normalize view vector
	float3 u = normalize( input.viewPosition );

	// Reflect u about the plane defined by the normal of fragment
	float3 r = reflect( u, normalize( input.viewNormal ) );

	// Compute texture coordinate from reflection vector
	float2 tc;
	tc.y = r.y;
	r.y = 0.0f;
	tc.x = normalize( r ).x * 0.5f;

	// Scale and bias texture coordinate based on direction of reflection vector
	float s = sign( r.z ) * 0.5f;
	tc.x = 0.75 - s * ( 0.5 - tc.x );
	tc.y = 0.5 + 0.5 * tc.y;

	// Sample from scaled and biased texture coordinate
	return environmentMap.Sample( sampleType, tc );
}
