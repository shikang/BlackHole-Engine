#include "constants.hlsl"

float3 HalfWayVector( float3 l, float3 v )
{
    return normalize( l + v );
}

float HeavisideStep(float v)
{
    return (v < 0.0f) ? 0.0f : 1.0f;
}

float4 Diffuse( float3 l, float3 n, float4 diffuse, float4 color )
{
	// @todo: Check if position is within light frustrum
    return diffuse * max( dot( n, l ), 0.0f ) * color;
}

float3 Diffuse( float3 l, float3 n, float3 diffuse, float3 color )
{
	// @todo: Check if position is within light frustrum
    return diffuse * max( dot( n, l ), 0.0f ) * color;
}

float4 PhongSpecular( float3 n, float3 h, float roughness, float4 specular )
{
    return pow( max( dot( n, h ), 0.0f ), roughness ) * specular;
}

float4 PhongSpecular( float3 n, float3 l, float3 v, float roughness, float4 specular )
{
    float3 h = HalfWayVector( l, v );

    return PhongSpecular( n, h, roughness, specular );
}

float4 Phong( float3 l, float3 n, float3 v, float4 diffuse, float roughness, float4 specular, float4 color )
{
    float4 d = Diffuse( l, n, diffuse, color );
    float4 s = PhongSpecular( n, l, v, roughness, specular );

    return d + s;
}

float BRDFDistribution( float3 h, float3 n, float roughness )
{
    return ( ( roughness + 2.0f ) / ( 2.0f * PI ) ) * pow( max( dot( h, n ), 0.0f ), roughness );
}

float3 BRDFFresnel( float3 l, float3 h, float3 specular )
{
    return specular + ( float3( 1.0f, 1.0f, 1.0f ) - specular ) * pow( 1 - max( dot( l, h ), 0.0f ), 5.0f );
}

float BRDFGeometry( float3 l, float3 h )
{
    //float geometry = 1.0f / pow( max( dot( l, h ), 0.01f ), 2.0f );
    return 1.0f;
}

float3 BRDFDiffuse( float3 l, float3 n, float3 diffuse, float3 color, float distribution, float3 fresnel, float geometry )
{
    return Diffuse( l, n, diffuse / PI + ( ( distribution * fresnel * geometry ) * 0.25f ), color );
}

float3 BRDF( float3 l, float3 v, float3 h, float3 n, float3 diffuse, float3 specular, float roughness, float3 color )
{
    float distribution = BRDFDistribution( h, n, roughness );
    float3 fresnel = BRDFFresnel( l, h, specular );
    float geometry = BRDFGeometry( l, h );

    //return ( diffuse / PI + ( ( distribution * fresnel * geometry ) * 0.25f ) ) * max( dot( n, l ), 0.0f ) * color;
    return BRDFDiffuse( l, n, diffuse, color, distribution, fresnel, geometry );
}

float3 BRDF( float3 l, float3 v, float3 n, float3 diffuse, float3 specular, float roughness, float3 color )
{
    // Calculate half vector
    float3 h = HalfWayVector( l, v );

    return BRDF( l, v, h, n, diffuse, specular, roughness, color );
}