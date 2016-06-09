////////////////////////////////////////////////////////////////////////////////
// Filename: depth.ps
////////////////////////////////////////////////////////////////////////////////

cbuffer ConstantBuffer
{
	float3 nearFarC;
	float padding;
};

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
	float4 shadowPosition : TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 DepthPixelShader( PixelInputType input ) : SV_TARGET
{
	//return float4( input.shadowPosition.z / input.shadowPosition.w, input.shadowPosition.z / input.shadowPosition.w, input.shadowPosition.z / input.shadowPosition.w, 1.0f );
	//return input.shadowPosition;

	float d = input.shadowPosition.z;
	float depth = clamp( ( d - nearFarC.x ) / ( nearFarC.y - nearFarC.x ), 0.0f, 1.0f );
	float exponentialDepth = exp( ( float )nearFarC.z * depth );
	return float4( exponentialDepth, 0.0f, 0.0f, 1.0f );
}
