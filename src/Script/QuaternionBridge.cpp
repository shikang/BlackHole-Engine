#include "Script/QuaternionBridge.h"

#ifdef __cplusplus
extern "C"
{
#endif

	BH_DLL_EXPORT BH::Quaternion * CreateQuaternion()
	{
		return new BH::Quaternion();
	}

	BH_DLL_EXPORT BH::Quaternion * CreateValQuaternion ( float x, float y, float z, float w )
	{
		return new BH::Quaternion( x, y, z, w );
	}

	BH_DLL_EXPORT BH::Quaternion * CreateVectorQuaternion ( const BH::Vector3f * v, float scalar )
	{
		return new BH::Quaternion( *v, scalar );
	}

	BH_DLL_EXPORT BH::Quaternion * CreateCopyQuaternion ( const BH::Quaternion * q )
	{
		return new BH::Quaternion( *q );
	}

	BH_DLL_EXPORT void DestroyQuaternion ( BH::Quaternion * q )
	{
		delete q;
	}

	BH_DLL_EXPORT void AssignQuaternion ( BH::Quaternion * dest, const BH::Quaternion * src )
	{
		*dest = *src;
	}

	BH_DLL_EXPORT void PlusAssignQuaternion ( BH::Quaternion * dest, const BH::Quaternion * src )
	{
		*dest += *src;
	}

	BH_DLL_EXPORT void MinusAssignQuaternion ( BH::Quaternion * dest, const BH::Quaternion * src )
	{
		*dest -= *src;
	}

	BH_DLL_EXPORT void ScaleAssignQuaternion ( BH::Quaternion * dest, float s )
	{
		*dest *= s;
	}

	BH_DLL_EXPORT void MulAssignQuaternion ( BH::Quaternion * dest, const BH::Quaternion * src )
	{
		*dest *= *src;
	}

	BH_DLL_EXPORT void DivAssignQuaternion ( BH::Quaternion * dest, const BH::Quaternion * src )
	{
		*dest /= *src;
	}

	BH_DLL_EXPORT void UnaryQuaternion ( BH::Quaternion * dest, const BH::Quaternion * src )
	{
		*dest = -*src;
	}

	BH_DLL_EXPORT bool EqualityQuaternion ( const BH::Quaternion * lhs, const BH::Quaternion * rhs )
	{
		return *lhs == *rhs;
	}

	BH_DLL_EXPORT bool NotEqualityQuaternion ( const BH::Quaternion * lhs, const BH::Quaternion * rhs )
	{
		return *lhs != *rhs;
	}

	BH_DLL_EXPORT void PlusQuaternion ( BH::Quaternion * dest, const BH::Quaternion * lhs, const BH::Quaternion * rhs )
	{
		*dest = *lhs + *rhs;
	}

	BH_DLL_EXPORT void MinusQuaternion ( BH::Quaternion * dest, const BH::Quaternion * lhs, const BH::Quaternion * rhs )
	{
		*dest = *lhs - *rhs;
	}

	BH_DLL_EXPORT void MulQuaternion ( BH::Quaternion * dest, const BH::Quaternion * lhs, const BH::Quaternion * rhs )
	{
		*dest = *lhs * *rhs;
	}

	BH_DLL_EXPORT void RotVecQuaternion ( BH::Vector3f * dest, const BH::Quaternion * lhs, const BH::Vector3f * rhs )
	{
		*dest = *lhs * *rhs;
	}

	BH_DLL_EXPORT void DivQuaternion ( BH::Quaternion * dest, const BH::Quaternion * lhs, const BH::Quaternion * rhs )
	{
		*dest = *lhs * *rhs;
	}

	BH_DLL_EXPORT float LengthQuaternion( const BH::Quaternion * q )
	{
		return q->Length();
	}

	BH_DLL_EXPORT float LengthSqQuaternion( const BH::Quaternion * q )
	{
		return q->LengthSq();
	}

	BH_DLL_EXPORT void NormalizeQuaternion( BH::Quaternion * q )
	{
		q->Normalize();
	}

	BH_DLL_EXPORT void ConjugateQuaternion( BH::Quaternion * q )
	{
		q->Conjugate();
	}

	BH_DLL_EXPORT void InverseQuaternion( BH::Quaternion * q )
	{
		q->Inverse();
	}

	BH_DLL_EXPORT float DotQuaternion( const BH::Quaternion * lhs, const BH::Quaternion * rhs )
	{
		return lhs->Dot( *rhs );
	}

	BH_DLL_EXPORT void CrossQuaternion( BH::Quaternion * dest, const BH::Quaternion * lhs, const BH::Quaternion * rhs )
	{
		*dest = BH::Quaternion::Cross( *lhs, *rhs );
	}

	BH_DLL_EXPORT void AxisAngleQuaternion( BH::Quaternion * dest, const BH::Vector3f * axis, float rhs )
	{
		*dest = BH::Quaternion::CreateFromAxisAngle( *axis, rhs );
	}

	BH_DLL_EXPORT void YawPitchRollQuaternion( BH::Quaternion * dest, float yaw, float pitch, float roll )
	{
		*dest = BH::Quaternion::CreateFromYawPitchRoll( yaw, pitch, roll );
	}

	BH_DLL_EXPORT void LerpQuaternion( BH::Quaternion * dest, const BH::Quaternion * q1, const BH::Quaternion * q2, float t )
	{
		*dest = BH::Quaternion::Lerp( *q1, *q2, t );
	}

	BH_DLL_EXPORT void SlerpQuaternion( BH::Quaternion * dest, const BH::Quaternion * q1, const BH::Quaternion * q2, float t )
	{
		*dest = BH::Quaternion::Slerp( *q1, *q2, t );
	}

	BH_DLL_EXPORT void ConcatenateQuaternion( BH::Quaternion * dest, const BH::Quaternion * q1, const BH::Quaternion * q2 )
	{
		*dest = BH::Quaternion::Concatenate( *q1, *q2 );
	}

	BH_DLL_EXPORT float GetXQuaternion( const BH::Quaternion * q )
	{
		return q->x;
	}

	BH_DLL_EXPORT float GetYQuaternion( const BH::Quaternion * q )
	{
		return q->y;
	}

	BH_DLL_EXPORT float GetZQuaternion( const BH::Quaternion * q )
	{
		return q->z;
	}

	BH_DLL_EXPORT float GetWQuaternion( const BH::Quaternion * q )
	{
		return q->w;
	}

	BH_DLL_EXPORT void SetXQuaternion( BH::Quaternion * q, float val )
	{
		q->x = val;
	}

	BH_DLL_EXPORT void SetYQuaternion( BH::Quaternion * q, float val )
	{
		q->y = val;
	}

	BH_DLL_EXPORT void SetZQuaternion( BH::Quaternion * q, float val )
	{
		q->z = val;
	}

	BH_DLL_EXPORT void SetWQuaternion( BH::Quaternion * q, float val )
	{
		q->w = val;
	}


#ifdef __cplusplus
}
#endif