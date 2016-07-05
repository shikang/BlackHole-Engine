#ifndef BH_QUATERNION_BRIDGE_H
#define BH_QUATERNION_BRIDGE_H

#include "Platform/Platform.h"
#include "Core/CoreEssential.h"
#include "Platform/Quaternion.h"

#ifdef __cplusplus
extern "C"
{
#endif

	// Create quaternion
	BH_DLL_EXPORT BH::Quaternion * CreateQuaternion();

	// Create fill quaternion
	BH_DLL_EXPORT BH::Quaternion * CreateValQuaternion( float x, float y, float z, float w );

	// Create quaternion from vector
	BH_DLL_EXPORT BH::Quaternion * CreateVectorQuaternion( const BH::Vector3f * v, float scalar );

	// Create copy quaternion
	BH_DLL_EXPORT BH::Quaternion * CreateCopyQuaternion( const BH::Quaternion * q );

	// Destroy quaternion
	BH_DLL_EXPORT void DestroyQuaternion( BH::Quaternion * q );

	// Assignment functions
	BH_DLL_EXPORT void AssignQuaternion( BH::Quaternion * dest, const BH::Quaternion * src );
	BH_DLL_EXPORT void PlusAssignQuaternion( BH::Quaternion * dest, const BH::Quaternion * src );
	BH_DLL_EXPORT void MinusAssignQuaternion( BH::Quaternion * dest, const BH::Quaternion * src );
	BH_DLL_EXPORT void ScaleAssignQuaternion( BH::Quaternion * dest, float s );
	BH_DLL_EXPORT void MulAssignQuaternion( BH::Quaternion * dest, const BH::Quaternion * src );
	BH_DLL_EXPORT void DivAssignQuaternion( BH::Quaternion * dest, const BH::Quaternion * src );

	// Operator functions
	BH_DLL_EXPORT void UnaryQuaternion( BH::Quaternion * dest, const BH::Quaternion * src );
	BH_DLL_EXPORT bool EqualityQuaternion( const BH::Quaternion * lhs, const BH::Quaternion * rhs );
	BH_DLL_EXPORT bool NotEqualityQuaternion( const BH::Quaternion * lhs, const BH::Quaternion * rhs );
	BH_DLL_EXPORT void PlusQuaternion ( BH::Quaternion * dest, const BH::Quaternion * lhs, const BH::Quaternion * rhs );
	BH_DLL_EXPORT void MinusQuaternion ( BH::Quaternion * dest, const BH::Quaternion * lhs, const BH::Quaternion * rhs );
	BH_DLL_EXPORT void MulQuaternion ( BH::Quaternion * dest, const BH::Quaternion * lhs, const BH::Quaternion * rhs );
	BH_DLL_EXPORT void RotVecQuaternion ( BH::Vector3f * dest, const BH::Quaternion * lhs, const BH::Vector3f * rhs );
	BH_DLL_EXPORT void DivQuaternion ( BH::Quaternion * dest, const BH::Quaternion * lhs, const BH::Quaternion * rhs );

	// Utility functions
	BH_DLL_EXPORT float LengthQuaternion( const BH::Quaternion * q );
	BH_DLL_EXPORT float LengthSqQuaternion( const BH::Quaternion * q );
	BH_DLL_EXPORT void NormalizeQuaternion( BH::Quaternion * q );
	BH_DLL_EXPORT void ConjugateQuaternion( BH::Quaternion * q );
	BH_DLL_EXPORT void InverseQuaternion( BH::Quaternion * q );
	BH_DLL_EXPORT float DotQuaternion( const BH::Quaternion * lhs, const BH::Quaternion * rhs );
	BH_DLL_EXPORT void CrossQuaternion( BH::Quaternion * dest, const BH::Quaternion * lhs, const BH::Quaternion * rhs );
	BH_DLL_EXPORT void AxisAngleQuaternion( BH::Quaternion * dest, const BH::Vector3f * axis, float angle );
	BH_DLL_EXPORT void YawPitchRollQuaternion( BH::Quaternion * dest, float yaw, float pitch, float roll );
	BH_DLL_EXPORT void LerpQuaternion( BH::Quaternion * dest, const BH::Quaternion * q1, const BH::Quaternion * q2, float t );
	BH_DLL_EXPORT void SlerpQuaternion( BH::Quaternion * dest, const BH::Quaternion * q1, const BH::Quaternion * q2, float t );
	BH_DLL_EXPORT void ConcatenateQuaternion( BH::Quaternion * dest, const BH::Quaternion * q1, const BH::Quaternion * q2 );

	// Getter & Setter functions
	BH_DLL_EXPORT float GetXQuaternion( const BH::Quaternion * q );
	BH_DLL_EXPORT float GetYQuaternion( const BH::Quaternion * q );
	BH_DLL_EXPORT float GetZQuaternion( const BH::Quaternion * q );
	BH_DLL_EXPORT float GetWQuaternion( const BH::Quaternion * q );
	BH_DLL_EXPORT void SetXQuaternion( BH::Quaternion * q, float val );
	BH_DLL_EXPORT void SetYQuaternion( BH::Quaternion * q, float val );
	BH_DLL_EXPORT void SetZQuaternion( BH::Quaternion * q, float val );
	BH_DLL_EXPORT void SetWQuaternion( BH::Quaternion * q, float val );

#ifdef __cplusplus
}
#endif

#endif