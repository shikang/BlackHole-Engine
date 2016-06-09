#ifndef BH_QUATERNION_BRIDGE_H
#define BH_QUATERNION_BRIDGE_H

#include "Core/CoreEssential.h"
#include "Platform/Quaternion.h"

#ifdef __cplusplus
extern "C"
{
#endif

	// Create quaternion
	__declspec( dllexport ) BH::Quaternion * CreateQuaternion();

	// Create fill quaternion
	__declspec( dllexport ) BH::Quaternion * CreateValQuaternion( float x, float y, float z, float w );

	// Create quaternion from vector
	__declspec( dllexport ) BH::Quaternion * CreateVectorQuaternion( const BH::Vector3f * v, float scalar );

	// Create copy quaternion
	__declspec( dllexport ) BH::Quaternion * CreateCopyQuaternion( const BH::Quaternion * q );

	// Destroy quaternion
	__declspec( dllexport ) void DestroyQuaternion( BH::Quaternion * q );

	// Assignment functions
	__declspec( dllexport ) void AssignQuaternion( BH::Quaternion * dest, const BH::Quaternion * src );
	__declspec( dllexport ) void PlusAssignQuaternion( BH::Quaternion * dest, const BH::Quaternion * src );
	__declspec( dllexport ) void MinusAssignQuaternion( BH::Quaternion * dest, const BH::Quaternion * src );
	__declspec( dllexport ) void ScaleAssignQuaternion( BH::Quaternion * dest, float s );
	__declspec( dllexport ) void MulAssignQuaternion( BH::Quaternion * dest, const BH::Quaternion * src );
	__declspec( dllexport ) void DivAssignQuaternion( BH::Quaternion * dest, const BH::Quaternion * src );

	// Operator functions
	__declspec( dllexport ) void UnaryQuaternion( BH::Quaternion * dest, const BH::Quaternion * src );
	__declspec( dllexport ) bool EqualityQuaternion( const BH::Quaternion * lhs, const BH::Quaternion * rhs );
	__declspec( dllexport ) bool NotEqualityQuaternion( const BH::Quaternion * lhs, const BH::Quaternion * rhs );
	__declspec( dllexport ) void PlusQuaternion ( BH::Quaternion * dest, const BH::Quaternion * lhs, const BH::Quaternion * rhs );
	__declspec( dllexport ) void MinusQuaternion ( BH::Quaternion * dest, const BH::Quaternion * lhs, const BH::Quaternion * rhs );
	__declspec( dllexport ) void MulQuaternion ( BH::Quaternion * dest, const BH::Quaternion * lhs, const BH::Quaternion * rhs );
	__declspec( dllexport ) void RotVecQuaternion ( BH::Vector3f * dest, const BH::Quaternion * lhs, const BH::Vector3f * rhs );
	__declspec( dllexport ) void DivQuaternion ( BH::Quaternion * dest, const BH::Quaternion * lhs, const BH::Quaternion * rhs );

	// Utility functions
	__declspec( dllexport ) float LengthQuaternion( const BH::Quaternion * q );
	__declspec( dllexport ) float LengthSqQuaternion( const BH::Quaternion * q );
	__declspec( dllexport ) void NormalizeQuaternion( BH::Quaternion * q );
	__declspec( dllexport ) void ConjugateQuaternion( BH::Quaternion * q );
	__declspec( dllexport ) void InverseQuaternion( BH::Quaternion * q );
	__declspec( dllexport ) float DotQuaternion( const BH::Quaternion * lhs, const BH::Quaternion * rhs );
	__declspec( dllexport ) void CrossQuaternion( BH::Quaternion * dest, const BH::Quaternion * lhs, const BH::Quaternion * rhs );
	__declspec( dllexport ) void AxisAngleQuaternion( BH::Quaternion * dest, const BH::Vector3f * axis, float angle );
	__declspec( dllexport ) void YawPitchRollQuaternion( BH::Quaternion * dest, float yaw, float pitch, float roll );
	__declspec( dllexport ) void LerpQuaternion( BH::Quaternion * dest, const BH::Quaternion * q1, const BH::Quaternion * q2, float t );
	__declspec( dllexport ) void SlerpQuaternion( BH::Quaternion * dest, const BH::Quaternion * q1, const BH::Quaternion * q2, float t );
	__declspec( dllexport ) void ConcatenateQuaternion( BH::Quaternion * dest, const BH::Quaternion * q1, const BH::Quaternion * q2 );

	// Getter & Setter functions
	__declspec( dllexport ) float GetXQuaternion( const BH::Quaternion * q );
	__declspec( dllexport ) float GetYQuaternion( const BH::Quaternion * q );
	__declspec( dllexport ) float GetZQuaternion( const BH::Quaternion * q );
	__declspec( dllexport ) float GetWQuaternion( const BH::Quaternion * q );
	__declspec( dllexport ) void SetXQuaternion( BH::Quaternion * q, float val );
	__declspec( dllexport ) void SetYQuaternion( BH::Quaternion * q, float val );
	__declspec( dllexport ) void SetZQuaternion( BH::Quaternion * q, float val );
	__declspec( dllexport ) void SetWQuaternion( BH::Quaternion * q, float val );

#ifdef __cplusplus
}
#endif

#endif