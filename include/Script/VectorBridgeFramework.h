#ifndef BH_VECTOR_BRIDGE_FRAMEWORK_H
#define BH_VECTOR_BRIDGE_FRAMEWORK_H

// Macros to declare BH::vector type bridge function
#define CREATE_VECTOR_DECL( vector )\
	__declspec( dllexport ) BH::vector * Create##vector();\
	__declspec( dllexport ) BH::vector * CreateFill##vector( float val );\
	__declspec( dllexport ) BH::vector * CreateCopy##vector( const BH::vector * v );

#define DESTROY_VECTOR_DECL( vector )\
	__declspec( dllexport ) void Destroy##vector( BH::vector * v );

#define ASSIGN_VECTOR_DECL( vector )\
	__declspec( dllexport ) void Assign##vector( BH::vector * dest, const BH::vector * src );\
	__declspec( dllexport ) void PlusAssign##vector( BH::vector * dest, const BH::vector * src );\
	__declspec( dllexport ) void MinusAssign##vector( BH::vector * dest, const BH::vector * src );\
	__declspec( dllexport ) void ScaleAssign##vector( BH::vector * dest, float s );\
	__declspec( dllexport ) void MulAssign##vector( BH::vector * dest, const BH::vector * src );\
	__declspec( dllexport ) void DivAssign##vector( BH::vector * dest, float s );

#define OPERATOR_VECTOR_DECL( vector )\
	__declspec( dllexport ) void Unary##vector( BH::vector * dest, const BH::vector * src );\
	__declspec( dllexport ) bool Equality##vector( const BH::vector * lhs, const BH::vector * rhs );\
	__declspec( dllexport ) bool NotEquality##vector( const BH::vector * lhs, const BH::vector * rhs );\
	__declspec( dllexport ) void Plus##vector ( BH::vector * dest, const BH::vector * lhs, const BH::vector * rhs );\
	__declspec( dllexport ) void Minus##vector ( BH::vector * dest, const BH::vector * lhs, const BH::vector * rhs );\
	__declspec( dllexport ) void Mul##vector ( BH::vector * dest, const BH::vector * lhs, const BH::vector * rhs );\
	__declspec( dllexport ) void Scale##vector ( BH::vector * dest, const BH::vector * lhs, float rhs );\
	__declspec( dllexport ) void Div##vector ( BH::vector * dest, const BH::vector * lhs, float rhs );

#define SPECIFIC_VECTOR_DECL\
	__declspec( dllexport ) float ScalarTripleProductVector2f( const BH::Vector2f * v1, const BH::Vector2f * v2 );\
	__declspec( dllexport ) void CrossVector3f( BH::Vector3f * v, const BH::Vector3f * v1, const BH::Vector3f * v2 );\
	__declspec( dllexport ) float GetXVector2f( const BH::Vector2f * v );\
	__declspec( dllexport ) float GetYVector2f( const BH::Vector2f * v );\
	__declspec( dllexport ) float GetXVector3f( const BH::Vector3f * v );\
	__declspec( dllexport ) float GetYVector3f( const BH::Vector3f * v );\
	__declspec( dllexport ) float GetZVector3f( const BH::Vector3f * v );\
	__declspec( dllexport ) float GetXVector4f( const BH::Vector4f * v );\
	__declspec( dllexport ) float GetYVector4f( const BH::Vector4f * v );\
	__declspec( dllexport ) float GetZVector4f( const BH::Vector4f * v );\
	__declspec( dllexport ) float GetWVector4f( const BH::Vector4f * v );\
	__declspec( dllexport ) void SetXVector2f( BH::Vector2f * v, float val );\
	__declspec( dllexport ) void SetYVector2f( BH::Vector2f * v, float val );\
	__declspec( dllexport ) void SetXVector3f( BH::Vector3f * v, float val );\
	__declspec( dllexport ) void SetYVector3f( BH::Vector3f * v, float val );\
	__declspec( dllexport ) void SetZVector3f( BH::Vector3f * v, float val );\
	__declspec( dllexport ) void SetXVector4f( BH::Vector4f * v, float val );\
	__declspec( dllexport ) void SetYVector4f( BH::Vector4f * v, float val );\
	__declspec( dllexport ) void SetZVector4f( BH::Vector4f * v, float val );\
	__declspec( dllexport ) void SetWVector4f( BH::Vector4f * v, float val );\
	__declspec( dllexport ) BH::Vector2f * CreateValVector2f( float x, float y );\
	__declspec( dllexport ) BH::Vector3f * CreateValVector3f( float x, float y, float z );\
	__declspec( dllexport ) BH::Vector4f * CreateValVector4f( float x, float y, float z, float w );

#define UTIL_VECTOR_DECL( vector )\
	__declspec( dllexport ) bool InBounds##vector( const BH::vector * v, const BH::vector * bounds );\
	__declspec( dllexport ) void Normalize##vector( BH::vector * v );\
	__declspec( dllexport ) float Length##vector( const BH::vector * v );\
	__declspec( dllexport ) float LengthSq##vector( const BH::vector * v );\
	__declspec( dllexport ) float Dot##vector( const BH::vector * v1, const BH::vector * v2 );\
	__declspec( dllexport ) void Clamp##vector( BH::vector * v, const BH::vector * min, const BH::vector * max );\
	__declspec( dllexport ) float Distance##vector( const BH::vector * v1, const BH::vector * v2 );\
	__declspec( dllexport ) float DistanceSquared##vector( const BH::vector * v1, const BH::vector * v2 );\
	__declspec( dllexport ) void Min##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2 );\
	__declspec( dllexport ) void Max##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2 );\
	__declspec( dllexport ) void Lerp##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2, float t );\
	__declspec( dllexport ) void SmoothStep##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2, float t );\
	__declspec( dllexport ) void Barycentric##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2, const BH::vector * v3, float f, float g );\
	__declspec( dllexport ) void CatmullRom##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2, const BH::vector * v3, const BH::vector * v4, float t );\
	__declspec( dllexport ) void Hermite##vector( BH::vector * v, const BH::vector * v1, const BH::vector * t1, const BH::vector * v2, const BH::vector * t2, float t );\
	__declspec( dllexport ) void Reflect##vector( BH::vector * v, const BH::vector * ivec, const BH::vector * nvec );\
	__declspec( dllexport ) void Refract##vector( BH::vector * v, const BH::vector * ivec, const BH::vector * nvec, float refractionIndex );

// Macros to define BH::vector type bridge function
#define CREATE_VECTOR_DEF( vector )\
	__declspec( dllexport ) BH::vector * Create##vector() { return new BH::vector(); }\
	__declspec( dllexport ) BH::vector * CreateFill##vector( float val ) { return new BH::vector( val ); }\
	__declspec( dllexport ) BH::vector * CreateCopy##vector( const BH::vector * v ) { return new BH::vector( *v ); }

#define DESTROY_VECTOR_DEF( vector )\
	__declspec( dllexport ) void Destroy##vector( BH::vector * v ) { delete v; }

#define ASSIGN_VECTOR_DEF( vector )\
	__declspec( dllexport ) void Assign##vector( BH::vector * dest, const BH::vector * src ) { *dest = *src; }\
	__declspec( dllexport ) void PlusAssign##vector( BH::vector * dest, const BH::vector * src ) { *dest += *src; }\
	__declspec( dllexport ) void MinusAssign##vector( BH::vector * dest, const BH::vector * src ) { *dest -= *src; }\
	__declspec( dllexport ) void ScaleAssign##vector( BH::vector * dest, float s ) { *dest *= s; }\
	__declspec( dllexport ) void MulAssign##vector( BH::vector * dest, const BH::vector * src ) { *dest *= *src; }\
	__declspec( dllexport ) void DivAssign##vector( BH::vector * dest, float s ) { *dest /= s; }

#define OPERATOR_VECTOR_DEF( vector )\
	__declspec( dllexport ) void Unary##vector( BH::vector * dest, const BH::vector * src ) { *dest = -*src; }\
	__declspec( dllexport ) bool Equality##vector( const BH::vector * lhs, const BH::vector * rhs ) { return *lhs == *rhs; }\
	__declspec( dllexport ) bool NotEquality##vector( const BH::vector * lhs, const BH::vector * rhs ) { return *lhs != *rhs; }\
	__declspec( dllexport ) void Plus##vector ( BH::vector * dest, const BH::vector * lhs, const BH::vector * rhs ) { *dest = *lhs + *rhs; }\
	__declspec( dllexport ) void Minus##vector ( BH::vector * dest, const BH::vector * lhs, const BH::vector * rhs ) { *dest = *lhs - *rhs; }\
	__declspec( dllexport ) void Mul##vector ( BH::vector * dest, const BH::vector * lhs, const BH::vector * rhs ) { *dest = *lhs * *rhs; }\
	__declspec( dllexport ) void Scale##vector ( BH::vector * dest, const BH::vector * lhs, float rhs ) { *dest = *lhs * rhs; }\
	__declspec( dllexport ) void Div##vector ( BH::vector * dest, const BH::vector * lhs, float rhs ) { *dest = *lhs / rhs; }

#define SPECIFIC_VECTOR_DEF\
	__declspec( dllexport ) float ScalarTripleProductVector2f( const BH::Vector2f * v1, const BH::Vector2f * v2 ) { return v1->ScalarTripleProduct( *v2 ); }\
	__declspec( dllexport ) void CrossVector3f( BH::Vector3f * v, const BH::Vector3f * v1, const BH::Vector3f * v2 ) { *v = v1->Cross( *v2 ); }\
	__declspec( dllexport ) float GetXVector2f( const BH::Vector2f * v ) { return v->x; }\
	__declspec( dllexport ) float GetYVector2f( const BH::Vector2f * v ) { return v->y; }\
	__declspec( dllexport ) float GetXVector3f( const BH::Vector3f * v ) { return v->x; }\
	__declspec( dllexport ) float GetYVector3f( const BH::Vector3f * v ) { return v->y; }\
	__declspec( dllexport ) float GetZVector3f( const BH::Vector3f * v ) { return v->z; }\
	__declspec( dllexport ) float GetXVector4f( const BH::Vector4f * v ) { return v->x; }\
	__declspec( dllexport ) float GetYVector4f( const BH::Vector4f * v ) { return v->y; }\
	__declspec( dllexport ) float GetZVector4f( const BH::Vector4f * v ) { return v->z; }\
	__declspec( dllexport ) float GetWVector4f( const BH::Vector4f * v ) { return v->w; }\
	__declspec( dllexport ) void SetXVector2f( BH::Vector2f * v, float val ) { v->x = val; }\
	__declspec( dllexport ) void SetYVector2f( BH::Vector2f * v, float val ) { v->y = val; }\
	__declspec( dllexport ) void SetXVector3f( BH::Vector3f * v, float val ) { v->x = val; }\
	__declspec( dllexport ) void SetYVector3f( BH::Vector3f * v, float val ) { v->y = val; }\
	__declspec( dllexport ) void SetZVector3f( BH::Vector3f * v, float val ) { v->z = val; }\
	__declspec( dllexport ) void SetXVector4f( BH::Vector4f * v, float val ) { v->x = val; }\
	__declspec( dllexport ) void SetYVector4f( BH::Vector4f * v, float val ) { v->y = val; }\
	__declspec( dllexport ) void SetZVector4f( BH::Vector4f * v, float val ) { v->z = val; }\
	__declspec( dllexport ) void SetWVector4f( BH::Vector4f * v, float val ) { v->w = val; }\
	__declspec( dllexport ) BH::Vector2f * CreateValVector2f( float x, float y ) { return new BH::Vector2f( x, y ); }\
	__declspec( dllexport ) BH::Vector3f * CreateValVector3f( float x, float y, float z ) { return new BH::Vector3f( x, y, z ); }\
	__declspec( dllexport ) BH::Vector4f * CreateValVector4f( float x, float y, float z, float w ) { return new BH::Vector4f( x, y, z, w ); }

#define UTIL_VECTOR_DEF( vector )\
	__declspec( dllexport ) bool InBounds##vector( const BH::vector * v, const BH::vector * bounds ) { return v->InBounds( *bounds ); }\
	__declspec( dllexport ) void Normalize##vector( BH::vector * v ) { v->Normalize(); }\
	__declspec( dllexport ) float Length##vector( const BH::vector * v ) { return v->Length(); }\
	__declspec( dllexport ) float LengthSq##vector( const BH::vector * v ) { return v->LengthSq(); }\
	__declspec( dllexport ) float Dot##vector( const BH::vector * v1, const BH::vector * v2 ) { return v1->Dot( *v2 ); }\
	__declspec( dllexport ) void Clamp##vector( BH::vector * v, const BH::vector * min, const BH::vector * max ) { v->Clamp( *min, *max ); }\
	__declspec( dllexport ) float Distance##vector( const BH::vector * v1, const BH::vector * v2 ) { return BH::vector::Distance( *v1, *v2 ); }\
	__declspec( dllexport ) float DistanceSquared##vector( const BH::vector * v1, const BH::vector * v2 ) { return BH::vector::DistanceSquared( *v1, *v2 ); }\
	__declspec( dllexport ) void Min##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2 ) { *v = BH::vector::Min( *v1, *v2 ); }\
	__declspec( dllexport ) void Max##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2 ) { *v = BH::vector::Max( *v1, *v2 ); }\
	__declspec( dllexport ) void Lerp##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2, float t ) { *v = BH::vector::Lerp( *v1, *v2, t ); }\
	__declspec( dllexport ) void SmoothStep##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2, float t ) { *v = BH::vector::SmoothStep( *v1, *v2, t ); }\
	__declspec( dllexport ) void Barycentric##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2, const BH::vector * v3, float f, float g ) { *v = BH::vector::Barycentric( *v1, *v2, *v3, f, g ); }\
	__declspec( dllexport ) void CatmullRom##vector( BH::vector * v, const BH::vector * v1, const BH::vector * v2, const BH::vector * v3, const BH::vector * v4, float t ) { *v = BH::vector::CatmullRom( *v1, *v2, *v3, *v4, t ); }\
	__declspec( dllexport ) void Hermite##vector( BH::vector * v, const BH::vector * v1, const BH::vector * t1, const BH::vector * v2, const BH::vector * t2, float t ) { *v = BH::vector::Hermite( *v1, *t1, *v2, *t2, t ); }\
	__declspec( dllexport ) void Reflect##vector( BH::vector * v, const BH::vector * ivec, const BH::vector * nvec ) { *v = BH::vector::Reflect( *ivec, *nvec ); }\
	__declspec( dllexport ) void Refract##vector( BH::vector * v, const BH::vector * ivec, const BH::vector * nvec, float refractionIndex ) { *v = BH::vector::Refract( *ivec, *nvec, refractionIndex ); }

#endif