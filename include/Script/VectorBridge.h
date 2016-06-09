#ifndef BH_VECTOR_BRIDGE_H
#define BH_VECTOR_BRIDGE_H

#include "Core/CoreEssential.h"

#include "Platform/Vector2f.h"
#include "Platform/Vector3f.h"
#include "Platform/Vector4f.h"

#include "Script/VectorBridgeFramework.h"

#ifdef __cplusplus
extern "C"
{
#endif

	CREATE_VECTOR_DECL( Vector2f )
	CREATE_VECTOR_DECL( Vector3f )
	CREATE_VECTOR_DECL( Vector4f )

	DESTROY_VECTOR_DECL( Vector2f )
	DESTROY_VECTOR_DECL( Vector3f )
	DESTROY_VECTOR_DECL( Vector4f )

	ASSIGN_VECTOR_DECL( Vector2f )
	ASSIGN_VECTOR_DECL( Vector3f )
	ASSIGN_VECTOR_DECL( Vector4f )

	OPERATOR_VECTOR_DECL( Vector2f )
	OPERATOR_VECTOR_DECL( Vector3f )
	OPERATOR_VECTOR_DECL( Vector4f )

	UTIL_VECTOR_DECL( Vector2f )
	UTIL_VECTOR_DECL( Vector3f )
	UTIL_VECTOR_DECL( Vector4f )

	SPECIFIC_VECTOR_DECL

#ifdef __cplusplus
}
#endif
#endif