#include "Script/QuaternionBridge.h"

#ifdef __cplusplus
extern "C"
{
#endif

	__declspec( dllexport ) BH::Quaternion * CreateQuaternion()
	{
		return new BH::Quaternion();
	}

	__declspec( dllexport ) BH::Quaternion * CreateValQuaternion ( float x, float y, float z, float w )
	{
		return new BH::Quaternion( x, y, z, w );
	}

	__declspec( dllexport ) BH::Quaternion * CreateVectorQuaternion ( const BH::Vector3f * v, float scalar )
	{
		return new BH::Quaternion( *v, scalar );
	}

	__declspec( dllexport ) BH::Quaternion * CreateCopyQuaternion ( const BH::Quaternion * q )
	{
		return new BH::Quaternion( *q );
	}

	__declspec( dllexport ) void DestroyQuaternion ( BH::Quaternion * q )
	{
		delete q;
	}

	__declspec( dllexport ) void AssignQuaternion ( BH::Quaternion * dest, const BH::Quaternion * src )
	{
		*dest = *src;
	}

	__declspec(dllexport) void PlusAssignQuaternion ( BH::Quaternion * dest, const BH::Quaternion * src )
	{
		*dest += *src;
	}

	__declspec( dllexport ) void MinusAssignQuaternion ( BH::Quaternion * dest, const BH::Quaternion * src )
	{
		*dest -= *src;
	}

	__declspec( dllexport ) void ScaleAssignQuaternion ( BH::Quaternion * dest, float s )
	{
		*dest *= s;
	}

	__declspec( dllexport ) void MulAssignQuaternion ( BH::Quaternion * dest, const BH::Quaternion * src )
	{
		*dest *= *src;
	}

	__declspec( dllexport ) void DivAssignQuaternion ( BH::Quaternion * dest, const BH::Quaternion * src )
	{
		*dest /= *src;
	}

	__declspec( dllexport ) void UnaryQuaternion ( BH::Quaternion * dest, const BH::Quaternion * src )
	{
		*dest = -*src;
	}

	__declspec( dllexport ) bool EqualityQuaternion ( const BH::Quaternion * lhs, const BH::Quaternion * rhs )
	{
		return *lhs == *rhs;
	}

	__declspec( dllexport ) bool NotEqualityQuaternion ( const BH::Quaternion * lhs, const BH::Quaternion * rhs )
	{
		return *lhs != *rhs;
	}

	__declspec( dllexport ) void PlusQuaternion ( BH::Quaternion * dest, const BH::Quaternion * lhs, const BH::Quaternion * rhs )
	{
		*dest = *lhs + *rhs;
	}

	__declspec( dllexport ) void MinusQuaternion ( BH::Quaternion * dest, const BH::Quaternion * lhs, const BH::Quaternion * rhs )
	{
		*dest = *lhs - *rhs;
	}

	__declspec( dllexport ) void MulQuaternion ( BH::Quaternion * dest, const BH::Quaternion * lhs, const BH::Quaternion * rhs )
	{
		*dest = *lhs * *rhs;
	}

	__declspec( dllexport ) void RotVecQuaternion ( BH::Vector3f * dest, const BH::Quaternion * lhs, const BH::Vector3f * rhs )
	{
		*dest = *lhs * *rhs;
	}

	__declspec( dllexport ) void DivQuaternion ( BH::Quaternion * dest, const BH::Quaternion * lhs, const BH::Quaternion * rhs )
	{
		*dest = *lhs * *rhs;
	}

	__declspec( dllexport ) float LengthQuaternion( const BH::Quaternion * q )
	{
		return q->Length();
	}

	__declspec( dllexport ) float LengthSqQuaternion( const BH::Quaternion * q )
	{
		return q->LengthSq();
	}

	__declspec( dllexport ) void NormalizeQuaternion( BH::Quaternion * q )
	{
		q->Normalize();
	}

	__declspec( dllexport ) void ConjugateQuaternion( BH::Quaternion * q )
	{
		q->Conjugate();
	}

	__declspec( dllexport ) void InverseQuaternion( BH::Quaternion * q )
	{
		q->Inverse();
	}

	__declspec( dllexport ) float DotQuaternion( const BH::Quaternion * lhs, const BH::Quaternion * rhs )
	{
		return lhs->Dot( *rhs );
	}

	__declspec( dllexport ) void CrossQuaternion( BH::Quaternion * dest, const BH::Quaternion * lhs, const BH::Quaternion * rhs )
	{
		*dest = BH::Quaternion::Cross( *lhs, *rhs );
	}

	__declspec( dllexport ) void AxisAngleQuaternion( BH::Quaternion * dest, const BH::Vector3f * axis, float rhs )
	{
		*dest = BH::Quaternion::CreateFromAxisAngle( *axis, rhs );
	}

	__declspec( dllexport ) void YawPitchRollQuaternion( BH::Quaternion * dest, float yaw, float pitch, float roll )
	{
		*dest = BH::Quaternion::CreateFromYawPitchRoll( yaw, pitch, roll );
	}

	__declspec( dllexport ) void LerpQuaternion( BH::Quaternion * dest, const BH::Quaternion * q1, const BH::Quaternion * q2, float t )
	{
		*dest = BH::Quaternion::Lerp( *q1, *q2, t );
	}

	__declspec( dllexport ) void SlerpQuaternion( BH::Quaternion * dest, const BH::Quaternion * q1, const BH::Quaternion * q2, float t )
	{
		*dest = BH::Quaternion::Slerp( *q1, *q2, t );
	}

	__declspec( dllexport ) void ConcatenateQuaternion( BH::Quaternion * dest, const BH::Quaternion * q1, const BH::Quaternion * q2 )
	{
		*dest = BH::Quaternion::Concatenate( *q1, *q2 );
	}

	__declspec( dllexport ) float GetXQuaternion( const BH::Quaternion * q )
	{
		return q->x;
	}

	__declspec( dllexport ) float GetYQuaternion( const BH::Quaternion * q )
	{
		return q->y;
	}

	__declspec( dllexport ) float GetZQuaternion( const BH::Quaternion * q )
	{
		return q->z;
	}

	__declspec( dllexport ) float GetWQuaternion( const BH::Quaternion * q )
	{
		return q->w;
	}

	__declspec( dllexport ) void SetXQuaternion( BH::Quaternion * q, float val )
	{
		q->x = val;
	}

	__declspec( dllexport ) void SetYQuaternion( BH::Quaternion * q, float val )
	{
		q->y = val;
	}

	__declspec( dllexport ) void SetZQuaternion( BH::Quaternion * q, float val )
	{
		q->z = val;
	}

	__declspec( dllexport ) void SetWQuaternion( BH::Quaternion * q, float val )
	{
		q->w = val;
	}


#ifdef __cplusplus
}
#endif