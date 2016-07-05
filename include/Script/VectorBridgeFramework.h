#ifndef BH_VECTOR_BRIDGE_FRAMEWORK_H
#define BH_VECTOR_BRIDGE_FRAMEWORK_H

#include "Platform/Platform.h"

// Macros to declare BH::vector type bridge function
#define CREATE_VECTOR_DECL( vector )\
	BH_DLL_EXPORT BH::vector * Create##vector();\
	BH_DLL_EXPORT BH::vector * CreateFill##vector( float val );\
	BH_DLL_EXPORT BH::vector * CreateCopy##vector( const BH::vector * v );

#define DESTROY_VECTOR_DECL( vector )\
	BH_DLL_EXPORT void Destroy##vector( BH::vector * v );

#define ASSIGN_VECTOR_DECL( vector )\
	BH_DLL_EXPORT void Assign##vector( BH::vector * dest, const BH::vector * src );\
	BH_DLL_EXPORT void PlusAssign##vector( BH::vector * dest, const BH::vector * src );\
	BH_DLL_EXPORT void MinusAssign##vector( BH::vector * dest, const BH::vector * src );\
	BH_DLL_EXPORT void ScaleAssign##vector( BH::vector * dest, float s );\
	BH_DLL_EXPORT void MulAssign##vector( BH::vector * dest, const BH::vector * src );\
	BH_DLL_EXPORT void DivAssign##vector( BH::vector * dest, float s );

#define OPERATOR_VECTOR_DECL( vector )\
	BH_DLL_EXPORT void Unary##vector( BH::vector * dest, const BH::vector * src );\
	BH_DLL_EXPORT bool Equality##vector( const BH::vector * lhs, const BH::vector * rhs );\
	BH_DLL_EXPORT bool NotEquality##vector( const BH::vector * lhs, const BH::vector * rhs );\
	BH_DLL_EXPORT void Plus##vector ( BH::vector * dest, const BH::vector * lhs, const BH::vector * rhs );\
	BH_DLL_EXPORT void Minus##vector ( BH::vector * dest, const BH::vector * lhs, const BH::vector * rhs );\
	BH_DLL_EXPORT void Mul##vector ( BH::vector * dest, const BH::vector * lhs, const BH::vector * rhs );\
	BH_DLL_EXPORT void Scale##vector ( BH::vector * dest, const BH::vector * lhs, float rhs );\
	BH_DLL_EXPORT void Div##vector ( BH::vector * dest, const BH::vector * lhs, float rhs );

#define SPECIFIC_VECTOR_DECL\
	BH_DLL_EXPORT float ScalarTripleProductVector2f( const BH::Vector2f * v1, const BH::Vector2f * v2 );\
	BH_DLL_EXPORT void CrossVector3f( BH::Vector3f * v, const BH::Vector3f * v1, const BH::Vector3f * v2 );\
	BH_DLL_EXPORT float GetXVector2f( const BH::Vector2f * v );\
	BH_DLL_EXPORT float GetYVector2f( const BH::Vector2f * v );\
	BH_DLL_EXPORT float GetXVector3f( const BH::Vector3f * v );\
	BH_DLL_EXPORT float GetYVector3f( const BH::Vector3f * v );\
	BH_DLL_EXPORT float GetZVector3f( const BH::Vector3f * v );\
	BH_DLL_EXPORT float GetXVector4f( const BH::Vector4f * v );\
	BH_DLL_EXPORT float GetYVector4f( const BH::Vector4f * v );\
	BH_DLL_EXPORT float GetZVector4f( const BH::Vector4f * v );\
	BH_DLL_EXPORT float GetWVector4f( const BH::Vector4f * v );\
	BH_DLL_EXPORT void SetXVector2f( BH::Vector2f * v, float val );\
	BH_DLL_EXPORT void SetYVector2f( BH::Vector2f * v, float val );\
	BH_DLL_EXPORT void SetXVector3f( BH::Vector3f * v, float val );\
	BH_DLL_EXPORT void SetYVector3f( BH::Vector3f * v, float val );\
	BH_DLL_EXPORT void SetZVector3f( BH::Vector3f * v, float val );\
	BH_DLL_EXPORT void SetXVector4f( BH::Vector4f * v, float val );\
	BH_DLL_EXPORT void SetYVector4f( BH::Vector4f * v, float val );\
	BH_DLL_EXPORT void SetZVector4f( BH::Vector4f * v, float val );\
	BH_DLL_EXPORT void SetWVector4f( BH::Vector4f * v, float val );\
	BH_DLL_EXPORT BH::Vector2f * CreateValVector2f( float x, float y );\
	BH_DLL_EXPORT BH::Vector3f * CreateValVector3f( float x, float y, float z );\
	BH_DLL_EXPORT BH::Vector4f * CreateValVector4f( float x, float y, float z, float w );

#define UTIL_VECTOR_DECL( vector )\
	BH_DLL_EXPORT bool InBounds##vector( const BH::vector * v, const BH::vector * bounds );\
	BH_DLL_EXPORT void Normalize##vector( BH::vector * v );\
	BH_DLL_EXPORT float Length##vector( const BH::vector * v );\
	BH_DLL_EXPORT float LengthSq##vector( const BH::vector * v );\
	BH_DLL_EXPORT float Dot##vector( const BH::vector * v1, const BH::vector * v2 );\
	BH_DLL_EXPORT void Clamp##vector( BH::vector * v, const BH::vector * min, const BH::vector * max );\
	BH_DLL_EXPORT float Distance##vector( const BH::vector * v1, const BH::vector * v2 );\
	BH_DLL_EXPORT float DistanceSquared##vector( const BH::vector * v1, const BH::vector * v2 );\
	BH_DLL_EXPORT void Min##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2 );\
	BH_DLL_EXPORT void Max##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2 );\
	BH_DLL_EXPORT void Lerp##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2, float t );\
	BH_DLL_EXPORT void SmoothStep##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2, float t );\
	BH_DLL_EXPORT void Barycentric##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2, const BH::vector * v3, float f, float g );\
	BH_DLL_EXPORT void CatmullRom##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2, const BH::vector * v3, const BH::vector * v4, float t );\
	BH_DLL_EXPORT void Hermite##vector( BH::vector * v, const BH::vector * v1, const BH::vector * t1, const BH::vector * v2, const BH::vector * t2, float t );\
	BH_DLL_EXPORT void Reflect##vector( BH::vector * v, const BH::vector * ivec, const BH::vector * nvec );\
	BH_DLL_EXPORT void Refract##vector( BH::vector * v, const BH::vector * ivec, const BH::vector * nvec, float refractionIndex );

// Macros to define BH::vector type bridge function
#define CREATE_VECTOR_DEF( vector )\
	BH_DLL_EXPORT BH::vector * Create##vector() { return new BH::vector(); }\
	BH_DLL_EXPORT BH::vector * CreateFill##vector( float val ) { return new BH::vector( val ); }\
	BH_DLL_EXPORT BH::vector * CreateCopy##vector( const BH::vector * v ) { return new BH::vector( *v ); }

#define DESTROY_VECTOR_DEF( vector )\
	BH_DLL_EXPORT void Destroy##vector( BH::vector * v ) { delete v; }

#define ASSIGN_VECTOR_DEF( vector )\
	BH_DLL_EXPORT void Assign##vector( BH::vector * dest, const BH::vector * src ) { *dest = *src; }\
	BH_DLL_EXPORT void PlusAssign##vector( BH::vector * dest, const BH::vector * src ) { *dest += *src; }\
	BH_DLL_EXPORT void MinusAssign##vector( BH::vector * dest, const BH::vector * src ) { *dest -= *src; }\
	BH_DLL_EXPORT void ScaleAssign##vector( BH::vector * dest, float s ) { *dest *= s; }\
	BH_DLL_EXPORT void MulAssign##vector( BH::vector * dest, const BH::vector * src ) { *dest *= *src; }\
	BH_DLL_EXPORT void DivAssign##vector( BH::vector * dest, float s ) { *dest /= s; }

#define OPERATOR_VECTOR_DEF( vector )\
	BH_DLL_EXPORT void Unary##vector( BH::vector * dest, const BH::vector * src ) { *dest = -*src; }\
	BH_DLL_EXPORT bool Equality##vector( const BH::vector * lhs, const BH::vector * rhs ) { return *lhs == *rhs; }\
	BH_DLL_EXPORT bool NotEquality##vector( const BH::vector * lhs, const BH::vector * rhs ) { return *lhs != *rhs; }\
	BH_DLL_EXPORT void Plus##vector ( BH::vector * dest, const BH::vector * lhs, const BH::vector * rhs ) { *dest = *lhs + *rhs; }\
	BH_DLL_EXPORT void Minus##vector ( BH::vector * dest, const BH::vector * lhs, const BH::vector * rhs ) { *dest = *lhs - *rhs; }\
	BH_DLL_EXPORT void Mul##vector ( BH::vector * dest, const BH::vector * lhs, const BH::vector * rhs ) { *dest = *lhs * *rhs; }\
	BH_DLL_EXPORT void Scale##vector ( BH::vector * dest, const BH::vector * lhs, float rhs ) { *dest = *lhs * rhs; }\
	BH_DLL_EXPORT void Div##vector ( BH::vector * dest, const BH::vector * lhs, float rhs ) { *dest = *lhs / rhs; }

#define SPECIFIC_VECTOR_DEF\
	BH_DLL_EXPORT float ScalarTripleProductVector2f( const BH::Vector2f * v1, const BH::Vector2f * v2 ) { return v1->ScalarTripleProduct( *v2 ); }\
	BH_DLL_EXPORT void CrossVector3f( BH::Vector3f * v, const BH::Vector3f * v1, const BH::Vector3f * v2 ) { *v = v1->Cross( *v2 ); }\
	BH_DLL_EXPORT float GetXVector2f( const BH::Vector2f * v ) { return v->x; }\
	BH_DLL_EXPORT float GetYVector2f( const BH::Vector2f * v ) { return v->y; }\
	BH_DLL_EXPORT float GetXVector3f( const BH::Vector3f * v ) { return v->x; }\
	BH_DLL_EXPORT float GetYVector3f( const BH::Vector3f * v ) { return v->y; }\
	BH_DLL_EXPORT float GetZVector3f( const BH::Vector3f * v ) { return v->z; }\
	BH_DLL_EXPORT float GetXVector4f( const BH::Vector4f * v ) { return v->x; }\
	BH_DLL_EXPORT float GetYVector4f( const BH::Vector4f * v ) { return v->y; }\
	BH_DLL_EXPORT float GetZVector4f( const BH::Vector4f * v ) { return v->z; }\
	BH_DLL_EXPORT float GetWVector4f( const BH::Vector4f * v ) { return v->w; }\
	BH_DLL_EXPORT void SetXVector2f( BH::Vector2f * v, float val ) { v->x = val; }\
	BH_DLL_EXPORT void SetYVector2f( BH::Vector2f * v, float val ) { v->y = val; }\
	BH_DLL_EXPORT void SetXVector3f( BH::Vector3f * v, float val ) { v->x = val; }\
	BH_DLL_EXPORT void SetYVector3f( BH::Vector3f * v, float val ) { v->y = val; }\
	BH_DLL_EXPORT void SetZVector3f( BH::Vector3f * v, float val ) { v->z = val; }\
	BH_DLL_EXPORT void SetXVector4f( BH::Vector4f * v, float val ) { v->x = val; }\
	BH_DLL_EXPORT void SetYVector4f( BH::Vector4f * v, float val ) { v->y = val; }\
	BH_DLL_EXPORT void SetZVector4f( BH::Vector4f * v, float val ) { v->z = val; }\
	BH_DLL_EXPORT void SetWVector4f( BH::Vector4f * v, float val ) { v->w = val; }\
	BH_DLL_EXPORT BH::Vector2f * CreateValVector2f( float x, float y ) { return new BH::Vector2f( x, y ); }\
	BH_DLL_EXPORT BH::Vector3f * CreateValVector3f( float x, float y, float z ) { return new BH::Vector3f( x, y, z ); }\
	BH_DLL_EXPORT BH::Vector4f * CreateValVector4f( float x, float y, float z, float w ) { return new BH::Vector4f( x, y, z, w ); }

#define UTIL_VECTOR_DEF( vector )\
	BH_DLL_EXPORT bool InBounds##vector( const BH::vector * v, const BH::vector * bounds ) { return v->InBounds( *bounds ); }\
	BH_DLL_EXPORT void Normalize##vector( BH::vector * v ) { v->Normalize(); }\
	BH_DLL_EXPORT float Length##vector( const BH::vector * v ) { return v->Length(); }\
	BH_DLL_EXPORT float LengthSq##vector( const BH::vector * v ) { return v->LengthSq(); }\
	BH_DLL_EXPORT float Dot##vector( const BH::vector * v1, const BH::vector * v2 ) { return v1->Dot( *v2 ); }\
	BH_DLL_EXPORT void Clamp##vector( BH::vector * v, const BH::vector * min, const BH::vector * max ) { v->Clamp( *min, *max ); }\
	BH_DLL_EXPORT float Distance##vector( const BH::vector * v1, const BH::vector * v2 ) { return BH::vector::Distance( *v1, *v2 ); }\
	BH_DLL_EXPORT float DistanceSquared##vector( const BH::vector * v1, const BH::vector * v2 ) { return BH::vector::DistanceSquared( *v1, *v2 ); }\
	BH_DLL_EXPORT void Min##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2 ) { *v = BH::vector::Min( *v1, *v2 ); }\
	BH_DLL_EXPORT void Max##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2 ) { *v = BH::vector::Max( *v1, *v2 ); }\
	BH_DLL_EXPORT void Lerp##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2, float t ) { *v = BH::vector::Lerp( *v1, *v2, t ); }\
	BH_DLL_EXPORT void SmoothStep##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2, float t ) { *v = BH::vector::SmoothStep( *v1, *v2, t ); }\
	BH_DLL_EXPORT void Barycentric##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2, const BH::vector * v3, float f, float g ) { *v = BH::vector::Barycentric( *v1, *v2, *v3, f, g ); }\
	BH_DLL_EXPORT void CatmullRom##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2, const BH::vector * v3, const BH::vector * v4, float t ) { *v = BH::vector::CatmullRom( *v1, *v2, *v3, *v4, t ); }\
	BH_DLL_EXPORT void Hermite##vector( BH::vector * v, const BH::vector * v1, const BH::vector * t1, const BH::vector * v2, const BH::vector * t2, float t ) { *v = BH::vector::Hermite( *v1, *t1, *v2, *t2, t ); }\
	BH_DLL_EXPORT void Reflect##vector( BH::vector * v, const BH::vector * ivec, const BH::vector * nvec ) { *v = BH::vector::Reflect( *ivec, *nvec ); }\
	BH_DLL_EXPORT void Refract##vector( BH::vector * v, const BH::vector * ivec, const BH::vector * nvec, float refractionIndex ) { *v = BH::vector::Refract( *ivec, *nvec, refractionIndex ); }

#endif