#include "Script/VectorBridge.h"

#ifdef __cplusplus
extern "C"
{
#endif

	CREATE_VECTOR_DEF( Vector2f )
	CREATE_VECTOR_DEF( Vector3f )
	CREATE_VECTOR_DEF( Vector4f )
	
	DESTROY_VECTOR_DEF( Vector2f )
	DESTROY_VECTOR_DEF( Vector3f )
	DESTROY_VECTOR_DEF( Vector4f )
	
	ASSIGN_VECTOR_DEF( Vector2f )
	ASSIGN_VECTOR_DEF( Vector3f )
	ASSIGN_VECTOR_DEF( Vector4f )
	
	OPERATOR_VECTOR_DEF( Vector2f )
	OPERATOR_VECTOR_DEF( Vector3f )
	OPERATOR_VECTOR_DEF( Vector4f )
	
	UTIL_VECTOR_DEF( Vector2f )
	UTIL_VECTOR_DEF( Vector3f )
	UTIL_VECTOR_DEF( Vector4f )
	
	SPECIFIC_VECTOR_DEF

#ifdef __cplusplus
}
#endif